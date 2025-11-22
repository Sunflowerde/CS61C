.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    li t0, 1 # 1 for compare
    li t1, 0 # initial index for max_num
    lw t6, 0(a0) # initial max_num for compare
    li t2, 0 # index for iterate
    bge a1, t0, loop_start # if len >= 1, then goto loop
    li a0, 36
    j exit
loop_start:
    beq t2, a1, loop_end # iterate over
    slli t3, t2, 2 # index * 4
    add t4, a0, t3 # address of a[i]
    lw t5, 0(t4) # a[i]
    bgt t5, t6, loop_continue # if a[i] > max_num, goto loop_continue 
    addi t2, t2, 1 # i++
    j loop_start
loop_continue:
    addi t1, t2, 0 # update index
    addi t6, t5, 0 # update max_num
    addi t2, t2, 1 # index++
    j loop_start
loop_end:
    addi a0, t1, 0 # return max_num_index
    jr ra