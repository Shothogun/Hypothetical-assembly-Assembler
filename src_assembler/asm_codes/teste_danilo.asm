	TRIANGULO: EQU 1


SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
A: B:	jmp		h
		jmpp		b
		jmpp		b
		jmpn		h
		jmpz		bisc		
		MULT		H
		IF TRIANGULO
		DIV		DOIS		
		STORE		R
		OUTPUT	R
		STOP




SECTION DATA
	B:		
SPACE
	H:
		SPACE
	R:		SPACE
	DOIS:	CONST		2
