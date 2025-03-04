# Variáveis
CC = gcc
TARGET = regalloc

SRC = $(wildcard *.c)
OBJ_FILES = $(SRC:.c=.o)

# Regra principal
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ -I.

# Compilação dos arquivos .c para .o
$(OBJ_FILES): %.o: %.c
	$(CC) -c $< -o $@ -I.

# Limpeza dos arquivos gerados
clean:
	rm -i $(TARGET) $(OBJ_FILES)

# Compactação dos arquivos fontes
zip:
	rm -f regalloc.zip
	zip regalloc.zip $(SRC) $(wildcard *.h) Makefile
