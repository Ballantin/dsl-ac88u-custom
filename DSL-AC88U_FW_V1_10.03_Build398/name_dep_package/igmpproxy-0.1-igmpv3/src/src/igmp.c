/*
**  igmpproxy - IGMP proxy based multicast router
**  Copyright (C) 2005 Johnny Egeland <johnny@rlo.org>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**----------------------------------------------------------------------------
**
**  This software is derived work from the following software. The original
**  source code has been modified from it's original state by the author
**  of igmpproxy.
**
**  smcroute 0.92 - Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**  - Licensed under the GNU General Public License, version 2
**
**  mrouted 3.9-beta3 - COPYRIGHT 1989 by The Board of Trustees of
**  Leland Stanford Junior University.
**  - Original license can be found in the Stanford.txt file.
**
*/
/**
*   igmp.h - Recieves IGMP requests, and handle them
*            appropriately...
*/

#include "igmpproxy.h"

// Globals
uint32_t     allhosts_group;          /* All hosts addr in net order */
uint32_t     allrouters_group;          /* All hosts addr in net order */
#ifdef SUPPORT_IGMP_V3
uint32_t     igmpv3_allrouters_group;          /* All hosts addr in net order */
#endif

extern int MRouterFD;

/*
 * Open and initialize the igmp socket, and fill in the non-changing
 * IP header fields in the output packet buffer.
 */
void initIgmp() {
    struct ip *ip;

    recv_buf = malloc(RECV_BUF_SIZE);
    send_buf = malloc(RECV_BUF_SIZE);

    k_hdr_include(true);    /* include IP header when sending */
    k_set_rcvbuf(256*1024,48*1024); /* lots of input buffering        */
    k_set_ttl(1);       /* restrict multicasts to one hop */
    k_set_loop(false);      /* disable multicast loopback     */

    ip         = (struct ip *)send_buf;
    memset(ip, 0, sizeof(struct ip));
    /*
     * Fields zeroed that aren't filled in later:
     * - IP ID (let the kernel fill it in)
     * - Offset (we don't send fragments)
     * - Checksum (let the kernel fill it in)
     */
    ip->ip_v   = IPVERSION;
    ip->ip_hl  = sizeof(struct ip) >> 2;
    ip->ip_tos = 0xc0;      /* Internet Control */
    ip->ip_ttl = MAXTTL;    /* applies to unicasts only */
    ip->ip_p   = IPPROTO_IGMP;

    allhosts_group   = htonl(INADDR_ALLHOSTS_GROUP);
    allrouters_group = htonl(INADDR_ALLRTRS_GROUP);
#ifdef SUPPORT_IGMP_V3
    igmpv3_allrouters_group = htonl(INADDR_IGMPV3_ALLRTRS_GROUP);
#endif
}

/**
*   Finds the textual name of the supplied IGMP request.
*/
char *igmpPacketKind(u_int type, u_int code) {
    static char unknown[20];

    switch (type) {
    case IGMP_MEMBERSHIP_QUERY:     return  "Membership query  ";
    case IGMP_V1_MEMBERSHIP_REPORT:  return "V1 member report  ";
    case IGMP_V2_MEMBERSHIP_REPORT:  return "V2 member report  ";
#ifdef SUPPORT_IGMP_V3
    case IGMP_V3_MEMBERSHIP_REPORT:  return "V3 member report  ";
#endif
    case IGMP_V2_LEAVE_GROUP:        return "Leave message     ";

    default:
        sprintf(unknown, "unk: 0x%02x/0x%02x    ", type, code);
        my_log(LOG_WARNING, 0,
            "[igmpPacketKind] unk: 0x%02x/0x%02x    ", type, code);
        return unknown;
    }
}


/**
 * Process a newly received IGMP packet that is sitting in the input
 * packet buffer.
 */
void acceptIgmp(int recvlen) {
    register uint32_t src, dst, group;
    struct ip *ip;
    struct igmp *igmp;
    int ipdatalen, iphdrlen, igmpdatalen;
#ifdef SUPPORT_IGMP_V3
	int			data_left, rec_len;
	igmpv3Report		*igmpv3;
	igmpv3GroupRecord	*grp_rec;
	unsigned char *ptr, *p;
	int	rec, num_of_grprec, out_ifno=-1, ret;
#endif

    if (recvlen < sizeof(struct ip)) {
        my_log(LOG_WARNING, 0,
            "[acceptIgmp] received packet too short (%u bytes) for IP header", recvlen);
        return;
    }

	//igmpproxy_dump_packet("acceptIgmp", recv_buf, recvlen);

    ip        = (struct ip *)recv_buf;
    src       = ip->ip_src.s_addr;
    dst       = ip->ip_dst.s_addr;

    /*
     * this is most likely a message from the kernel indicating that
     * a new src grp pair message has arrived and so, it would be
     * necessary to install a route into the kernel for this.
     */
    if (ip->ip_p == 0) {
        if (src == 0 || dst == 0) {
            my_log(LOG_WARNING, 0, "[acceptIgmp] kernel request not accurate");
        }
        else {
            struct IfDesc *checkVIF;

            // Check if the source address matches a valid address on upstream vif.
            checkVIF = getIfByIx( upStreamVif );
            if(checkVIF == 0) {
                my_log(LOG_ERR, 0, "[acceptIgmp] Upstream VIF was null.");
                return;
            }
            else if(src == checkVIF->InAdr.s_addr) {
                my_log(LOG_NOTICE, 0, "[acceptIgmp] Route activation request from %s for %s is from myself. Ignoring.",
                    inetFmt(src, s1), inetFmt(dst, s2));
                return;
            }
            else if(!isAdressValidForIf(checkVIF, src)) {
                my_log(LOG_WARNING, 0, "[acceptIgmp] The source address %s for group %s, is not in any valid net for upstream VIF.",
                    inetFmt(src, s1), inetFmt(dst, s2));
                return;
            }

            // Activate the route.
            my_log(LOG_DEBUG, 0, "[acceptIgmp] Route activate request from %s to %s",
		    inetFmt(src,s1), inetFmt(dst,s2));
            activateRoute(dst, src);


        }
        return;
    }

    iphdrlen  = ip->ip_hl << 2;
    ipdatalen = ip_data_len(ip);

    if (iphdrlen + ipdatalen != recvlen) {
        my_log(LOG_WARNING, 0,
            "[acceptIgmp] received packet from %s shorter (%u bytes) than hdr+data length (%u+%u)",
            inetFmt(src, s1), recvlen, iphdrlen, ipdatalen);
        return;
    }

    igmp        = (struct igmp *)(recv_buf + iphdrlen);
#ifdef SUPPORT_IGMP_V3
	if(igmp->igmp_type == IGMP_V3_MEMBERSHIP_REPORT)
	{
	    igmpv3        = (struct igmpv3Report *)(recv_buf + iphdrlen);
		ptr = (char *)igmpv3;
		num_of_grprec = ntohs(igmpv3->number_of_grp);
		// Check validity of IGMPv3 report
		ptr += sizeof(igmpv3Report);
		data_left = ipdatalen;
		if (data_left < sizeof(igmpv3Report)) {
			my_log(LOG_WARNING, 0, "[acceptIgmp] IGMPv3_REPORT: invalid length (%d)", data_left);
	        return;
		}
        my_log(LOG_WARNING, 0, "[acceptIgmp] IGMPv3, Num Record=%d, from %s", num_of_grprec, inetFmt(src, s1));
	}
	else
#endif
	{
	    group       = igmp->igmp_group.s_addr;
	    igmpdatalen = ipdatalen - IGMP_MINLEN;
	    if (igmpdatalen < 0) {
	        my_log(LOG_WARNING, 0,
	            "[acceptIgmp] received IP data field too short (%u bytes) for IGMP, from %s",
	            ipdatalen, inetFmt(src, s1));
	        return;
	    }

	    my_log(LOG_NOTICE, 0, "[acceptIgmp] RECV %s from %-15s to %s",
	        igmpPacketKind(igmp->igmp_type, igmp->igmp_code),
	        inetFmt(src, s1), inetFmt(dst, s2) );
	}

    switch (igmp->igmp_type) {
    case IGMP_V1_MEMBERSHIP_REPORT:
    case IGMP_V2_MEMBERSHIP_REPORT:
        acceptGroupReport(src, group, igmp->igmp_type);
        return;

    case IGMP_V2_LEAVE_GROUP:
        acceptLeaveMessage(src, group);
        return;

    case IGMP_MEMBERSHIP_QUERY:
        return;

#ifdef SUPPORT_IGMP_V3
    case IGMP_V3_MEMBERSHIP_REPORT:
        acceptV3GroupReport(recv_buf, recvlen, igmp->igmp_type);
    	return;
#endif

    default:
        my_log(LOG_INFO, 0,
            "[acceptIgmp] ignoring unknown IGMP message type %x from %s to %s",
            igmp->igmp_type, inetFmt(src, s1),
            inetFmt(dst, s2));
        return;
    }
}


/*
 * Construct an IGMP message in the output packet buffer.  The caller may
 * have already placed data in that buffer, of length 'datalen'.
 */
 //shawn: this fucntion only build query message. report message will be build by kernel by IP_ADD_MEMBERSHIP opt
void buildIgmp(uint32_t src, uint32_t dst, int type, int code, uint32_t group, int datalen) {
    struct ip *ip;
    struct igmp *igmp;
    extern int curttl;
    struct Config *conf = getCommonConfig();
#ifdef SUPPORT_IGMP_V3
	igmpv3Query	*igmpv3;
	int igmp_len=0, value;
	byte exp, mant;
#ifdef ROUTER_ALERT_OPTION
	char *option = NULL;
#endif // ROUTER_ALERT_OPTION
#endif

    ip                      = (struct ip *)send_buf;
#ifdef SUPPORT_IGMP_V3
	#ifdef ROUTER_ALERT_OPTION
	ip->ip_hl      = (sizeof(struct ip)+4)>>2;
	#else
	ip->ip_hl      = sizeof(struct ip)>>2;
	#endif
#endif	
    ip->ip_src.s_addr       = src;
    ip->ip_dst.s_addr       = dst;
#ifdef SUPPORT_IGMP_V3
	#ifdef ROUTER_ALERT_OPTION
	igmp_len = MIN_IP_HEADER_LEN + 4 + sizeof(igmpv3Query);
	#else
	igmp_len = MIN_IP_HEADER_LEN + sizeof(igmpv3Query);
	#endif // ROUTER_ALERT_OPTION
    ip_set_len(ip, igmp_len);
#else
    ip_set_len(ip, MIN_IP_HEADER_LEN + IGMP_MINLEN + datalen);
#endif

    if (IN_MULTICAST(ntohl(dst))) {
        ip->ip_ttl = curttl;
    } else {
        ip->ip_ttl = MAXTTL;
    }

#ifdef SUPPORT_IGMP_V3

#ifdef ROUTER_ALERT_OPTION
	option = (send_buf + MIN_IP_HEADER_LEN);
	option[0] = 0x94;
	option[1] = 0x04;
	option[2] = 0x00;
	option[3] = 0x00;
    igmpv3                  = (igmpv3Query *)(send_buf + MIN_IP_HEADER_LEN + 4);
#else
    igmpv3                  = (igmpv3Query *)(send_buf + MIN_IP_HEADER_LEN);
#endif // ROUTER_ALERT_OPTION
    igmpv3->type 			= type;
    if (code < 128) {
	    igmpv3->MRT = code;	// the unit of this field is 1 second
    }
    else {
    	value = code;
    	exp = (byte)(value / 128) - 1;
    	mant = (byte)(value >> (exp + 3)) & 0x0f;
    	igmpv3->MRT = 0x80 | (exp<<4) | mant;
    }

   	longToByteArray((unsigned char *)&(igmpv3->address), group);
    igmpv3->s_qrv = conf->robustnessValue;
    if (conf->queryInterval < 128) {
    	igmpv3->qqic = conf->queryInterval;
    }
    else {
    	value = conf->queryInterval;
    	exp = (byte)(value / 128) - 1;
    	mant = (byte)(value >> (exp + 3)) & 0x0f;
    	igmpv3->qqic = 0x80 | (exp<<4) | mant;
    }
    igmpv3->number_of_src = 0;

    /* then compute the IGMP checksum */
    igmpv3->checksum = 0;
    igmpv3->checksum = inetChksum((u_short *)igmpv3, sizeof(igmpv3Query)); //checksum(igmpv3, sizeof(igmpv3Query));
#else
    igmp                    = (struct igmp *)(send_buf + MIN_IP_HEADER_LEN);
    igmp->igmp_type         = type;
    igmp->igmp_code         = code;
    igmp->igmp_group.s_addr = group;
    igmp->igmp_cksum        = 0;
    igmp->igmp_cksum        = inetChksum((u_short *)igmp,
                                         IGMP_MINLEN + datalen);
#endif
}

/*
 * Call build_igmp() to build an IGMP message in the output packet buffer.
 * Then send the message from the interface with IP address 'src' to
 * destination 'dst'.
 */
void sendIgmp(uint32_t src, uint32_t dst, int type, int code, uint32_t group, int datalen) {
    struct sockaddr_in sdst;
    int setloop = 0, setigmpsource = 0;
	int igmp_len=0;

    buildIgmp(src, dst, type, code, group, datalen);

    if (IN_MULTICAST(ntohl(dst))) {
        k_set_if(src);
        setigmpsource = 1;
        if (type != IGMP_DVMRP || dst == allhosts_group) {
            setloop = 1;
            k_set_loop(true);
        }
    }

    memset(&sdst, 0, sizeof(sdst));
    sdst.sin_family = AF_INET;
#ifdef HAVE_STRUCT_SOCKADDR_IN_SIN_LEN
    sdst.sin_len = sizeof(sdst);
#endif
    sdst.sin_addr.s_addr = dst;

#ifdef SUPPORT_IGMP_V3
	#ifdef ROUTER_ALERT_OPTION
	igmp_len = MIN_IP_HEADER_LEN + 4 + sizeof(igmpv3Query);
	#else
	igmp_len = MIN_IP_HEADER_LEN + sizeof(igmpv3Query);
	#endif // ROUTER_ALERT_OPTION
#else
    igmp_len = MIN_IP_HEADER_LEN + IGMP_MINLEN + datalen;
#endif

    if (sendto(MRouterFD, send_buf,
               igmp_len, 0,
               (struct sockaddr *)&sdst, sizeof(sdst)) < 0) {
        if (errno == ENETDOWN)
            my_log(LOG_ERR, errno, "[sendIgmp] Sender VIF was down.");
        else
            my_log(LOG_INFO, errno,
                "[sendIgmp] sendto to %s on %s",
                inetFmt(dst, s1), inetFmt(src, s2));
    }

    if(setigmpsource) {
        if (setloop) {
            k_set_loop(false);
        }
        // Restore original...
        k_set_if(INADDR_ANY);
    }

    my_log(LOG_DEBUG, 0, "[sendIgmp] SENT %s from %-15s to %s",
	    igmpPacketKind(type, code), src == INADDR_ANY ? "INADDR_ANY" :
	    inetFmt(src, s1), inetFmt(dst, s2));
}
