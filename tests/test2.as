	lw	 0	 1	 a	0
	lw	 0	 2	 b	0
	add 	 1 	 2	 1
	nand 	 1 	 2 	 2
	sw 	 0	 2 	 Res	0
	halt
a 	.fill 	4
b 	.fill 	2
Res 	.fill	0
