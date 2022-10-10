#!/usr/bin/env python3
from scapy.all import *
def print_pkt(pkt):
    pkt.show()
    
all_interfaces=[
    'br-2f8364426470',
    'br-407378364ccb',
    'br-563e54b25150',
    'br-894020cbb969',
    'br-a6188c8defb6',
    'docker0',
    'eth0',
    'lo',
    'services1',
    'veth2113205',
    'veth0f0f4fd'
]


# sniff all icmp packets
# pkt = sniff(iface='br-407378364ccb', filter='icmp', prn=print_pkt)

pkt = sniff(iface='br-407378364ccb', filter='host 10.9.0.5 and tcp and src port 23', prn=print_pkt)