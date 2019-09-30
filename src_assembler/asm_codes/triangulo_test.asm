	TRIANGULO: EQU 0
	_Abobrinhas: EQU 2




  
		

SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
		INPUT		H
		LOAD		B
		MULT		H
		COPY		B  	,						H
		IF triangulo
		DIV		DOIS
		STORE		R
		OUTPUT	R
		STOP




			    



		
SECTION DATA
	b:		sPace
	H:		SPACE
	R:		SPACE
	dois:	CONST		2
