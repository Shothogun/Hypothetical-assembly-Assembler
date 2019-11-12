	dois: EQU 2
	tres: EQU 3
	quatro: 
EQU 4
	triangulo: EQU 0
SECTION DATA
	B:	SPACE				
	H:	SPACE		dois
	R:	SPACE 
	DOIS:	CONST		dois
	QUATRO: CONST		quatro
	TRES:	CONST		tres

SECTION TEXT
		INPUT		B
		INPUT		H
Biscoit:	COPY Q,B,C
		IF	TRIANGULO
		LOAD		B		
		MULT		H		

		DIV		D		t
		STORE		R
		OUTPUT	R
Banana:		STOP	A



