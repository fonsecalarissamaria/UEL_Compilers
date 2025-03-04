# Definições
GCC = gcc
PROGRAMA = linearscan
CFLAGS = -I.  # Diretório de includes

# Fontes e Objetos
C_SOURCE = $(wildcard *.c)
OBJ = $(C_SOURCE:.c=.o)

# Targets
$(PROGRAMA): $(OBJ)
	$(GCC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos de compilação
clean:
	rm -i $(PROGRAMA) $(OBJ)
	@echo "Done cleaning."

# Compactar os arquivos necessários
zip:
	rm -f linearscan.zip
	zip linearscan.zip $(C_SOURCE) *.h Makefile
