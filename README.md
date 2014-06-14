Author:
  Wesley McClintock

Description:
  This was a project I did for school over the course of a semester. The assignment was iterative, so we started with just   implementing mov, and went from there. 

It's a sort of psuedo assembler/interpreter. The instruction set is 8-bit, but the actual memory is 16-bit. It takes a plaintext file "assembles" it into machine code in memory, and then "executes" it. It supports the following operations:

        mov
        add
        sub
        and
        or
        not
        jmp
        j<e, ne, a, ae, b, be>
        cmp
        halt
        fun (call)
        rfn (ret)
        
Labels, constant definition, and comments are all supported. 
  Labels are defined by appending a ':'. They are referenced just like hardcoded numbers
  Comments are defined with a ';'. The rest of the line will be ignored, similar to '//' in other languages. However, there must be whitespace between code and the ';'.

        mov bx [bx];comment    no!

        mov bx [bx] ;comment   yes!
  Constants are defined with the def command, like so: 

        def myConstant 125



The function command -- fun -- is used like so:
        
        fun <address> <number of arguments>
            <parameter 1>
            <parameter 2>
            ...
            <parameter n>
            
  While recursion works, it fills up memory /very/ fast.
        



Registers:

  general purpose, usable in code:
  
    ax
    bx
    cx
    dx
    
  only used by cmp and conditional jumps:
  
    flag
    
  only used by fun and rfn:
  
    sp (stack pointer)
    instrAddr (instruction pointer)
  
    

