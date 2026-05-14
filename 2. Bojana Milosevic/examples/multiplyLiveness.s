			IN:
			m1
			m2
			m3
la 	r1, m1
			OUT:
			m2
			m3
			r1


			IN:
			m2
			m3
			r1
lw 	r2, 0(r1)
			OUT:
			m2
			m3
			r2


			IN:
			m2
			m3
			r2
la 	r3, m2
			OUT:
			m3
			r2
			r3


			IN:
			m3
			r2
			r3
lw 	r4, 0(r3)
			OUT:
			m3
			r2
			r4


			IN:
			m3
			r2
			r4
li 	r5, 1
			OUT:
			m3
			r2
			r4
			r5


			IN:
			m3
			r2
			r4
			r5
li 	r6, 0
			OUT:
			m3
			r2
			r4
			r5
			r6


			IN:
			m3
			r2
			r4
			r5
			r6
add 	r6, r6, r2
			OUT:
			m3
			r2
			r4
			r5
			r6


			IN:
			m3
			r2
			r4
			r5
			r6
sub 	r7, r5, r4
			OUT:
			m3
			r2
			r4
			r5
			r6
			r7


			IN:
			m3
			r2
			r4
			r5
			r6
			r7
addi 	r5, r5, 1
			OUT:
			m3
			r2
			r4
			r5
			r6
			r7


			IN:
			m3
			r2
			r4
			r5
			r6
			r7
bltz 	r7, lab
			OUT:
			m3
			r2
			r4
			r5
			r6


			IN:
			m3
la 	r8, m3
			OUT:
			r8


			IN:
			r8
sw 	r6, (0)r8
			OUT:


			IN:
nop 	
			OUT:


