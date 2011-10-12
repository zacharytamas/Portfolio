#
#  @author Zachary Jones <zacharytamas@gmail.com>
#  @date April 22nd, 2011
#
#  Below is a MIPS program I wrote for school which 
#  implements and tests two functions: readIntAnyBase 
#  and printIntAnyBase. These functions parse a string 
#  given a number base and print an integer in a given 
#  base, respectively.
#

.text
.globl main

    main:
        jal readIntAnyBase
        # the integer is in $v1
        
        bltz $v1, invalidNumber
        move $a1, $v1
        
        # Show base 2
        li $v0, 4
        la $a0, base2
        syscall
        li $a2, 2
        jal printIntAnyBase
        
        # base 10
        li $v0, 4
        la $a0, base10
        syscall
        li $a2, 10
        jal printIntAnyBase
        
        # base 16
        li $v0, 4
        la $a0, base16
        syscall
        li $a2, 16
        jal printIntAnyBase
        
        # base 36
        li $v0, 4
        la $a0, base36
        syscall
        li $a2, 36
        jal printIntAnyBase
        
        
        li $v0, 4
        la $a0, anotherPrompt
        syscall
        
        li $v0, 12 # get a character from the user for determining whether they wanna go again
        syscall
        
        beq $v0, 89, main
        beq $v0, 121, main
        
        li $v0, 10
        syscall
        
        invalidNumber:
            li $v0, 4
            la $a0, invalidNumberMsg
            syscall
            j main

    
    #
    #  Reads in an integer as a string in any base from 2-36
    #
    #  @param $a1 The base to interpret the number as
    #  @return in $v1
    #
    readIntAnyBase:
        # our output will be in $v1, we're initializing it
        move $v1, $zero
        move $a2, $zero		# pointer index
        
        li $v0, 4		# print prompt to ask for string
        la $a0, readPrompt
        syscall
        
        li $v0, 8		# grab the input string
        la $a0, strBuffer
        li $a1, 32
        syscall
        
        li $v0, 4		# print prompt to ask for base
        la $a0, readBasePrompt
        syscall
        
        li $v0, 5		# read integer base
        syscall
        
        move $a1, $v0		# move that integer into $a1 register
        
        
        readLoop:
            lb $a3, strBuffer($a2)
            beq $a3, 10, readOut  # jump out if we reach the point where they hit enter
            
            mult $v1, $a1	# multiply our current value by the base
            mflo $v1		# store the mult result back into the value var
            
            bgt $a3, 122, isInvalid
            bgt $a3, 96, isLowercase
            bgt $a3, 90, isInvalid
            bgt $a3, 64, isUppercase
            bgt $a3, 57, isInvalid
            bgt $a3, 47, isDigit
            j isInvalid		# if it hasn't matched yet, it's invalid
            
            isUppercase:
            subi $a3, $a3, 55
            j checkForInvalid
            
            isLowercase:
            subi $a3, $a3, 87
            j checkForInvalid
            
            isDigit:
            subi $a3, $a3, 48
            j checkForInvalid
            
            checkForInvalid:
            blt $a3, $a1, continue # if the number is less than the base, continue: it's valid
            j isInvalid
            
            isInvalid:
            li $v1, -1 # set the output to -1, which signals there were problems
            jr $ra  # jump out because we encounted an invalid character
            
            
            continue:
            add $v1, $v1, $a3		# add the value we calculated from this character
            addi $a2, $a2, 1		# increment pointer to the next character
            j readLoop
            
        readOut:
            jr $ra		# return, we're done.
            
    #
    #  Prints a string representing an integer in any base from 2-36
    #
    #  @param $a0 The integer to be printed
    #  @param $a2 The base
    #
    printIntAnyBase:
    
    	addi $v1, $zero, 1	# set the initial value to one
    	move $t0, $zero
    	
    	lengthLoop:
    	    bgt $v1, $a1, lengthDetermined
    	    mult $v1, $a2	# multiply value by the base
    	    mflo $v1		# store result in the value's place
    	    addi $t0, $t0, 1	# increment the length
    	    j lengthLoop
    	
    	lengthDetermined:    
    	    move $t1, $a1 # move the number we're converting to the quotient variable
    	    sb $zero, strBuffer($t0) # set a null terminator
    	    j decrementPointer
    	    
    	conversionLoop:
    	    beqz $t1, conversionFinished
    	    div $t1, $a2	# divide by the base, quotient is in LO, remainder in HI
    	    mflo $t1
    	    mfhi $t2
    	    bgt $t2, 9, charIsLetter
    	    # if the remainder is less than 10 it will just continue on to the block after this
    	
    	charIsDigit:
    	    addi $t3, $t2, 48
    	    j writeChar
    	
    	charIsLetter:
    	    addi $t3, $t2, 55
    	
    	writeChar:
    	    sb $t3, strBuffer($t0)  # stores the ascii character we generated in its place in the buffer
    	
    	decrementPointer:
    	    subi $t0, $t0, 1
    	    j conversionLoop

        conversionFinished:
            j printOut
                        
        printOut:
	    li $v0, 4
            la $a0, strBuffer
            syscall
            jr $ra
        
    
# --------------------------------------------------------
.data
    strBuffer:		.space 33
    readPrompt:		.asciiz "\n\nPlease supply a number: "
    readBasePrompt:	.asciiz "What base is that number? "
    base2:		.asciiz "\nBase  2 : "
    base10:		.asciiz "\nBase 10 : "
    base16:		.asciiz "\nBase 16 : "
    base36:		.asciiz "\nBase 36 : "
    anotherPrompt:	.asciiz "\n\nAnother? (y or n) "
    invalidNumberMsg:	.asciiz "That number is invalid! Nice try..."
