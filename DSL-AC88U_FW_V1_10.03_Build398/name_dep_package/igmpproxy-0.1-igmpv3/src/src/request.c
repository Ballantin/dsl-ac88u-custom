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
*   request.c
*
*   Functions for recieveing and processing IGMP requests.
*
*/

#include <stdlib.h>
#include <stdarg.h>
#include "igmpproxy.h"

// Prototypes...
void sendGroupSpecificMemberQuery(void *argument);

typedef struct {
    uint32_t      group;
    uint32_t      vifAddr;
    short       started;
} GroupVifDesc;


/**
*   Handles incoming membership reports, and
*   appends them to the routing table.
*/
void acceptGroupReport(uint32_t src, uint32_t group, uint8_t type) {
    struct IfDesc  *sourceVif;

    // Sanitycheck the group adress...
    if(!IN_MULTICAST( ntohl(group) )) {
        my_log(LOG_WARNING, 0, "[acceptGroupReport] The group address %s is not a valid Multicast group.",
            inetFmt(group, s1));
        return;
    }
    //skip local control grp
    if(is_local_network_ctrl(group)){
        //printf("[acceptGroupReport] skip local control %08x\n", group);
        return;
    }

    // Find the interface on which the report was recieved.
    sourceVif = getIfByAddress( src );
    if(sourceVif == NULL) {
        my_log(LOG_WARNING, 0, "[acceptGroupReport] No interfaces found for source %s",
            inetFmt(src,s1));
        return;
    }

    if(sourceVif->InAdr.s_addr == src) {
        my_log(LOG_NOTICE, 0, "[acceptGroupReport] The IGMP message was from myself. Ignoring.");
        return;
    }

    // We have a IF so check that it's an downstream IF.
    if(sourceVif->state == IF_STATE_DOWNSTREAM) {

        my_log(LOG_DEBUG, 0, "[acceptGroupReport] Should insert group %s (from: %s) to route table. Vif Ix : %d",
            inetFmt(group,s1), inetFmt(src,s2), sourceVif->index);

        // The membership report was OK... Insert it into the route table..
        insertRoute(group, sourceVif->index);


    } else {
        // Log the state of the interface the report was recieved on.
        my_log(LOG_INFO, 0, "[acceptGroupReport] Mebership report was recieved on %s. Ignoring.",
            sourceVif->state==IF_STATE_UPSTREAM?"the upstream interface":"a disabled interface");
    }

}

int is_local_network_ctrl(unsigned long grp_add)
{
    unsigned char *pgrp, byte1, byte2, byte3;

    if(grp_add==htonl(IGMP_UPNP_GROUP))
        return 1;

    pgrp=(unsigned char *)&grp_add;
    byte1=pgrp[0];
    byte2=pgrp[1];
    byte3=pgrp[2];

    //224.0.0.0-224.0.0.255
    if(byte1==224 && byte2==0 && byte3==0)
        return 1;

    return 0;
}

#ifdef SUPPORT_IGMP_V3
void acceptV3GroupReport(char *recv_buf, int recvlen, uint8_t type) {
    struct IfDesc  *sourceVif;
    register uint32_t src, dst, group;
    struct ip *ip;
    struct igmp *igmp;
    int ipdatalen, iphdrlen, igmpdatalen;
	int			data_left, rec_len;
	igmpv3Report		*igmpv3;
	igmpv3GroupRecord	*grp_rec;
	unsigned char *ptr, *p;
	int	rec, num_of_grprec, out_ifno=-1, ret;
	GroupVifDesc   *gvDesc;
	int	port = -1;

    ip        = (struct ip *)recv_buf;
    src       = ip->ip_src.s_addr;
    dst       = ip->ip_dst.s_addr;

	// Find the interface on which the report was recieved.
    sourceVif = getIfByAddress( src );
    if(sourceVif == NULL) {
        my_log(LOG_WARNING, 0, "[acceptV3GroupReport] No interfaces found for source %s",
            inetFmt(src,s1));
		return;
    }

   	if(sourceVif->InAdr.s_addr == src) {
       	my_log(LOG_NOTICE, 0, "[acceptV3GroupReport] The IGMP message was from myself. Ignoring.");
		return;
   	}

    // We have a IF so check that it's an downstream IF.
    if(sourceVif->state != IF_STATE_DOWNSTREAM) {
        // Log the state of the interface the report was recieved on.
        my_log(LOG_INFO, 0, "[acceptV3GroupReport] Mebership report was recieved on %s. Ignoring.",
            sourceVif->state==IF_STATE_UPSTREAM?"the upstream interface":"a disabled interface");
		return;
    }

    iphdrlen  = ip->ip_hl << 2;
    ipdatalen = ip_data_len(ip);

    if (iphdrlen + ipdatalen != recvlen) {
        my_log(LOG_WARNING, 0,
            "[acceptV3GroupReport] received packet from %s shorter (%u bytes) than hdr+data length (%u+%u)",
            inetFmt(src, s1), recvlen, iphdrlen, ipdatalen);
        return;
    }

    igmpv3        = (igmpv3Report *)(recv_buf + iphdrlen);
	ptr = (char *)igmpv3;
	num_of_grprec = ntohs(igmpv3->number_of_grp);
	// Check validity of IGMPv3 report
	ptr += sizeof(igmpv3Report);
	data_left = ipdatalen;
	if (data_left < sizeof(igmpv3Report)) {
		my_log(LOG_WARNING, 0, "[acceptV3GroupReport] IGMPv3_REPORT: invalid length (%d)", data_left);
        return;
	}
	my_log(LOG_WARNING, 0, "[acceptV3GroupReport] IGMPv3, Num Record=%d, from %s", num_of_grprec, inetFmt(src, s1));
	data_left -= sizeof(igmpv3Report);

	for (rec=0; rec<num_of_grprec; rec++) {
		if (data_left < sizeof(igmpv3GroupRecord)) {
			my_log(LOG_WARNING, 0, "[acceptV3GroupReport] IGMPv3_REPORT: 0 record length check failed (rec=%d, data_left=%d)", rec, data_left);
			return;
		}
		grp_rec = (igmpv3GroupRecord *)ptr;
		rec_len = sizeof(igmpv3GroupRecord)+(grp_rec->aux_data_len*4)+(grp_rec->number_of_src*4);
		if (data_left < rec_len) {
			my_log(LOG_WARNING, 0, "[acceptV3GroupReport] IGMPv3_REPORT: 1 record length check failed (rec=%d, data_left=%d)", rec, data_left);
			return;
		}
		data_left -= rec_len;
		ptr += rec_len;
	}
	// Process IGMPv3 report
	ptr = (char *)igmpv3;	// malone, 21/Jun/2007
	ptr += sizeof(igmpv3Report);
	for (rec=0; rec<num_of_grprec; rec++) {
		grp_rec = (igmpv3GroupRecord *)ptr;
		my_log(LOG_WARNING, 0, "[acceptV3GroupReport] \tGroup Record [%d]: Record Type=%d", rec, grp_rec->type);
		my_log(LOG_WARNING, 0, "[acceptV3GroupReport] \tGroup Record [%d]: Aux Data Len=%d", rec, grp_rec->aux_data_len);
		my_log(LOG_WARNING, 0, "[acceptV3GroupReport] \tGroup Record [%d]: Num Src=%d", rec, grp_rec->number_of_src);
		p = (unsigned char *)&grp_rec->address;
		my_log(LOG_WARNING, 0, "[acceptV3GroupReport] \tGroup Record [%d]: Multicast Address=%d.%d.%d.%d",
				rec, p[0], p[1], p[2], p[3]);
		if (grp_rec->number_of_src > 0) {
			int a;
			for (a=0; a<grp_rec->number_of_src; a++) {
				p += 4;
				my_log(LOG_WARNING, 0, "[acceptV3GroupReport] \tGroup Record [%d]: Source Address=%d.%d.%d.%d",
						rec, p[0], p[1], p[2], p[3]);
			}
		}

    	// Sanitycheck the group adress...
    	if(!IN_MULTICAST( ntohl(grp_rec->address) )) {
        	my_log(LOG_WARNING, 0, "[acceptV3GroupReport] The group address %s is not a valid Multicast group.",
            	inetFmt(grp_rec->address, s1));
        	continue;
    	}

    	//skip local control grp
    	if(is_local_network_ctrl(grp_rec->address)){
    	    //printf("[acceptV3GroupReport] skip local control %08x\n", grp_rec->address);
    	    continue;
        }

		switch (grp_rec->type) {
			case IGMP_GRPREC_MODE_IS_INCLUDE:
				if (grp_rec->number_of_src == 0) {
					// Leave the group

			        gvDesc = (GroupVifDesc*) malloc(sizeof(GroupVifDesc));

		        	my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_MODE_IS_INCLUDE, Should remove group %s (from: %s) from route table. Vif Ix : %d",
			            inetFmt(grp_rec->address,s1), inetFmt(src,s2), sourceVif->index);

			        // Tell the route table that we are checking for remaining members...
			        setRouteLastMemberMode(grp_rec->address);

			        // Call the group spesific membership querier...
			        gvDesc->group = grp_rec->address;
			        gvDesc->vifAddr = sourceVif->InAdr.s_addr;
			        gvDesc->started = 0;

			        sendGroupSpecificMemberQuery(gvDesc);
				}
				else {
					// Join the group
		        	my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_MODE_IS_INCLUDE, Should insert group %s (from: %s, port %d) to route table. Vif Ix : %d",
			        	    inetFmt(grp_rec->address,s1), inetFmt(src,s2), port, sourceVif->index);

		        	// The membership report was OK... Insert it into the route table..
		        	insertRoute(grp_rec->address, sourceVif->index);
				}
			break;

			case IGMP_GRPREC_MODE_IS_EXCLUDE:
				// Join the group
			    // We have a IF so check that it's an downstream IF.
		        my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_MODE_IS_EXCLUDE, Should insert group %s (from: %s) to route table. Vif Ix : %d",
			            inetFmt(grp_rec->address,s1), inetFmt(src,s2), sourceVif->index);

		        // The membership report was OK... Insert it into the route table..
		        insertRoute(grp_rec->address, sourceVif->index);
			break;

			case IGMP_GRPREC_CHANGE_TO_INCLUDE_MODE:
				if (grp_rec->number_of_src == 0) {
					// Leave the group

			        gvDesc = (GroupVifDesc*) malloc(sizeof(GroupVifDesc));

		        	my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_CHANGE_TO_INCLUDE_MODE, Should remove group %s (from: %s) from route table. Vif Ix : %d",
			            inetFmt(grp_rec->address,s1), inetFmt(src,s2), sourceVif->index);

			        // Tell the route table that we are checking for remaining members...
			        setRouteLastMemberMode(grp_rec->address);

			        // Call the group spesific membership querier...
			        gvDesc->group = grp_rec->address;
			        gvDesc->vifAddr = sourceVif->InAdr.s_addr;
			        gvDesc->started = 0;

			        sendGroupSpecificMemberQuery(gvDesc);
				}
				else {
					// Join the group
				    // We have a IF so check that it's an downstream IF.
			        my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_CHANGE_TO_INCLUDE_MODE, Should insert group %s (from: %s port %d) to route table. Vif Ix : %d",
				            inetFmt(grp_rec->address,s1), inetFmt(src,s2), port, sourceVif->index);

			        // The membership report was OK... Insert it into the route table..
			        insertRoute(grp_rec->address, sourceVif->index);
				}
			break;

			case IGMP_GRPREC_CHANGE_TO_EXCLUDE_MODE:
				// Join the group
			    // We have a IF so check that it's an downstream IF.
		        my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_CHANGE_TO_EXCLUDE_MODE, Should insert group %s (from: %s port %d) to route table. Vif Ix : %d",
			            inetFmt(grp_rec->address,s1), inetFmt(src,s2), port, sourceVif->index);

		        // The membership report was OK... Insert it into the route table..
		        insertRoute(grp_rec->address, sourceVif->index);
			break;

			case IGMP_GRPREC_ALLOW_NEW_SOURCES:
				// Join the group
			    // We have a IF so check that it's an downstream IF.
		        my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_ALLOW_NEW_SOURCES, Should insert group %s (from: %s port %d) to route table. Vif Ix : %d",
			            inetFmt(grp_rec->address,s1), inetFmt(src,s2), port, sourceVif->index);

		        // The membership report was OK... Insert it into the route table..
		        insertRoute(grp_rec->address, sourceVif->index);
			break;

			case IGMP_GRPREC_BLOCK_OLD_SOURCES:
				// Leave the group
		        gvDesc = (GroupVifDesc*) malloc(sizeof(GroupVifDesc));

	        	my_log(LOG_DEBUG, 0, "[acceptV3GroupReport] IGMP_GRPREC_BLOCK_OLD_SOURCES, Should remove group %s (from: %s) from route table. Vif Ix : %d",
		            inetFmt(grp_rec->address,s1), inetFmt(src,s2), sourceVif->index);

		        // Tell the route table that we are checking for remaining members...
		        setRouteLastMemberMode(grp_rec->address);

		        // Call the group spesific membership querier...
		        gvDesc->group = grp_rec->address;
		        gvDesc->vifAddr = sourceVif->InAdr.s_addr;
		        gvDesc->started = 0;

		        sendGroupSpecificMemberQuery(gvDesc);
			break;

			default:
				break;
		}
		ptr += (sizeof(igmpv3GroupRecord)+(grp_rec->aux_data_len*4)+(grp_rec->number_of_src*4));
	}
}
#endif

/**
*   Recieves and handles a group leave message.
*/
void acceptLeaveMessage(uint32_t src, uint32_t group) {
    struct IfDesc   *sourceVif;

    my_log(LOG_DEBUG, 0,
	    "[acceptLeaveMessage] Got leave message from %s to %s. Starting last member detection.",
	    inetFmt(src, s1), inetFmt(group, s2));

    // Sanitycheck the group adress...
    if(!IN_MULTICAST( ntohl(group) )) {
        my_log(LOG_WARNING, 0, "[acceptLeaveMessage] The group address %s is not a valid Multicast group.",
            inetFmt(group, s1));
        return;
    }
    //skip local control grp
    if(is_local_network_ctrl(group)){
        //printf("[acceptLeaveMessage] skip local control %08x\n", group);
        return;
    }

    // Find the interface on which the report was recieved.
    sourceVif = getIfByAddress( src );
    if(sourceVif == NULL) {
        my_log(LOG_WARNING, 0, "[acceptLeaveMessage] No interfaces found for source %s",
            inetFmt(src,s1));
        return;
    }

    // We have a IF so check that it's an downstream IF.
    if(sourceVif->state == IF_STATE_DOWNSTREAM) {

        GroupVifDesc   *gvDesc;
        gvDesc = (GroupVifDesc*) malloc(sizeof(GroupVifDesc));

        // Tell the route table that we are checking for remaining members...
        setRouteLastMemberMode(group);

        // Call the group spesific membership querier...
        gvDesc->group = group;
        gvDesc->vifAddr = sourceVif->InAdr.s_addr;
        gvDesc->started = 0;


        sendGroupSpecificMemberQuery(gvDesc);
    } else {
        // just ignore the leave request...
        my_log(LOG_DEBUG, 0, "[acceptLeaveMessage] The found if for %s was not downstream. Ignoring leave request.", inetFmt(src, s1));
    }
}

/**
*   Sends a group specific member report query until the
*   group times out...
*/
void sendGroupSpecificMemberQuery(void *argument) {
    struct  Config  *conf = getCommonConfig();

    // Cast argument to correct type...
    GroupVifDesc   *gvDesc = (GroupVifDesc*) argument;

    if(gvDesc->started) {
        // If aging returns false, we don't do any further action...
        if(!lastMemberGroupAge(gvDesc->group)) {
            return;
        }
    } else {
        gvDesc->started = 1;
    }

    // Send a group specific membership query...
    sendIgmp(gvDesc->vifAddr, gvDesc->group,
             IGMP_MEMBERSHIP_QUERY,
             conf->lastMemberQueryInterval * IGMP_TIMER_SCALE,
             gvDesc->group, 0);

    my_log(LOG_DEBUG, 0, "[sendGroupSpecificMemberQuery] Sent membership query from %s to %s. Delay: %d",
        inetFmt(gvDesc->vifAddr,s1), inetFmt(gvDesc->group,s2),
        conf->lastMemberQueryInterval);

    // Set timeout for next round...
    timer_setTimer(conf->lastMemberQueryInterval, sendGroupSpecificMemberQuery, gvDesc);

}


/**
*   Sends a general membership query on downstream VIFs
*/
void sendGeneralMembershipQuery() {
    struct  Config  *conf = getCommonConfig();
    struct  IfDesc  *Dp;
    int             Ix;

    // Loop through all downstream vifs...
    for ( Ix = 0; (Dp = getIfByIx(Ix)); Ix++ ) {
        if ( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) ) {
            if(Dp->state == IF_STATE_DOWNSTREAM) {
                // Send the membership query...
                sendIgmp(Dp->InAdr.s_addr, allhosts_group,
                         IGMP_MEMBERSHIP_QUERY,
                         conf->queryResponseInterval * IGMP_TIMER_SCALE, 0, 0);

                my_log(LOG_DEBUG, 0,
			"[sendGeneralMembershipQuery] Sent membership query from %s to %s. Delay: %d",
			inetFmt(Dp->InAdr.s_addr,s1),
			inetFmt(allhosts_group,s2),
			conf->queryResponseInterval);
            }
        }
    }

    // Install timer for aging active routes.
    timer_setTimer(conf->queryResponseInterval, ageActiveRoutes, NULL);

    // Install timer for next general query...
    if(conf->startupQueryCount>0) {
        // Use quick timer...
        timer_setTimer(conf->startupQueryInterval, sendGeneralMembershipQuery, NULL);
        // Decrease startup counter...
        conf->startupQueryCount--;
    }
    else {
        // Use slow timer...
        timer_setTimer(conf->queryInterval, sendGeneralMembershipQuery, NULL);
    }


}

