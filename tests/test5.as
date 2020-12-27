	lw	 0	 1	 x1
	lw	 0	 2	 x2
	beq	 1	 2	 next
	and	 1	 2	 3
next	halt
x1	 .fill	 6
x2	 .fill	 3