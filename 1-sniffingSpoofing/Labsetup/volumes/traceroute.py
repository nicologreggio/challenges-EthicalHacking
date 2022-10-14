from scapy.all import *

a = IP()
a.src='10.9.0.5'
a.dst = '8.8.8.8' 
a.ttl = 1
b = ICMP() 
icmp=a/b
for i in range(1,100):
    icmp.ttl=i
    print(send(icmp, return_packets=True).show())


