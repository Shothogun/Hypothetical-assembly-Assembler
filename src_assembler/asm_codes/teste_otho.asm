	TRIANGULO: EQU 1
	_Abobrinhas: EQU 2




  
		
SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
		INPUT		H
		INPUT				; Erro 8
		INPUT		23H
		COPY 		B		; Erro 8
		STOP B 			; Erro 8
		INPUTH			; Erro 5
		MOV H
		CONST 2
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
	A: 	CONST						; Erro 8
	C:		sPace 4 5			; Erro 8
D: space2							; Erro 8
E: CONST5							; Erro 4
	b:		sPace
	H:		SPACE 		2
COPY B, DOIS
T: ALLOC 			2
	R:		SPACE
	dois:	CONST		2