# void context_switch(context_t** old_ctx, context_t* new_ctx)
#
# Order of push and pop needs to match structure of context_t

.globl context_switch
context_switch:
    movl 4(%esp), %eax
    movl 8(%esp), %edx

    # Save old callee-saved registers
    # %eip already implicitly pushed when this procedure called
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %ebx

    # Switch stacks
    movl %esp, (%eax)
    movl %edx, %esp

    # Load new callee-saved registers
    popl %ebx
    popl %esi
    popl %edi
    popl %ebp
    # %eip implicitly popped when we call 'ret'

    ret
