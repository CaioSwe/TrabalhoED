typedef struct lista Lista;
typedef struct player Player;

void menuItem(Lista*);
int  menuInventario();
void coletarItem(int, Lista*);
int  itemChoice();
void abrirInventario(Lista*, Player*);
void usarItem();
void descartarItem();
void inspecionarItem(int);