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


Labels are defined with a colon (':') appended to the end. Label references are implemented as if they were hard numbers. for example:
        mov ax [myLabel]
is the same thing as:
        mov ax [50]
assuming that myLabel is at address 50.


Constants may be defined with the def command, like so:
        def null 50
They are treated the same way as labels.

Comments are created with a semicolon (';'). Everything after the ';' will be ignored, until a new line. However, there must be whitespace between code and the ';'. so:
        mov bx [bx];comment here
will not work.
        



