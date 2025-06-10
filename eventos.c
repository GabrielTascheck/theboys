#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "conjunto.h"
#include "fila.h"
#include "math.h"

static int aleat(int min, int max)
{
  return min + rand() % (max - min + 1);
}

// Representa um her´oi H chegando em uma base B no instante T. Ao chegar, o
// her´oi analisa o tamanho da fila e decide se espera para entrar ou desiste:
void chega(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f)
{
  if (!h || !b || !f)
  {
    printf("Ponteiro Nulo");
    return;
  }

  if (b->lot > b->presentes->num && b->espera->num == 0) // se h´a vagas em B e a fila de espera em B est´a vazia:
  {
    // fprio_insere ESPERA(agora,H,B)
  }
  else if (h->pac > (10 * b->espera->num)) // espera = (paci^encia de H) > (10 * tamanho da fila em B)
  {
    // fprio_insere ESPERA(agora,H,B)
  }
  // fprio_insere DESISTE(agora,H,B)
}

// O her´oi H entra na fila de espera da base B. Assim que H entrar na fila, o
// porteiro da base B deve ser avisado para verificar a fila:
void espera(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f)
{
  if (!h || !b || !f)
  {
    printf("Ponteiro Nulo");
    return;
  }

  if (fila_insere(b->espera, h) == -1)
  {
    printf("Erro ao inserir na fila de espera");
    return;
  }

  // fprio_insere AVISA(agora,B)
}

// O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
// para l´a
void desiste(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  int proxBase = aleat(0, m->nBases-1);

  struct base_ent *d = m->bases[proxBase];

  // fprio_insere VIAJA(agora, H, D)
}

// O porteiro da base B trata a fila de espera:
void avisa(int t, struct base_ent *b, struct fprio_t *f)
{
  while (b->espera->num < b->lot && b->espera->num > 0) // enquanto houver vaga em B e houver her´ois esperando na fila
  {
    struct heroi_ent *heroi = fila_retira(b->espera);
    cjto_insere(b->presentes, heroi->id);
    // fprio_insere ENTRA(agora, H', B)
  }
}

// O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
// agenda sua sa´ıda da base
void entra(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f)
{
  int tpb = 15 + h->pac * aleat(1, 20);

  //fprio_insere SAI(agora+tpb, H, B)
}

// O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
// porteiro de B ´e avisado, pois uma vaga foi liberada:
void sai(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  cjto_retira(b->presentes, h->id);
  struct base_ent *d = m->bases[aleat(0, m->nBases-1)];

  // fprio_insere VIAJA(agora, H, D)
  // fprio_insere AVISA(agora, B)
}

// O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a esta)
void viaja(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  int distancia = hypot(m->bases[h->base]->coordX - b->coordX, m->bases[h->base]->coordY - b->coordY);
  int duracao = distancia / h->vel;

  // fprio_insere CHEGA(agora + duração, H, D)
}

// O her´oi H morre no instante T.
void morre(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f)
{
  cjto_retira(b->presentes, h->id);
  h->status = 0;
  // fprio_insere AVISA (agora , B)
}

// Uma miss˜ao M ´e disparada no instante T
void missao(int t, struct missao_ent *m, struct fprio_t *f);

// Encerra a simula¸c˜ao no instante T:
void fim(int t);