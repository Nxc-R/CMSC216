# Name: Idris Akala, UID: 117000627, Directory ID: iakala
reverse_prefix_sum:
   # PROLOGUE
   subu	$sp, $sp, 16
   sw	$ra, 16($sp)
   sw	$fp, 12($sp)
   addu	$fp, $sp, 16
   # BODY
   lw $t1, ($a0)          # loads r
   bne $t1, -1, rec_call  #check to see if we reached end of array, if not call recursive label
   li $v0, 0              # return 0 when end of array is reached
   j ret
rec_call:
   sw 	$a0, 8($sp)      #store a0
   sw    $t1, 4($sp)      #store t1
   addu  $a0, $a0, 4

   jal reverse_prefix_sum  #call reverse_prefix_sum recursively


   lw $t1, -12($fp)        #loads t1
   lw $a0, -8($fp)         #loads a0

   move $t2, $t1           #move t1 to t2
   addu $v0, $v0, $t2      #add t2 to v0
   sw $v0, 0($a0)          #store v0 value
ret:
   # EPILOGUE
    move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr	$ra