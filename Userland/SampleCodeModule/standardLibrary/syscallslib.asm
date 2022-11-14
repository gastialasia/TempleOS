GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysPrintMem
GLOBAL sysClear
GLOBAL sysDate
GLOBAL sysSleep
GLOBAL sysInforeg
GLOBAL divzero
GLOBAL opcode
GLOBAL sysGetLast
GLOBAL sysSnapshotRegs
GLOBAL sysMalloc
GLOBAL sysFree
GLOBAL sysMemStatus
GLOBAL sysCreateProcess
GLOBAL sysExit
GLOBAL sysGetpid
GLOBAL sysPs
GLOBAL sysKill
GLOBAL sysChangepriority
GLOBAL sysChangestate
GLOBAL sysYield
GLOBAL sysSemOpen
GLOBAL sysSemClose
GLOBAL sysSemPost
GLOBAL sysSemWait
GLOBAL sysGetAllSems
GLOBAL sysCreatePipe
GLOBAL sysGetAllPipes
GLOBAL sysOpenPipe
GLOBAL sysCloseFd
GLOBAL sysPipeRead
GLOBAL sysPipeWrite
GLOBAL sysOpenShm

section .text

sysWrite:          ; Los registros de rdi, rsi y rdx ya están cargados por los parámetros de C
    mov rax,2
    int 80h
    ret

sysRead:
    mov rax,1
    int 80h
    ret

sysPrintMem:
    mov rax,4
    int 80h
    ret

sysClear:
    mov rax,3
    int 80h
    ret

sysDate:
    mov rax, 6
    int 80h
    ret

sysSleep:
    mov rax, 7
    int 80h
    ret

sysInforeg:
    mov rax, 5
    int 80h
    ret

sysGetLast:
    mov rax, 8
    int 80h
    ret

sysSnapshotRegs:
    mov rax, 9
    int 80h
    ret

sysMalloc:
    mov rax, 10
    int 80h
    ret

sysFree:
    mov rax, 11
    int 80h
    ret

sysMemStatus:
    mov rax, 12
    int 80h
    ret

sysCreateProcess:
    mov rax, 13
    int 80h
    ret

sysExit:
    mov rax, 14
    int 80h
    ret

sysGetpid:
    mov rax, 15
    int 80h
    ret

sysPs:
  mov rax, 16
  int 80h
  ret

sysKill:
  mov rax, 17
  int 80h
  ret

sysChangepriority:
  mov rax, 18
  int 80h
  ret

sysChangestate:
  mov rax, 19
  int 80h
  ret

sysYield:
  mov rax, 20
  int 80h
  ret

sysSemOpen:
  mov rax, 21
  int 80h
  ret

sysSemClose:
  mov rax, 22
  int 80h
  ret

sysSemPost:
  mov rax, 23
  int 80h
  ret

sysSemWait:
  mov rax, 24
  int 80h
  ret

sysGetAllSems:
  mov rax, 25
  int 80h
  ret

sysCreatePipe:
  mov rax, 26
  int 80h
  ret

sysGetAllPipes:
  mov rax, 27
  int 80h
  ret

sysOpenPipe:
  mov rax, 28
  int 80h
  ret

sysPipeRead:
  mov rax, 29
  int 80h
  ret

sysPipeWrite:
  mov rax, 30
  int 80h
  ret

sysCloseFd:
  mov rax, 31
  int 80h
  ret

sysOpenShm:
mov rax, 32
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
