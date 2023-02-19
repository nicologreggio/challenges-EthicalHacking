# Return to libc
## setup
- disable ASR and link sh to zsh:
  ```bash
  sudo sysctl -w kernel.randomize_va_space=0
  sudo ln -sf /bin/zsh /bin/sh
  ```
---

# Task 1 $-$ Finding out the Addresses of libc Functions
- use gdb
  ```bash
  gdb retlib
  break main
  run
  $ p system
  $ p exit
  ```
---
# Task 2 $-$ Putting the shell string in the memory
- store new global env
  ```bash
  export MY_GLOB_VAR="/bin/sh" 
  env | grep MY_GLOB_VAR
  ```
- Keep in mind that the length of the binary file is important!
- compile custom `prtenv` (smae lenght of `retlib`) and compare by putting printing var code in retlib as well that addresses match

---
# Task 3 $-$ Launching the Attack
- using the given skeleton, set the addresses found previously
- then only those "X, Y, Z" need to be figured out
- basically you may try running the program so that it takes the badfile bigger and bigger until you find out when the seg fault happens (as # of chars, so bytes)
- so for example if it happens at 28, it means that's your starting point:
  - 28:28+4 -> system addr
  - 28+4:28+8 -> exit addr
  - 28+8:28+12 -> /bin/sh address