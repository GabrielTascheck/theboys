#include "entidades.h"
#include "fprio.h"

#ifndef EVENTOS
#define EVENTOS

struct evt_gen { //evento generico
  struct heroi_ent *h; 
  struct base_ent *b;
  struct missao_ent *m;
};


int aleat(int min, int max);

// Representa um her´oi H chegando em uma base B no instante T. Ao chegar, o
// her´oi analisa o tamanho da fila e decide se espera para entrar ou desiste:
void chega(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f);

// O her´oi H entra na fila de espera da base B. Assim que H entrar na fila, o
// porteiro da base B deve ser avisado para verificar a fila:
void espera(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f);

// O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
// para l´a
void desiste(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m,struct fprio_t *f);

// O porteiro da base B trata a fila de espera:
void avisa(int t, struct base_ent *b, struct fprio_t *f);

// O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
// agenda sua sa´ıda da base
void entra(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f);

// O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
// porteiro de B ´e avisado, pois uma vaga foi liberada:
void sai(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f);

// O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a esta)
void viaja(int t, struct heroi_ent *h, struct base_ent *d, struct mundo_ent *m,struct fprio_t *f);

// O her´oi H morre no instante T.
void morre(int t, struct heroi_ent *h, struct base_ent *b, struct missao_ent *m,struct fprio_t *f);

// Uma miss˜ao M ´e disparada no instante T
void missao(int t, struct missao_ent *m, struct mundo_ent *w,struct fprio_t *f);

// Encerra a simula¸c˜ao no instante T:
void fim(int t, struct mundo_ent *m);

#endif