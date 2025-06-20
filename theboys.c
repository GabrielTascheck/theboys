#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include "entidades.h"
#include "eventos.h"
#include "init.h"

/*

1 = chega
2 = espera
3 = desiste
4 = avisa
5 = entra
6 = sai
7 = viaja
8 = morre
9 = missao
0 = fim

*/

int main()
{
  srand(time(NULL));
  struct mundo_ent *w = init_mundo();
  if (!w)
    printf("DEBUG: ERRO EM CRIAR MUNDO");

  struct fprio_t *fEventos = fprio_cria();
  if (!fEventos)
    printf("\nDEBUG: ERRO EM CRIAR fEventos");

  // INICIANDO BASES
  for (int i = 0; i < N_BASES; i++)
  {
    struct base_ent *b = init_base(w);
    if (b == NULL)
      printf("\nDEBUG: ERRO EM CRIAR BASE");
  }

  // INICIANDO HEROIS
  for (int i = 0; i < N_HEROIS; i++)
  {
    int tempo = aleat(0, 5);
    int base = aleat(0, w->nBases - 1);

    struct heroi_ent *h = init_heroi(w);
    if (h == NULL)
      printf("\nDEBUG: ERRO EM CRIAR HEROI");

    struct evt_gen *evento = evento_cria();
    evento->h = h;
    evento->b = w->bases[base];
    if (!evento->b)
      printf("\nDEBUG: BASE NULA NA INIT");
    fprio_insere(fEventos, evento, E_CHEGA, tempo); // evento CHEGA = 1
    // printf("\nDEBUG: CRIA HEROI %d CHEGA NA BASE %d EM %d HABS [",h->id,evento->b->id,tempo);
    // cjto_imprime(h->hab);
  }

  // INICIANDO MISSOES
  for (int i = 0; i < N_MISSOES; i++)
  {
    int tempo = aleat(0, T_FIM_DO_MUNDO);
    struct missao_ent *m = init_missao(w);

    struct evt_gen *evento = evento_cria();
    evento->m = m;
    fprio_insere(fEventos, evento, E_MISSAO, tempo); // evento MISSAO = 9
    // printf("\n%6d: INSERE MISSÃO %d EM %d [", w->relogio, m->id, tempo);
    // cjto_imprime(m->habs);
  }

  if (fEventos->num == 0)
    printf("\n0 EVENTOS");

  struct evt_gen *fimMundo = evento_cria();
  fprio_insere(fEventos, fimMundo, 0, T_FIM_DO_MUNDO);

  // fprio_imprime(fEventos);
  // printf("\n");

  while (fEventos->num > 0)
  {
    w->eventos++;
    int tipoEvento;

    struct evt_gen *evento;
    evento = fprio_retira(fEventos, &tipoEvento, &w->relogio);
    // printf("\nEVENTO: %d TIPO: %d PRIO: %d", w->eventos, tipoEvento, w->relogio);
    if (evento->h)
      if (evento->h->status == 0)
        tipoEvento = -1;

    switch (tipoEvento)
    {
    case 0:
      fim(w->relogio, w);
      free(fimMundo);
      fprio_destroi(fEventos); // destroi a fila e os eventos depois do FIM
      mundo_destroi(w);
      return 0;
      break;

    case 1:
      chega(w->relogio, evento->h, evento->b, fEventos);
      break;

    case 2:
      espera(w->relogio, evento->h, evento->b, fEventos);
      break;

    case 3:
      desiste(w->relogio, evento->h, evento->b, w, fEventos);
      break;

    case 4:
      avisa(w->relogio, evento->b, fEventos);
      break;

    case 5:
      entra(w->relogio, evento->h, evento->b, fEventos);
      break;

    case 6:
      sai(w->relogio, evento->h, evento->b, w, fEventos);
      break;

    case 7:
      viaja(w->relogio, evento->h, evento->b, w, fEventos);
      break;

    case 8:
      morre(w->relogio, evento->h, evento->b, evento->m, fEventos);
      break;

    case 9:
      missao(w->relogio, evento->m, w, fEventos);
      break;

    case -1:
      w->eventos--; // Não contabiliza eventos de herois mortos
      break;

    default:
      printf("\nTIPO DE EVENTO NÃO DEFINIDO TIPO: %d", tipoEvento);
      break;
    }
    free(evento);
  }

  printf("\n");
  return 0;
}