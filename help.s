
	sw	$s1 0($fp) ;nothing exciting
	la	$s1 int_const1 ;load int_const1 OBJECT into s1
	la	$a0 int_const2 ;load int_const2 OBJECT into a0 for copying
	jal	Object.copy ;copy int_const2 into a0
	lw	$t2 12($a0) ;load VALUE of a0 (int_const2 copy) into t2
	lw	$t1 12($s1) ;load VALUE of a1 into t1
	add	$t1 $t1 $t2 ;result of addition into t1
	sw	$t1 12($a0) ;now put the result into our newly copied object (this is the result)
	sw	$a0 0($sp)  ;put a0 on stack in prep for function call
	addiu	$sp $sp -4 ;prep function call
	move	$a0 $s0 ;prep function call
	bne	$a0 $zero label0 ;...
