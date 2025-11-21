.data
n .word 12

.text
main:
    add t0, x0, x0 # curr_fib = 0
    addi t1, x0, 1 # next_fib = 1
    la t3, n # load the address of the label n
    lw t3, 0(t3)
fib:
    beq t3, x0, finish # if t3 == 0, goto finish
    add t2, t1, t0 # new_fib = next_fib + cur_fib
    mv t0, t1 # curr_fib = next_fib
    mv t1, t2 # next_fib = new_fib
    addi t3, t3, -1 # i--
    j fib # jump to fib
finish:
    addi a0, x0, 1 # prepare print register
    addi a1, t0, 0
    ecall
    addi a0, x0, 10 # argument to ecall to terminate
    ecall