
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _syscallHandler
GLOBAL printAllRegs

EXTERN getStackBase
EXTERN syscallsetter
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN saveBackup
EXTERN ncPrintReg
EXTERN rebootTerm
EXTERN contextSwitching
EXTERN ncPrintReg	;Este puntero a funcion debe ser desreferenciado para poder ser llamado

SECTION .text

%macro pushStateWithoutRax 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro pushState 0
	push rax
	pushStateWithoutRax
%endmacro

%macro popStateWithoutRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro popState 0
	popStateWithoutRax
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState
	cli

	call saveBackup

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

  mov rax, %1
  cmp rax,0
  jne .end

.timerTick
  mov rdi,rsp
  call contextSwitching
  cmp rax,0
  je .end
  mov rsp,rax

.end:
	popState
	sti
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState

	call printAllRegs

	mov rax, [rsp+24];
	printReg rax, 16

	pop rax ;rip esta "arriba"  en el stack
	printReg rax, 14

	call rebootTerm

	mov rax,400000h; muevo el nuevo rip al principio de la shell
	push rax
	call getStackBase
	mov qword [rsp+24],rax
	iretq
%endmacro


%macro syscallHandlerMaster 0
	pushStateWithoutRax
	push rdi
	mov rdi,rax
	call syscallsetter
	pop rdi

	call syscallDispatcher

	popStateWithoutRax
	iretq
%endmacro

%macro printReg 2
	mov rsi, %1
	lea rdi, [regsNames + 4 * %2]
	call ncPrintReg			;Desreferencio el puntero a funcion
%endmacro

printAllRegs: 			;Imprime los registros de la instancia en la cual se lo llamó
;	push rdi
;	printReg rsi, 1
;	pop rdi
;	printReg rdi, 0
;	printReg rax, 2
;	printReg rbx, 3
;	printReg rcx, 4
;	printReg rdx, 5
;	printReg r8, 6
;	printReg r9, 7
;	printReg r10, 8
;	printReg r11, 9
;	printReg r12, 10
;	printReg r13, 11
;	printReg r14, 12
;	printReg r15, 13
;	printReg rbp, 16
	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Opcode Exception
_exception6Handler:
	exceptionHandler 6

;Software Interruption
_syscallHandler:
	syscallHandlerMaster

haltcpu:
	cli
	hlt
	ret

section .rodata
	; dd = 4 byte value. Hacemos un "array" donde cada posicion es de 4 bytes (cada caracter ocupa 1 byte, de esta forma todos terminan en 0)
	regsNames dd "rdi", "rsi", "rax", "rbx", "rcx", "rdx", "r8 ", "r9 ", "r10", "r11", "r12", "r13", "r14", "r15", "rip", "rsp", "rbp" ; 17 registros

SECTION .bss
	aux resq 1
