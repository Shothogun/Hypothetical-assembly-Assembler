TEST: BEGIN
	triangulo: EQU 1
SECTION DATA
	B:	SPACE				
	H:	SPACE		2
	R:	SPACE 
	DOIS:	CONST		2


SECTION TEXT
		PUBLIC DOIS
		PUBLIC H
		PUBLIC B
		PUBLIC BISCOIT
		BISCOITO: EXTERN
		BATATA: EXTERN
		INPUT		B
		INPUT		H
Biscoit:	COPY H, DOIS
		IF	TRIANGULO
		LOAD		BISCOITO		
		JMP		TEST
		MULT		BATATA		
		DIV		DOIS		
		STORE		R
		OUTPUT	R
Banana:		STOP	
END


