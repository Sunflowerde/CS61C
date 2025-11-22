.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    li t0, 1 # 1 for compare
    li t4, 0 # 0 for compare
    li t1, 0 # index = 0
    bge a1, t0, loop_start # if a1 >= 1, goto loop_start
    li a0, 36
    j exit

loop_start:
    beq t1, a1, loop_end # if t1 == a1. goto loop_end
    slli t2, t1, 2 # index * 4
    add t3, a0, t2 # address of a[i]
    lw t5, 0(t3) # a[i]
    blt t5, t4, loop_continue # if a[i] < 0, goto loop_continue
    addi t1, t1, 1 # index++
    j loop_start
loop_continue:
    li t5, 0 # if a[i] < 0, then a[i] = 0
    sw t5, 0(t3) # store a[i]
    addi t1, t1, 1 # index++
    j loop_start
loop_end:
    jr ra
