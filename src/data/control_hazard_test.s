	;; Control Hazard Test
	addi r2,r3,7
	beqz r0,8(r0)
	multi r6,r8,7
	div   r7,r10,r8
	addi  r8,r2,7
	addi  r9, r4,4

	;; implementation
	;; 000001 00011 00010 0000000000000111
	;; 000101 00000 00000 0000000000001000
	;; 000111 01000 00110 0000000000000111
	;; 010000 01010 01000 00111 00000000110
	;; 000001 00010 01000 0000000000000111
	;; 000001 00100 01001 0000000000000100