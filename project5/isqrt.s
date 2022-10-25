# Name: Idris Akala, UID: 117000627, Directory ID: iakala
isqrt:
     # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8
   # BODY
    bge $a0, 2, rec
    move $v0, $a0
    j ret
rec: 
    subu $sp, $sp, 4    #save n on the stack
    sw $a0, 4($sp)
    srl $a0, $a0, 2     #shift two bits to the right

    jal isqrt           #call isqrt recursively
    sll $t0, $v0, 1     #shifts bit 1 to the right
    
    addu $t1, $t0, 1     #large + small + 1

    mul $t2, $t1, $t1   #large * large
    lw $t3, 4($sp)      

    bgt $t2, $t3, check #check if t2 > n

   
   
    move $v0, $t1       #return large
    j ret
check:
    move $v0, $t0       #return small
ret:
    move $a0, $a0
   # EPILOGUE
    move	$sp, $fp
    lw	$ra, ($fp)
    lw	$fp, -4($sp)
    jr	$ra