; get and set the current time and date
GLOBAL get_date
GLOBAL set_date

get_date:
	push rbp      ; Armado de stack frame
    mov rbp, rsp  ;

	cli  				
    mov rax, rdi		; first argument
    out 0x70, al 	; get the date data
    in al, 0x71		; get the date data
    sti 	
    and rax, 0xFF

    mov rsp, rbp  ; Desarmado de stack frame
    pop rbp
    ret

set_date:
	push rbp      ; Armado de stack frame
    mov rbp, rsp  ;

	cli             
	mov rax , rdi 	; first argument
	out 0x70,al     ; set the date data
	mov rax, rsi		; second argument
	out 0x71,al 
	sti 

    mov rsp, rbp  ; Desarmado de stack frame
    pop rbp
    ret