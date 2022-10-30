GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_printMem
GLOBAL sys_clear
GLOBAL sys_date
GLOBAL sys_sleep
GLOBAL sys_inforeg
GLOBAL divzero
GLOBAL opcode
GLOBAL sys_getLast
GLOBAL sys_snapshotRegs
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_memStatus
GLOBAL sys_createProcess
GLOBAL sys_exit
GLOBAL sys_getpid
GLOBAL sys_ps
GLOBAL sys_kill
GLOBAL sys_changepriority

section .text

sys_write:          ; Los registros de rdi, rsi y rdx ya están cargados por los parámetros de C
    mov rax,2
    int 80h
    ret

sys_read:
    mov rax,1
    int 80h
    ret

sys_printMem:
    mov rax,4
    int 80h
    ret

sys_clear:
    mov rax,3
    int 80h
    ret

sys_date:
    mov rax, 6
    int 80h
    ret

sys_sleep:
    mov rax, 7
    int 80h
    ret

sys_inforeg:
    mov rax, 5
    int 80h
    ret

sys_getLast:
    mov rax, 8
    int 80h
    ret

sys_snapshotRegs:
    mov rax, 9
    int 80h
    ret

sys_malloc:
    mov rax, 10
    int 80h
    ret

sys_free:
    mov rax, 11
    int 80h
    ret

sys_memStatus:
    mov rax, 12
    int 80h
    ret

sys_createProcess:
    mov rax, 13
    int 80h
    ret

sys_exit:
    mov rax, 14
    int 80h
    ret

sys_getpid:
    mov rax, 15
    int 80h
    ret

sys_ps:
  mov rax, 16
  int 80h
  ret

sys_kill:
  mov rax, 17
  int 80h
  ret

sys_changepriority:
  mov rax, 18
  int 80h
  ret

divzero:
  mov rax, 4
  xor rbx, rbx
  div rbx
  ret

 

opcode:
  ud2   ;undefined instruction, para tirar el opcode exception.
  ret
