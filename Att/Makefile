PROJ_NAME = ted

ALUNO = waldemarsn
LIBS =-lm
OBJETOS = main.o inventario.o lista.o mapa.o movimentacao.o pilha.o player.o

# Compilador
CC = gcc

# Flags
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS = -O0

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Empacotar para entrega
#
# - se necessario: sudo apt install zip
pack: $(PROJ_NAME)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-ME.txt .entrega

# Regra para executar com Valgrind
valgrind: $(PROJ_NAME)
	valgrind --leak-check=full ./$(PROJ_NAME)

# Limpar arquivos
clean:
	rm -f *.o $(PROJ_NAME)