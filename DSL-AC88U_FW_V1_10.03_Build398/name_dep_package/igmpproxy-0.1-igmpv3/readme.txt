wan to lan:
1. igmpproxy.conf:
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
quickleave


##------------------------------------------------------
## Configuration for eth0 (Upstream Interface)
##------------------------------------------------------
phyint eth1 upstream  ratelimit 0  threshold 1
        altnet 10.21.0.0/24


##------------------------------------------------------
## Configuration for eth1 (Downstream Interface)
##------------------------------------------------------
phyint br-lan downstream  ratelimit 0  threshold 1


##------------------------------------------------------
## Configuration for eth2 (Disabled Interface)
##------------------------------------------------------
#phyint eth2 disabled

2. begin to test:
a) run igmpproxy program
/usr/sbin/igmpproxy -d /etc/igmpproxy.conf &
b) bypass iptables
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT
c) pass through iptables by rule(when INPUT and OUPUT are set to ACCEPT)
#### iptables -I FORWARD -i eth1 -p udp --dport 1234 -j ACCEPT -o br-lan
# Lantiq's current UGW4.3 kernel needs to config software snooping first.
switch_utility MulticastSnoopCfgSet 2 1 0 3 6 1 3 100 2 1 1
# Multicast rulte to pass iptables
iptables -I INPUT -d 224.0.0.0/4 -j ACCEPT
iptables -I FORWARD -d 224.0.0.0/4 -j ACCEPT

3. use iptables to debug:
iptables -I FORWARD -j LOG => check filter tables's forwarding chain