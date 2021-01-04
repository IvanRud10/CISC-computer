	lw  0 1 S
	lw  0 2 mcand
	lw  0 3 mplier
	lw  0 4 i
	lw  0 5 n
	lw  0 6 b
	add 3 4 4
	add 1 2 1
cycle 	sbb 4 5 4
	beq 4 6 done
	add 1 3 1
	beq 0 0 cycle 
done halt
S	.fill 0
mcand	.fill 3
mplier	.fill 3
i	.fill 0
n	.fill 1
b	.fill 0