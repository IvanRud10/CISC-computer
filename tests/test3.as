	lw	0	1	a	1
	lw	0	2	b	1
	lw	0	3	Res	1
	cmpl	1	2	3
	sw	0	3	Res	1
	halt
a	.fill	5
b	.fill	2
Res	.fill	0