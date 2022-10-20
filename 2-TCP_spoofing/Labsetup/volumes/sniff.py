#!/usr/bin/env python3
from scapy.all import *
def print_pkt(pkt):
    print(pkt.show())
    print('\n\n\n\t\t\t\t====')


iface='br-0a1deadc53a1'

# sniff all icmp packets
# pkt = sniff(iface='br-407378364ccb', filter='icmp', prn=print_pkt)

# sniff only tcp packets from that host on that port
# pkt = sniff(iface='br-407378364ccb', filter='host 10.9.0.5 and tcp and src port 23', prn=print_pkt)

pkt = sniff(iface=iface, filter='tcp', prn=print_pkt)