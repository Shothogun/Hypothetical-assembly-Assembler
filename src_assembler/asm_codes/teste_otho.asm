	TRIANGULO: EQU 1
	_Abobrinhas: EQU 2




  
		
SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
INPUT 1			; Erro 9

		COPY  2,B		; Erro 9
		COPY  B,2		; Erro 9
		INPUT		H
		INPUT				; Erro 8
Z:		INPUT		23H
		ADD 		B+A				; Erro 14
		INPUT DOIS	; Erro 15
	INPUT z				; Erro 14
		COPY 		B		; Erro 8
		STOP B 			; Erro 8
		INPUTH			; Erro 5
		MOV H
		CONST 2
		LOAD		B
		MULT		H
		COPY		B +1 	,						H    + 1
		IF triangulo
		DIV		DOIS +						1
				COPY		DOIS+1,   	H		; Erro 15
		COPY		DOIS+1,   	B		
		COPY		B,							DOIS
		STORE		R
		OUTPUT	R
		AT: CONST 2

		STOP





			    



		
SECTION DATA
	A: 	CONST						; Erro 8
	C:		sPace 4 5			; Erro 8
D: space2							; Erro 8
J: SPACE f 						; Erro 14
L: CONST	P						; Erro 14
E: CONST5							; Erro 4
	b:		sPace
	H:		SPACE 		2
COPY B, DOIS
T: ALLOC 			2
	R:		SPACE
	dois:	CONST		2
		ndois:	CONST		-2
	hdois:	CONST		0x10
	nhdois:	CONST		-0x10
INPUT A
	234243fewfwe: CONST 1