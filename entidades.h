#include "conjunto.h"
#include "fila.h"
#include "init.h"

#ifndef ENTIDADES
#define ENTIDADES

struct heroi_ent
{
  int id;
  struct cjto_t *hab;
  int pac;
  int vel;
  int xp;
  int base;
};

struct base_ent
{
  int id;
  int lot; // lotação
  struct cjto_t *presentes;
  struct fila_t *fila;
  int CoordX;
  int CoordY;
};

struct mundo_ent
{
  int nHerois;
  int herois[N_HEROIS];
  int numBases;
  int bases[N_BASES];
  int numMissoes;
  int missoes[N_MISSOES];
  int numHab;
  int nCompostosV;
  int tamanhoMundo;
  int relogio;
};

struct heroit_ent *init_heroi(struct heroi_ent *heroi);

struct base_ent *init_base(struct base_ent *base);

struct mundo_ent *init_mundo(struct mundo_t *mundo);

#endif