; ----------------------------------
; RC4.asm 
;   Stephen A. Ridley stephen@sa7ori.org 2009
;   for CSAW 2009 (NYUPoly)
; -----------------------------------   
; Originally I was going to use this as the basis
; for my "flag" obfuscation. The reason I originally wanted to do this as asm
; was because I had this elaborate scheme for implementing this entire bit
; as polymorphic shellcode, so that the "signature" of the flag decryption
; was not reconizable in the disassembly. compiling this, and hiding it not as
; a character buffer but as "_emit 0xbyte" style inline assembly I thought was
; going to be the best idea. I decided against it becaues I procrastinated
; and ran out of time, plus people said they thought it would be too hard

bits 32

        section .text
                align 4
                global _start ;declares entry point for execution

        _start:
		jmp .LOAD_RC4_STATE
	.LOAD_RC4_STATE_RETURN:
		pop esi
		jmp .LOAD_RC4_KEYS
	.LOAD_RC4_KEYS_RETURN:
		pop edi
		jmp .LOAD_RC4_KEYS_LENGTH
	.LOAD_RC4_KEYS_LENGTH_RETURN:
		pop ecx
		mov ecx, [ecx]
		push ecx ;key_length
		push edi ;keys
		push esi ;state
		call .RC4_SETUP ;tested and compatible
		jmp .LOAD_RC4_DATA
	.LOAD_RC4_DATA_RETURN:
		pop edi
		jmp .LOAD_RC4_DATA_LENGTH
	.LOAD_RC4_DATA_LENGTH_RETURN:
		pop ecx
		mov ecx,[ecx]
		push ecx
		push edi
		push esi
		call .RC4_CRYPT ;tested and compatible
		call .END

			.RC4_CRYPT: ;RC4_CRYPT(state, data, data_length)
			;after pushad
			;esp+36 state	(esi)
			;esp+40 data	(edi)
			;esp+44 length	(edx)
				pushad
				mov esi, [esp+36]
				mov edi, [esp+40]
				mov ecx, [esp+44]
				push ecx
				push 0

			.CRYPTIT:
				pop eax ;index
				pop ecx ;length
				cmp eax, ecx
				jge .CRYPT_DONE
				push ecx
				push eax
				xor eax, eax
				xor ebx, ebx
				xor ecx, ecx
				xor edx, edx
				mov al, [esi]
				inc al
				shl eax, 24
				shr eax, 24 ;this makes sure nothing else is in eax other than al
				mov bl, [esi+2+eax]
				mov [esi], al
				mov al, [esi+1]
				add al, bl
				mov [esi+1], al
				shl eax, 24
				shr eax, 24 ;this makes sure nothing else is in eax other than al
				mov cl, [esi+2+eax]
				mov dl, [esi]
				mov [esi+2+edx], cl
				mov [esi+2+eax], bl
				;a == bl
				;b == cl
				add bl, cl
				;bl == a+b
				shl ebx, 24
				shr ebx, 24 ;this makes sure nothing else is in ebx other than bl
				mov al, [esi+2+ebx]
				pop edx
				mov bl, [edi+edx]
				xor bl, al
				mov [edi+edx], bl
				inc edx
				push edx
				jmp .CRYPTIT
				
			.CRYPT_DONE
				popad
				ret 12
				
			.RC4_SETUP:
			;after pushad
			;esp+36 state 	(esi)
			;esp+40 keys 	(edi)
			;esp+44 key_length (ecx)
				pushad
				mov esi, [esp+36]
				mov edi, [esp+40]
				mov ecx, [esp+44]
				xor eax, eax
			.INIT_M:
				mov [esi+2+eax], al
				inc eax
				cmp eax, 256
				jl .INIT_M
				push ecx ;length
				xor ebx, ebx ;j
				xor ecx, ecx ;i
				xor edx, edx ;k
			.KEY_M:
				xor eax, eax ;a
				mov al, [esi+2+ecx]
				add bl, al
				add bl, [edi+edx]
				shl ebx, 24
				shr ebx, 24 ;this makes sure nothing else is in ebx other than bl
				push edx
				mov dl, [esi+2+ebx]
				mov [esi+2+ecx], dl
				pop edx
				mov [esi+2+ebx], al
				pop eax
				inc edx
				cmp edx, eax
				jl .KEY_M_CONTINUE
				xor edx, edx
			.KEY_M_CONTINUE:
				inc ecx
				cmp ecx, 256
				jge .KEY_M_DONE
				push eax ;save off length
				jmp .KEY_M
			.KEY_M_DONE:			
				popad
				ret 12
				
				
				
	.LOAD_RC4_STATE:
		call .LOAD_RC4_STATE_RETURN
		db 0x0 ;byte x
		db 0x0 ;byte y
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 0x0
		
	.LOAD_RC4_KEYS:
		call .LOAD_RC4_KEYS_RETURN
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 0x0

	.LOAD_RC4_KEYS_LENGTH:
		call .LOAD_RC4_KEYS_LENGTH_RETURN
		dd 256
		
	.LOAD_RC4_DATA:
		call .LOAD_RC4_DATA_RETURN
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
		dd 0x0

	.LOAD_RC4_DATA_LENGTH:
		call .LOAD_RC4_DATA_LENGTH_RETURN
		dd 256

	.END:	
	.JUMPER:
		jmp .JUMPER
	
