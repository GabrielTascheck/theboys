#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "init.h"
#include "conjunto.h"
#include "eventos.h"

struct heroi_ent *init_heroi(struct mundo_ent *mundo)
{
  if (mundo == NULL)
    return NULL;
  if (mundo->nHerois >= N_HEROIS)
  {
    fprintf(stderr, "ERRO: Capacidade máxima de heróis (%d) atingida. Não é possível criar mais.\n", N_HEROIS);
    return NULL;
  }

  struct heroi_ent *heroi = malloc(sizeof(struct heroi_ent));

  if (heroi == NULL)
    return NULL;

  heroi->base = -1;
  heroi->id = mundo->nHerois;
  heroi->xp = 0;
  heroi->pac = aleat(0, 100);
  heroi->vel = aleat(50, 5000);
  heroi->status = 1;
  heroi->hab = cjto_aleat(aleat(1, 3), mundo->nHabs);
  if (heroi->hab == NULL)
    return NULL;

  mundo->herois[mundo->nHerois] = heroi;
  mundo->nHerois++;
  
  return heroi;
}

struct base_ent *init_base(struct mundo_ent *mundo)
{
  if (mundo == NULL)
    return NULL;
  if (mundo->nBases >= N_BASES)
  {
    fprintf(stderr, "ERRO: Capacidade máxima de bases (%d) atingida. Não é possível criar mais.\n", N_BASES);
    return NULL;
  }

  struct base_ent *base = malloc(sizeof(struct base_ent));

  if (base == NULL)
    return NULL;

  base->id = mundo->nBases;
  base->lot = aleat(3, 10);
  base->presentes = cjto_cria(N_HEROIS);
  if (base->presentes == NULL)
    return NULL;

  base->espera = fila_cria();
  if (base->espera == NULL)
    return NULL;

  base->coordX = aleat(0, mundo->tamanhoMundo);
  base->coordY = aleat(0, mundo->tamanhoMundo);

  base->filaMax = 0;
  base->missoes = 0;

  mundo->bases[base->id] = base;
  mundo->nBases++;

  return base;
}

struct missao_ent *init_missao(struct mundo_ent *mundo)
{
  if (mundo == NULL)
    return NULL;
  if (mundo->nMissoes >= N_MISSOES)
  {
    fprintf(stderr, "ERRO: Capacidade máxima de missões (%d) atingida. Não é possível criar mais.\n", N_MISSOES);
    return NULL;
  }
  struct missao_ent *missao = malloc(sizeof(struct missao_ent));

  if (missao == NULL)
    return NULL;

  missao->id = mundo->nMissoes;
  missao->habs = cjto_aleat(aleat(6, 10), mundo->nHabs);
  if (missao->habs == NULL)
    return NULL;

  missao->coordX = aleat(0, mundo->tamanhoMundo);
  missao->coordY = aleat(0, mundo->tamanhoMundo);
  missao->cumprida = 0;
  missao->tentativas = 0;

  mundo->missoes[missao->id] = missao;
  mundo->nMissoes++;

  return missao;
}

struct mundo_ent *init_mundo()
{
  struct mundo_ent *mundo = calloc(1, sizeof(struct mundo_ent)); // iniciar vetores também

  if (mundo == NULL)
    return NULL;

  // mundo->nHerois = 0;
  // mundo->nBases = 0;
  // mundo->nMissoes = 0;
  mundo->nHabs = N_HABILIDADES;
  mundo->nCompostosV = N_COMPOSTOS_V;
  mundo->tamanhoMundo = N_TAMANHO_MUNDO;
  mundo->relogio = T_INICIO;
  // mundo->eventos = 0;

  return mundo;
}

struct mundo_ent *mundo_destroi(struct mundo_ent *w)
{
  if (w == NULL) // Boa prática: sempre verifique se o ponteiro é nulo
    return NULL;

  // Libera todos os heróis alocados dinamicamente
  for (int i = 0; i < w->nHerois; i++)
  {
    if (w->herois[i])
    {
      struct heroi_ent *h = w->herois[i];
      cjto_destroi(h->hab);
      free(h);
    }
  }

  // Libera todas as bases alocadas dinamicamente
  for (int i = 0; i < w->nBases; i++)
  {
    if (w->bases[i])
    {
      struct base_ent *b = w->bases[i];
      cjto_destroi(b->presentes);
      fila_destroi(b->espera);
      free(b);
    }
  }

  // Libera todas as missões alocadas dinamicamente
  for (int i = 0; i < w->nMissoes; i++)
  {
    if (w->missoes[i])
    {
      struct missao_ent *m = w->missoes[i];
      cjto_destroi(m->habs);
      free(m);
    }
  }

  // Libera a estrutura mundo
  free(w);

  return NULL;
}
