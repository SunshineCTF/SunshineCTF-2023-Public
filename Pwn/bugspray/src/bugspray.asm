section .data
    prompt db "AskMe >>> ", 0

section .bss
    input_buffer resb 100

section .text
    global _start

_start:
    ; Print the prompt
    mov rax, 1
    mov rdi, 1
    mov rsi, prompt
    mov rdx, 12
    syscall

    ; mmap(0x77777,300,7,0x22)
    mov rax, 0x9           ; syscall number for sys_mmap
    mov rdi, 0x777777      ; addr (let the kernel choose)
    mov rsi, 300           ; length (300 bytes)
    mov rdx, 7             ; prot (rwx permissions)
    mov r10, 0x22          ; flags (MAP_ANONYMOUS | MAP_PRIVATE)
    mov r8, -1             ; fd (ignored)
    mov r9, 0              ; offset (ignored)
    syscall

mov r12, 100
loop:
   inc r10
   cmp r10, r12
   jne loop   

    ; read(0x0,0x777777,500)
    mov rax, 0             ; syscall number for sys_read
    mov rdi, 0             ; file descriptor 0 (stdin)
    mov rsi, 0x777777
    mov rdx, 500           ; maximum number of bytes to read
    syscall

    ; syscall rax+32(0,0,0) -> goal == rax=101, ptrace(0,0,0)
    add rax, 32
    mov r11, 102
    cmp rax, r10
    jl bugspray
    cmp rax, r11
    jge bugspray

    mov rdi, 0             ; request: PTRACE_TRACEME
    mov rsi, 0             ; pid (0 for current process)
    mov rdx, 0             ; addr (unused for PTRACE_TRACEME)
    syscall

    ; Check the result of ptrace
    test rax, rax
    jz off
    xor rax, rax
    mov rcx, 100

bugspray:                  ; spray the stack with c1cada
    inc rax
    push 0xc1cada
    cmp rax, rcx
    jl bugspray
    ret

off:                       ; jmp to shellcode
   xor rdx, rdx
   mov rdx, 0x777777
   jmp rdx
