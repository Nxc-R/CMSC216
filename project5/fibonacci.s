# Name: Idris Akala, UID: 117000627, Directory ID: iakala
fibonacci:
   # PROLOGUE
   subu	$sp, $sp, 16
   sw	$ra, 16($sp)
   sw	$fp, 12($sp)
   addu	$fp, $sp, 16

   # BODY
   bgt $a0, 1, rec
   move $v0, $a0

   j ret
rec:
   subu	$sp, $sp, 4	# save arg n on stack
   sw 	$a0, 4($sp)
   sub	$a0, $a0, 1	# $a0 = arg n-1
   jal  fibonacci 		# call fibonacci
   sw $v0, 8($sp)       # stores first recursive call
   
   lw $a0, 4($sp)
   sub $a0, $a0, 2
   jal fibonacci        # fibonacci(f(n-2))
   # $v0 == f(j-1)
   lw	$t1, 8($sp)	      
   add $v0, $v0, $t1  	# $v0 = fibbonaci(n-1) + fibonacci(n-2)

ret:
   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra
