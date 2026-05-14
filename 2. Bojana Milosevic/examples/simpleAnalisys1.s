			IN:
			m1
			m2
la 	r4, m1
			OUT:
			m2
			r4


			IN:
			m2
			r4
lw 	r1, 0(r4)
			OUT:
			m2
			r1


			IN:
			m2
			r1
la 	r5, m2
			OUT:
			r1
			r5


			IN:
			r1
			r5
lw 	r2, 0(r5)
			OUT:
			r1
			r2


			IN:
			r1
			r2
add 	r3, r1, r2
			OUT:
			r2
			r3


			IN:
			r2
			r3
div 	r5, r3, r2
			OUT:
			r3
			r5


			IN:
			r3
			r5
or 	r1, r3, r5
			OUT:
			r1
			r3


			IN:
			r1
			r3
add 	r4, r1, r3
			OUT:
			r1
			r3


			IN:
			r1
			r3
addi 	r3, r3, 1
			OUT:
			r1
			r3


			IN:
			r1
			r3
bgtz 	r1, petlja
			OUT:
			r1
			r3


