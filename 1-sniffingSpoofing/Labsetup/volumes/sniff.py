#!/usr/bin/env python3
from scapy.all import *
def print_pkt(pkt):
    print(pkt.show())
    print('\n\n\n\t\t\t\t====')

def print_and_spoof(pkt):
    print('\n\n\n\t\t\t\tTRYING TO SPOOF')
    print(pkt.show())
    a = IP()
    a.hwsrc = '1.2.3.4'
    a.hwdst = '10.9.0.5' 
    icmp=a/ICMP(type=0, code=0)
    send(icmp)

    
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

# sniff only tcp packets from that host on that port
# pkt = sniff(iface='br-407378364ccb', filter='host 10.9.0.5 and tcp and src port 23', prn=print_pkt)

pkt = sniff(iface='br-0a1deadc53a1', filter='icmp', prn=print_and_spoof)