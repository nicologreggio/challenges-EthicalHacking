# Lab 12 - Format string
---

# Task 1 $-$ Crashing the Program
- write more and more %s until it crash, using [exploi1.py](solutions/exploit1.py)
  - `./exploit1.py 10 && cat badfile | nc 10.9.0.5 9090`
---
# Task 2 $-$ Printing Out the Server Program’s Memory
## 2.A - Stack data
- now using [exploit2a.py](solutions/exploit2a.py) which puts 4 characters and then several %x we can exploit it by having more than 54 bytes, since that's the difference between frame pointer and input buffer
  - `./exploit2a.py 64 && cat badfile | nc 10.9.0.5 9090`
## 2.B - Heap data
- with [exploit2b.py](solutions/exploit2b.py) we print several %x and then a %s to obtain the secret
  - `./exploit2b.py 64 && cat badfile | nc 10.9.0.5 9090`
---
# Task 3 $-$ Modifying the Server Program’s Memory
## 3.A - Change the value to a different value
- with [exploit3a.py](solutions/exploit3a.py) we do sth similar to the previous but start with the address, then several %x and finally %n which allow to actually write
  - the effect is that we write the number of characters printed out by the printf function
- `./exploit3a.py 64 && cat badfile | nc 10.9.0.5 9090`
## 3.B - Change the value to 0x5000
- similarly with [exploit3b.py](solutions/exploit3b.py) we start with the address but we wanna write sth specific
- keeping in mind that the target variable will contain the number of characters printed out by the printf function, we can use the last %x to get a counter to 0x5000 and save the counter value on the stack
- `./exploit3b.py 64 && cat badfile | nc 10.9.0.5 9090`