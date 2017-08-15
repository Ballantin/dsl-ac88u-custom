#!/bin/sh
#
# Copyright (C) 2009 Arcadyan
# All Rights Reserved.
#

COMMENTED=^[[:blank:]]*#
SECTION_B=0
SECTION_E=0

QUICKLEAVE_FLAG="0"
QUICKLEAVE=""
UPSTREAM_SECTION=""
UPSTREAM=""
UPSTREAM_SUBNET=""
UPSTREAM_NETMASK=""
DOWNSTREAM=""

echo "[set_igmpproxy.sh] input quickleave=$1, upstream_section=$2, downstream=$3"

if [ "$1" != "1" -a "$1" != "0" ]; then
	echo " "
	echo "Example: $0 <0/1 (quickleave)> <upstream section> <downstream interface>"
	echo " "
	exit 1
fi

if [ "$2" == "" ]; then
	echo " "
	echo "Example: $0 <0/1 (quickleave)> <upstream section> <downstream interface>"
	echo " "
	exit 1
fi

if [ "$3" == "" ]; then
	echo " "
	echo "Example: $0 <0/1 (quickleave)> <upstream section> <downstream interface>"
	echo " "
	exit 1
fi

QUICKLEAVE_FLAG=$1
if [ "$1" == "1" ]; then
	QUICKLEAVE="quickleave"
fi

UPSTREAM_SECTION=$2
DOWNSTREAM=$3

PROTO=`ccfg_cli get proto@${UPSTREAM_SECTION}`
UPSTREAM=`ccfg_cli get ifname@${UPSTREAM_SECTION}`
tmp=${PROTO:0:3}
if [ "${tmp}" == "ppp" ] ; then
	unit=${UPSTREAM_SECTION:3:3}
	if [ ${#unit} -eq 1 ] ; then
		echo -n
	elif [ ${#unit} -eq 2 ] ; then
		unit=$(( 1${unit} - 100 ))
	elif [ ${#unit} -eq 3 ] ; then
		unit=$(( 1${unit} - 1000 ))
	else
		echo"[set_igmpproxy.sh] WRONG interface name $UPSTREAM_SECTION"
		return
	fi
	UPSTREAM="ppp$unit"
fi

sync_igmpproxy_section() {
    local cfg_file="/tmp/igmpproxy.conf"

	cp /etc/igmpproxy.conf /tmp
#	ccfg_cli set quickleave@igmpproxy=$QUICKLEAVE_FLAG
#	ccfg_cli set upstream@igmpproxy=$UPSTREAM_SECTION
#	ccfg_cli set downstream@igmpproxy=$DOWNSTREAM

	# handle lan section

    set -- $(awk "BEGIN { SEC_B=0 }
             /^quickleave/ { SEC_B=NR; next; }
             /$COMMENTED/ { if(SEC_B!=0) { print 1, SEC_B, NR-1; exit; } else { next; }; }
             END { print 0 }" $cfg_file)
    found=$1

    if [ "$found" != "0" ]; then
    	start_pos=$2
    	end_pos=$3
    fi
	echo "[set_igmpproxy.sh] quickleave, found=${found}, start_pos=${start_pos}, end_pos=${end_pos}"

    set -- $(awk "BEGIN { SEC_B=0 }
             /^phyint.*upstream/ { SEC_B=NR; next; }
             /$COMMENTED/ { if(SEC_B!=0) { print 1, SEC_B, NR-1; exit; } else { next; }; }
             END { print 0 }" $cfg_file)
    found=$1

    if [ "$found" != "0" ]; then
    	start_pos=$2
    	end_pos=$3
    fi
	echo "[set_igmpproxy.sh] upstream, found=${found}, start_pos=${start_pos}, end_pos=${end_pos}"

    set -- $(awk "BEGIN { SEC_B=0 }
             /^phyint.*downstream/ { SEC_B=NR; next; }
             /$COMMENTED/ { if(SEC_B!=0) { print 1, SEC_B, NR-1; exit; } else { next; }; }
             END { print 0 }" $cfg_file)
    found=$1

    if [ "$found" != "0" ]; then
    	start_pos=$2
    	end_pos=$3
    fi
	echo "[set_igmpproxy.sh] downstream, found=${found}, start_pos=${start_pos}, end_pos=${end_pos}"

    rm /etc/igmpproxy.conf
    echo "
########################################################
#
#   Example configuration file for the IgmpProxy
#   --------------------------------------------
#
#   The configuration file must define one upstream
#   interface, and one or more downstream interfaces.
#
#   If multicast traffic originates outside the
#   upstream subnet, the "altnet" option can be
#   used in order to define legal multicast sources.
#   (Se example...)
#
#   The "quickleave" should be used to avoid saturation
#   of the upstream link. The option should only
#   be used if it's absolutely nessecary to
#   accurately imitate just one Client.
#
########################################################

##------------------------------------------------------
## Enable Quickleave mode (Sends Leave instantly)
##------------------------------------------------------
$QUICKLEAVE


##------------------------------------------------------
## Configuration for wan (Upstream Interface)
##------------------------------------------------------
phyint $UPSTREAM upstream  ratelimit 0  threshold 1
        #altnet 10.21.0.0/24


##------------------------------------------------------
## Configuration for lan (Downstream Interface)
##------------------------------------------------------
phyint $DOWNSTREAM downstream  ratelimit 0  threshold 1


##------------------------------------------------------
## Configuration for other interface (Disabled Interface)
##------------------------------------------------------
#phyint eth2 disabled
" > /etc/igmpproxy.conf

	return 0
}

sync_igmpproxy_section