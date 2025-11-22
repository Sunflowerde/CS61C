.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (read the lab spec for more information).
# The return value should be stored in a0
f:
    # Your code here
    # function return value store in a0
    addi t0, a0, 3 # index = n + 3
    slli t0, t0, 2 # index * 4
    add t1, a1, t0 # a1 + 4 * index
    lw a0, 0(t1) # load word from a1 + 4 * index

    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
