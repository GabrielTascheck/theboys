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
  int status; //vivo = 1 -- morto = 0
};

struct base_ent
{
  int id;
  int lot; // lotação
  struct cjto_t *presentes;
  struct fila_t *espera;
  int coordX;
  int coordY; 
  int filaMax; //Maior fila
  int missoes; // qntd de missoes feitas
};

struct missao_ent
{
  int id;
  struct cjto_t *habs;
  int coordX;
  int coordY;
  int cumprida;
  int tentativas;
};

struct mundo_ent
{
  int nHerois;
  struct heroi_ent *herois[N_HEROIS];
  int nBases;
  struct base_ent *bases[N_BASES];
  int nMissoes;
  struct missao_ent *missoes[N_MISSOES];
  int nHabs;
  int nCompostosV;
  int tamanhoMundo;
  int relogio;
  int eventos;
};


// id = n´umero sequencial [0...N_HEROIS-1]
// experi^encia = 0
// paci^encia = n´umero aleat´orio [0...100]
// velocidade = n´umero aleat´orio [50...5000] // em metros/minuto = 3 Km/h a 300 Km/h
// habilidades = conjunto com tamanho aleat´orio [1...3] de habilidades aleat´orias
struct heroi_ent *init_heroi(struct mundo_ent *mundo);

// id = n´umero sequencial [0...N_BASES-1]
// local = par de n´umeros aleat´orios [0...N_TAMANHO_MUNDO-1]
// lota¸c~ao = n´umero aleat´orio [3...10]
// presentes = conjunto vazio (com capacidade para armazenar IDs
// de her´ois at´e a lota¸c~ao da base)
// espera = fila vazia
struct base_ent *init_base(struct mundo_ent *mundo);


// id = n´umero sequencial [0...N_MISSOES-1]
// local = par de n´umeros aleat´orios [0...N_TAMANHO_MUNDO-1]
// habilidades = conjunto com tamanho aleat´orio [6...10] de habilidades aleat´orias
struct missao_ent *init_missao(struct mundo_ent *mundo);

struct mundo_ent *init_mundo();

#endif