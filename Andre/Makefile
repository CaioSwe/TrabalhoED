# Atribuir à variável EXEC o nome do seu programa principal (sem extensão)
# Atribuir à variável TAD o nome do seu TAD (sem extensão)

EXEC = P1

TADS = pilha.h lista.h inventario.h movimentacao.h mapa.h
OBJS = main.o pilha.o lista.o inventario.o movimentacao.o mapa.o

all: $(EXEC)

$(EXEC): $(OBJS)
	gcc $(OBJS) -o $(EXEC)

main.o: main.c $(TADS)
	gcc main.c $(TADS) -o main.o

pilha.o: pilha.c pilha.h
	gcc pilha.c -o pilha.o

lista.o: lista.c lista.h
	gcc lista.c -o lista.o

inventario.o: inventario.c inventario.h lista.o
	gcc inventario.c -o inventario.o

movimentacao.o: movimentacao.c movimentacao.h mapa.o inventario.o pilha.o lista.o
	gcc movimentacao.c -o movimentacao.o

mapa.o: mapa.c mapa.h
	gcc mapa.c -o mapa.o

clean:
	rm -f $(OBJS) $(EXECUTAVEL)