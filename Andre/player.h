typedef struct player Player;
typedef struct stats Stats;

Player* criarPlayer();

int     getPlayerHP   (Player*     );
void    setPlayerHP   (Player*, int);
int     getPlayerMaxHP(Player*     );

int     getPlayerGold(Player*);
void    setPlayerGold(Player*, int);