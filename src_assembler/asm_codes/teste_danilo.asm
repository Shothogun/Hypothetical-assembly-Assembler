BISCOITO:
	TRIANGULO: 
EQU 1


SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
A: B:	jmp		h
		jmpp		b
		jmpp		b
Amante: Biscoito:		jmpn		h
		jmpz		bisc		
		MULT		H
		IF TRIANGULO
		DIV		DOIS
		div		dois
		div 		dois			
biscoito: 
		copy   H   ,    R
		STORE		R
		OUTPUT	R
		STOP




SECTION DATA
	B:		
SPACE
	H:
		SPACE
	R:		SPACE
	DOIS:	CONST		0
