	lw  0 1 S
	lw  0 2 mcand
	lw  0 3 mplier
	lw  0 4 mask
	lw  0 5 i
	lw  0 6 n
	lw  0 7 b
start          and 3 4 7
	beq 7 0 else 
	add 1 2 1
	shl 2 4 2
	shr 3 4 3
	beq 0 0 next
else 	shr 3 4 3
	shl 2 4 2
next 	add 5 4 5
	beq 3 0 done
	beq 5 6 done
	beq 0 0 start
done halt
S	.fill 0
mcand	.fill 3
mplier	.fill 3
mask 	.fill 1
i	.fill 0
n 	.fill 16
b	.fill 0