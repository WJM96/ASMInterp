Author:
  Wesley McClintock

Description:
  This was a project I did for school over the course of a semester. The assignment was iterative, so we started with just   implementing mov, and went from there. 

It's a sort of psuedo assembler/interpreter. It takes a plaintext file "assembles" it into machine code in memory, and then "executes" it. It supports the following operations:
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
  Comments are defined with a ';'. The rest of the line will be ignored, similar to '//' in other languages
    However, there must be whitespace between code and the ';'.
        mov bx [bx];comment    no!
        mov bx [bx] ;comment   yes!
  Constants are defined with the def command, like so: 
        def myConstant 125


        
        



