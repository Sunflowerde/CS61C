.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    ebreak # for debug
    li t0, 0 # sum = 0
    li t1, 0 # iterate for a1
    li t2, 0 # iterate for a2
    li t3, 1 # 1 for compare
    li t4, 0 # step = 0   
    blt a2, t3, exit1 
    blt a3, t3, exit2
    blt a4, t3, exit2
loop_start:
    beq t4, a2, loop_end # if step == a2, goto end
    slli t5, t1, 2 # index1 * 4
    slli t6, t2, 2 # index2 * 4
    add t3, a0, t5 # address of a1[i]
    add a5, a1, t6 # address of a2[i]
    lw a6, 0(t3) # a1[i]
    lw a7, 0(a5) # a2[i]
    mul a7, a6, a7 # a1[i] * a2[i]
    add t0, t0, a7
    add t1, t1, a3 # index1 + stride1
    add t2, t2, a4 # index2 + stride2
    addi t4, t4, 1 # step++
    j loop_start
loop_end:
    addi a0, t0, 0
    jr ra
exit1:
    li a0, 36
    j exit
exit2:
    li a0, 37
    j exit