	lw	 0	 1	 x1	0
	lw	 0	 2	 x2	0
	jmae	 1	 2	 l1	0
	halt
l1	jmle	 1	 3	 done	0
	div	 1	 2	 3
done	halt
x1	 .fill 10
x2	 .fill 5