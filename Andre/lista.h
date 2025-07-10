typedef struct lista Lista;
typedef struct cel Celula;

Lista*  criaLista     (                );
Celula* buscarElemento(Lista*,      int);
int     inserirInicio (Lista*,      int);
int     inserirMeio   (Lista*, int, int);
int     inserirFim    (Lista*,      int);
int     removerInicio (Lista*          );
int     removerMeio   (Lista*,      int);
int     removerFim    (Lista*          );
void    imprimirLista (Lista*          );
int     getValor      (Celula*         );