	lw	 0	 1	 a	1
	lw	 0	 2	 b	1
	xidiv 	 1 	 2	 3
	xadd 	 3 	 2 	 3
	sw 	 0	 2 	 Res	1
	halt
a 	.fill 	36
b 	.fill 	12
Res 	.fill	0
