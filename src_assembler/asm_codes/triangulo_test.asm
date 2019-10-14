TRIANGULO: EQU abas
1BISCOITO: EQU 1
Ab:
EQU 1*72$#
biscoit: bisc: EQU 1
bicoi:	
bisca: EQU 3
SECTION TEXT
		INPUT		B   ; isso aqui é um triangulo
	; isso aqui é um triangulo
		INPUT		H
		LOAD		B
		MULT		H
		IF biscoito
		COPY		B  	,						H
		IF triangulo
		DIV		DOIS +						2
		IF 13#$!@			
		STORE		R
		OUTPUT	R
		STOP




			    



		
SECTION DATA
	b:		sPace
	H:		SPACE 		2
	R:		SPACE
	dois:	CONST		2