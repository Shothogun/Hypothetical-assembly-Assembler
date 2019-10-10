	TRIANGULO: EQU 1
	_Abobrinhas: EQU 2




  
		

SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
		INPUT		H
		LOAD		B
		MULT		H
		COPY		B  	,						H
		IF triangulo
		DIV		DOIS +						2
		STORE		R
		OUTPUT	R
		STOP




			    



		
SECTION DATA
	b:		sPace
	H:		SPACE 		2
	R:		SPACE
	dois:	CONST		2