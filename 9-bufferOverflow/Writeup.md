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
- 