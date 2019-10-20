	TRIANGULO: 
EQU 1


SECTION DATA
	B:		
SPACE	DADO			;argumento inválido		
	H:
		SPACE
	R:		
SPACE
	DOIS:	
JMP	BISCOITO		;instrução na seção errada

CONST		dois		;argumento inválido

CONSTA	1	;diretiva inválida

SECTION biscoito		;seção inválida

SECTION TEXT
		INPUT		B
		INPUT		H
		
		DIV	0 ;instrução c omparametros inválidos

		DIV    1 a	;quantidade de parametros inválida
		STOP	a	;quantidade de parametros inválida


		JMP		d	;saltos para label inválido
		JMPP		d
		JMPN		c
		JMPZ		a		
		
1BISC: 					;token inválido
	BISCO:				;dois labels na mesma linha
		STOP
		A:	const 0		;diretivas na seção errada
		C:	SPACE

		A: div		DOIS    ;label repetido

		ACC	BISCO    ;instrução inválida
				
		LOAD		B		
		MULT		H		
		IF TRIANGULO
		DIV		DOIS		
		STORE		R
		OUTPUT	R
		STOP



