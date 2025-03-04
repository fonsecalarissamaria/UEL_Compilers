# Definições de variáveis
FLEX = flex
BISON = bison
CC = gcc

PROGRAMA = sintatico
LEXICO = lexico.l
SINTATICO = sintatico.y
ZIPFILE = sintatico.zip

# Alvo principal
all: $(PROGRAMA)

# Compila o programa
$(PROGRAMA): $(LEXICO) $(SINTATICO)
	$(BISON) -d $(SINTATICO)
	$(FLEX) $(LEXICO)
	$(CC) -c *.c -I.
	$(CC) *.o -o $(PROGRAMA)

# Cria o arquivo ZIP
zip: 
	zip -r $(ZIPFILE) $(LEXICO) $(SINTATICO) Makefile

# Limpa os arquivos gerados
clean:
	rm -f *.yy.c *.tab.c *.tab.h *.o $(PROGRAMA) $(ZIPFILE)

# Define que o alvo 'all' é o padrão
.PHONY: all zip clean

