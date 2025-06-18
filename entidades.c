#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "entidades.h"
#include "init.h"
#include "conjunto.h"


static int aleat(int min, int max)
{
  return min + rand() % (max - min + 1);
}

struct heroi_ent *init_heroi(struct mundo_ent *mundo)
{
  if (mundo == NULL)
    return NULL;

  struct heroi_ent *heroi = malloc(sizeof(struct heroi_ent));

  if (heroi == NULL)
    return NULL;


  heroi->id = mundo->nHerois;
  heroi->xp = 0;
  heroi->pac = aleat(0, 100);
  heroi->vel = aleat(50, 5000);
  heroi->hab = cjto_aleat(mundo->nHabs - 1, aleat(1, 3));
  heroi->status = 1;
  if(heroi->hab == NULL)
    return NULL;

  mundo->herois[mundo->nHerois] = heroi;
  mundo->nHerois++;

  return heroi;
}

struct base_ent *init_base(struct mundo_ent *mundo)
{
  if (mundo == NULL)
    return NULL;

  struct base_ent *base = malloc(sizeof(struct base_ent));

  if (base == NULL)
    return NULL;

  base->id = mundo->nBases;
  base->lot = aleat(3, 10);
  base->presentes = cjto_cria(base->lot);
  if(base->presentes == NULL)
    return NULL;

  base->espera = fila_cria();
  if(base->espera == NULL)
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
  if(mundo == NULL)
    return NULL;
  
  struct missao_ent *missao = malloc(sizeof(struct missao_ent));

  if(missao == NULL)
    return NULL;
  
  missao->id = mundo->nMissoes;
  missao->habs = cjto_aleat(mundo->nHabs, aleat(6,10));
  if(missao->habs == NULL)
    return NULL;
    
  missao->coordX  = aleat(0, mundo->tamanhoMundo);
  missao->coordY  = aleat(0, mundo->tamanhoMundo);
  missao->cumprida = 0;
  missao->tentativas = 0;

  mundo->missoes[missao->id] = missao;
  mundo->nMissoes++;

  return missao;
}

struct mundo_ent *init_mundo()
{
  struct mundo_ent *mundo = malloc(sizeof(struct mundo_ent));

  if(mundo == NULL)
    return NULL;

  mundo->nHerois = 0;
  mundo->nBases = 0;
  mundo->nHabs = N_HABILIDADES;
  mundo->nCompostosV = N_COMPOSTOS_V;
  mundo->tamanhoMundo = N_TAMANHO_MUNDO;
  mundo->relogio = 0;

  return mundo;
}
