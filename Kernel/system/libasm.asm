GLOBAL cpuVendor
GLOBAL rtcGetter
GLOBAL saveRegisters
GLOBAL _xchg

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

rtcGetter:
	; Recibe como parametro en dil (parte baja de rdi) un char que corresponde al tipo de dato que quiero del RTC. Lo devuelve en hexa.

	push rbp
	mov rbp, rsp

	mov al, dil
	out 0x70, al
	in al, 0x71

	mov rsp, rbp
	pop rbp
	ret

%macro saveRegister 1
	mov rax, [rsp + %1]
	mov [rdi], rax
	add rdi, 8
%endmacro

saveRegisters:
	saveRegister 136  ; rax
	saveRegister 128
	saveRegister 120
	saveRegister 112
	saveRegister 104
	saveRegister 96
	saveRegister 88
	saveRegister 80
	saveRegister 72
	saveRegister 64
	saveRegister 56
	saveRegister 48
	saveRegister 40
	saveRegister 32
	saveRegister 24		; r15
  	ret

_xchg:
  ; se usa xcgh pq garantiza atomicidad aunque no sea eficiente
  mov rax,rsi
  xchg [rdi],eax
  ret



section .bss
struct_reg resb 128

