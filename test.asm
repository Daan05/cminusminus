section .data
	fmt db "Result: %d", 10, 0     ; printf format string (with newline and null terminator)

section .text
	global main
	extern printf

main:
	push rbp ; save caller's base pointer
	mov rbp, rsp ; set up a new base pointer frame for this function

	; (- (group (* (group (+ 3 5)) (group (- 6 (- 3))))))
	push 3
	push 5
	pop rax
	pop rcx
	add rax, rcx
	push rax
	push 6
	push 3
	pop rax
	neg rax
	push rax
	pop rax
	pop rcx
	sub rcx, rax
	push rcx
	pop rax
	pop rcx
	imul rax, rcx
	push rax
	pop rax
	neg rax
	push rax

	mov rdi, fmt ; 1st argument (format string)
	pop rsi ; 2nd argument (integer to print)
	xor eax, eax ; Clear RAX: required before calling variadic functions like printf
	call printf
	mov rax, 0 ; return value
	pop rbp ; Restore caller's base pointer
	ret ; Return to caller (exit program)
