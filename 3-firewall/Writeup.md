# Firewall
## Environment setup
```mermaid
flowchart TD
  subgraph Network 1
    N["Net. 10.9.0.0/24"]
    N o--o Attacker
    N o--o 5["10.9.0.5"]
  end
  N o--o R["10.9.0.11\nRouter\n192.168.60.11"]
  R o--o N2
  subgraph Network 2
    N2["Net. 192.168.60.0/24"]
    N2 o--o 605["192.168.60.5"]
    N2 o--o 606["192.168.60.6"]
    N2 o--o 607["192.168.60.7"]
  end
```

# Task 1.A
- install needed stuff
  ```bash
  apt install build-essential linux-generic
  ```
- fix the makefile: `find / | grep include/generated/autoconf.h`
- replace `/lib/modules.../` in makefile with the one found
  - in our case: `/usr/src/linux-headers-5.10.0-1026-oem`
- but didn't work...
  - tried also: `make -C /lib/modules/5.4.0-131-generic/build M=$(PWD) clean`
    - but still no luck...
      - (looks like in mac os is not as straightforward as on Linux due to the docker implementation, installing a kernel module directly on mac os is nearly impossible due to system limitations, a viable way might be exploiting the linuxkit vm on top of which docker engine for mac os runs)
- run (finally) make
- module related commands
  | command | desc |
  | --- | --- |
  | `sudo insmod hello.ko`  | inserting a module |
  | `lsmod \| grep hello` | list modules 
  | `sudo rmmod hello` | remove the module
  | `dmesg` | check the messages |

--- 

# Task 1.B
:(