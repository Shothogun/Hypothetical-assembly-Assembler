	TRIANGULO: EQU 1
	_Abobrinhas: EQU 2




  
		
SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
		INPUT		H
		INPUT		23H
		COPY 		B		; Erro 8
		MOV H
		CONST 2
		SECTION TEXT		
		LOAD		B
		MULT		H
		COPY		B +1 	,						H    + 2
		IF triangulo
		DIV		DOIS +						1
		COPY		DOIS+1,   	B		
		COPY		B,							DOIS
		STORE		R
		OUTPUT	R
		STOP





			    



		
SECTION DATA
qwewqewqewq 2
	b:		sPace
	H:		SPACE 		2
COPY B, DOIS
T: ALLOC 			2
	R:		SPACE
	dois:	CONST		2