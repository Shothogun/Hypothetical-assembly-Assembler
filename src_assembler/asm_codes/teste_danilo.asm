	triangulo: EQU 1
SECTION DATA
	B:	SPACE				
	H:	SPACE		2
	DOIS:	CONST		2


SECTION TEXT
		R: EXTERN
		INPUT		B
		INPUT		H
Biscoit:	COPY H, DOIS
		IF	TRIANGULO
		LOAD		H
		JMP		AQUI
AQUI:		MULT		B		
		DIV		DOIS		
		STORE		R
		OUTPUT	R
Banana:		STOP	


