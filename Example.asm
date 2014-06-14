;;;;;;;;;;;;;;;;;;;;;;;
;This program gets numbers, doubles, and outputs them until 0 is entered
;;;;;;;;;;;;;;;;;;;;;;;

def null 0  ;not needed, just an example

loop:
	get

	cmp ax null 
	je done

	fun double 1
		ax	

	put

	jmp loop

done:

halt


;recieves:
;	 the number to be doubled as a register
;returns:
;	the doubled number via the ax register
double:
	;the stack pointer is now in bx
	;so we want to go grab our argument first
	mov [sp] bx			;save the sp
	mov bx [bx]
	sub bx 1
	
	mov ax [bx]			;ax = arg1

	mov bx [sp]
	add bx 1
	add bx ax

	mov ax [bx]

	add ax ax			;ax += ax

	mov bx [sp]
	add bx 1

	mov [bx] ax

	rfn

sp: 0

