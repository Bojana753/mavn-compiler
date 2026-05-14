.global main

.data
m1:	.word 6
m2:	.word 5

.text
main:
la 	$t0, m1
lw 	$t0, 0($t0)
la 	$t0, m2
lw 	$t0, 0($t0)
add 	$t0, $t0, $t0
div 	$t0, $t0, $t0
or 	$t0, $t0, $t0
petlja:
add 	$t0, $t0, $t0
addi 	$t0, $t0, 1
bgtz 	$t0, petlja
