#include "entidades.h"


struct evento{
  int tempo;
  struct heroi_ent *heroi;
  struct base_ent *base;
};

// Representa um her´oi H chegando em uma base B no instante T. Ao chegar, o
// her´oi analisa o tamanho da fila e decide se espera para entrar ou desiste:
struct evento *chega(int t, struct heroi_ent *h, struct base_ent *b);

// O her´oi H entra na fila de espera da base B. Assim que H entrar na fila, o
// porteiro da base B deve ser avisado para verificar a fila:
struct evento *espera(int t, struct heroi_ent *h, struct base_ent *b);

// O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
// para l´a
struct evento *desiste(int t, struct heroi_ent *h, struct base_ent *b);

// O porteiro da base B trata a fila de espera:
struct evento *avisa(int t, struct base_ent *b);

// O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
// agenda sua sa´ıda da base
struct evento *entra(int t, struct heroi_ent *h, struct base_ent *b);

// O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
// porteiro de B ´e avisado, pois uma vaga foi liberada:
struct evento *sai(int t, struct heroi_ent *h, struct base_ent *b);

// O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a esta)
struct evento *viaja(int t, struct heroi_ent *h, struct base_ent *b);

// O her´oi H morre no instante T.
struct evento *morre(int t, struct heroi_ent *h, struct base_ent *b);

struct evento *missao(int t, struct missao_ent *m);

// Encerra a simula¸c˜ao no instante T:
struct evento *fim(int t);
