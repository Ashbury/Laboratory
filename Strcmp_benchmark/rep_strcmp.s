section .text
extern strlen
global rep_strcmp

rep_strcmp:
	push rbp
	mov rbp, rsp
	push rdi
	call strlen
	pop rdi
	mov rcx, rax
	cld
	repz cmpsb
	mov al, [rdi - 1]
	sub al, [rsi - 1]
	movsx eax, al
	pop rbp
	ret
