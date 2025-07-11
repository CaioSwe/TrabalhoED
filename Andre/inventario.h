typedef struct lista Lista;
typedef struct player Player;

void menuItem        (Lista*, Player*     );
int  menuInventario  (                    );
void coletarItem     (int, Lista*, Player*);
int  itemChoice      (                    );
void abrirInventario (Lista*, Player*     );
void usarItem        (Lista*, Player*     );
void descartarItem   (Lista*              );
void inspecionarItem (Lista*              );