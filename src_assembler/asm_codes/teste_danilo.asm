	TRIANGULO: EQU 1
	_Abobrinhas: EQU 2


		
SECTION DATA
	b:		sPace
	H:		SPACE 		2
	R:		SPACE
	ZERO:	CONST		0


SECTION TEXT


		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
		INPUT		H
		LOAD		B
		JMPN	H
		jmpp	r	
		jmpz	h
		load		B
	MULT		H
		COPY		B 	,						H
		IF triangulo
		DIV		ZERO
		STORE		R
		OUTPUT	R
		JMP	H
		STOP





			    



