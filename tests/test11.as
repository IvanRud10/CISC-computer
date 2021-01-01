	lw	 0	 1	 x1	1
	lw	 0	 2	 x2	1
start	add 	 2	 2	 2
	jmae	 1	 2	 l1
	halt
l1	jmle	 1	 2	 done	1
	beq	 0	 0	 start
done	halt
x1	 .fill 6
x2	 .fill 2