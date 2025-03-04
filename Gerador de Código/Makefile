# Compiladores e ferramentas
FLEX = flex
BISON = bison
GCC = gcc

# Nome do programa final
PROGRAMA = gerador

# Arquivos principais
LEXICO = lexico.l
SINTATICO = sintatico.y
HEADERS = *.h
SOURCES = *.c

# Flags de compilação
CFLAGS = -Wall -g -Wno-unused-function -I.
LDFLAGS = -lm

# Regra principal
all: $(PROGRAMA)

# Regra para criar o programa
$(PROGRAMA): lex.yy.c sintatico.tab.c sintatico.tab.h $(SOURCES) $(HEADERS)
	$(GCC) -c $(SOURCES) lex.yy.c sintatico.tab.c $(CFLAGS)
	$(GCC) *.o -o $(PROGRAMA) $(LDFLAGS)

# Gerar o analisador léxico
lex.yy.c: $(LEXICO) sintatico.tab.h
	$(FLEX) -o lex.yy.c $(LEXICO)

# Gerar o analisador sintático
sintatico.tab.c sintatico.tab.h: $(SINTATICO)
	$(BISON) -d $(SINTATICO)

# Limpeza dos arquivos gerados
clean:
	rm -f lex.yy.c sintatico.tab.c sintatico.tab.h *.o $(PROGRAMA)
	echo "Arquivos limpos com sucesso."
