# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -std=c11

# Nome do executável
TARGET = portugol

# Arquivos de origem
SRCS = lexico.c sintatico.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Regras de compilação
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -f $(TARGET) $(OBJS)

# Cria o arquivo ZIP contendo main.c e Makefile
zip: clean
	zip portugol.zip lexico.h lexico.c sintatico.c Makefile
