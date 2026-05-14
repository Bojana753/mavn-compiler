.global main

.data
m1:	.word 6
m2:	.word 5
m3:	.word 0

.text
main:
la 	$t0, m1
lw 	$t0, 0($t0)
la 	$t0, m2
lw 	$t0, 0($t0)
li 	$t0, 1
li 	$t0, 0
lab:
add 	$t0, $t0, $t0
sub 	$t0, $t0, $t0
addi 	$t0, $t0, 1
bltz 	$t0, lab
la 	$t0, m3
sw 	$t0, (0)$t0
nop 	
