# 2022-10-10 Spoofing and Sniffing

## setup local machine
```bash
docker compose up

# connect to attacker container
docker exec -ti seed-attacker bash
```

### inside attacker container
- looked for all interfaces, found'br-407378364ccb'
- check promisc mode
  - `ip -d link show br-407378364cc`
    - we saw it wasn't enabled
- enable promoiscuous mode on that interface
  - `ip  link set br-407378364ccb promisc on`

## Task 1.1 A

- set iface to the one we discovered, and make scapy listen on it
```python
pkt = sniff(iface='br-407378364ccb', filter='icmp', prn=print_pkt)
```
- enter one of those 3 containers and try to ping one of the others
  - we can acutally see traffic getting through:
    ```
    ###[ Ethernet ]### 
    dst       = 02:42:84:e3:ea:c0
    src       = 02:42:0a:09:00:06
    type      = IPv4
    ###[ IP ]### 
        version   = 4
        ihl       = 5
        tos       = 0x0
        len       = 84
        id        = 5554
        flags     = 
        frag      = 0
        ttl       = 64
        proto     = icmp
        chksum    = 0x50df
        src       = 10.9.0.6
        dst       = 10.9.0.1
        \options   \
    ###[ ICMP ]### 
            type      = echo-reply
            code      = 0
            chksum    = 0xd357
            id        = 0x40
            seq       = 0x2
    ###[ Raw ]### 
            load      = '\x8c<Dc\x00\x00\x00\x00\x94\xf3\x08\x00\x00\x00\x00\x00\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./01234567'

    ```

- we can login with user `seed` (already present on the system) and try to execute the python script as we did with root earlier and see that we get a permission error: `PermissionError: [Errno 1] Operation not permitted`

---

## Task 1.1 B

### Filter only ICMP packets
- just did in task 1.1A

### Capture any TCP packet that comes from a particular IP and with a destination port number 23.
- we have these hosts:
  - 10.9.0.5
  - 10.9.0.6
- setting this filter `filter='host 10.9.0.5 and tcp and src port 23'` we are able to listen only tcp packets coming from that machine on that port, as we can see with:
  - `echo "hellooooo" | netcat 10.9.0.1 23`
    - if we run the same command on a port different than 23 or from another host, we won't sniff any packet

---

## Task 1.2 Spoofing ICMP Packets
- we can send an icmp request to our sniffer with a fake source address by simply setting it: 
```bash
ip = IP()
ip.dst = '10.9.0.1' 
icmp = ip/ICMP() 
icmp.src='192.168.4.12' # or also ip.src='...' above
sendp(icmp)
```

---

## Task 1.3 Traceroute
- we can craft our icmp packet:
```bash
a = IP()
a.src='10.9.0.5'
a.dst = '8.8.8.8' 
a.ttl = 1
b = ICMP() 
icmp=a/b
```
- experimenting with different ttls we can see that with too small ones we're not able to reach the destination, this is what we get:
   ```bash
    ###[ IP ]### 
     version   = 4
     ihl       = 5
     tos       = 0x0
     len       = 56
     id        = 0
     flags     = 
     frag      = 0
     ttl       = 37
     proto     = icmp
     chksum    = 0x6d40
     src       = 72.14.214.105
     dst       = 10.9.0.5
     \options   \
    ###[ ICMP ]### 
        type      = time-exceeded
        code      = ttl-zero-during-transit
        chksum    = 0x41f6
  ```
- instead with bigger ones (like 50) we can reach our destination, as we can see from the packet received:
  ```bash
    # [...]
    src = 8.8.8.8
  ```

---

## Task 1.4 Sniﬀing and-then Spoofing
> needed to use hwsrc and hwdst maybe to spoof arp