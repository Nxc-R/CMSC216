# Name: Idris Akala, UID: 117000627, Directory ID: iakala
is_palindrome:
# PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8
   # BODY
   jal strlen        # gets string length
   move $t0, $v0     # start at start of pointer
   move $a1, $a0     # end of pointer
   move $a2, $a0
   sub $a1, $a1, $t0 #starts at the first letter
   sub $a2, $a2, 1   #end of the word

   #li $t2, 0
   #li $v0, 1
   div $t0, $t0, 2   # divides to go halfway of the length of the string and
   add $t0, $t0, 1   # adds 1 in case its an even number
   li $t1, 0         #  iterator at 0
   li $v0, 1         # v0 set to palindrome
   loop_palindrome:
      beq $t1, $t0, ret_exit
      lb $t4,  ($a1)  
      lb $t5, ($a2)  # byte load at end
      beq $t4, $t5, continue_palindrome
      li $v0, 0
      j ret_exit
   continue_palindrome:
    add $a1, $a1, 1
    sub $a2, $a2, 1
    add $t1, $t1, 1
    j loop_palindrome   #continue loop

ret_exit:
   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr	$ra
# strlen(str)

strlen:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8
   # BODY
   li $t0, 0
   loop: 
      lb $t1, 0($a0)
      beqz $t1, endloop
      add $a0, $a0, 1
      add $t0, $t0, 1
      j loop
   endloop:
      move $v0 $t0
   # EPILOGUE
    move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr	$ra
