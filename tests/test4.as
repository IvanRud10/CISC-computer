	lw	 0	 1	 x1
	lw	 0	 2	 x2
	sbb	 1	 2	 3 
	bt	 1	 2
	rcr	 1	 2	 3
done halt
x1 .fill 7
x2 .fill 2