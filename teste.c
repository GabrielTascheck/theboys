#include <stdio.h>
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
  struct mundo_ent *w = init_mundo();
  if (!w)
    printf("ERRO EM CRIAR MUNDO");

  printf("\nnH: %d nB: %d nM: %d", w->nHerois, w->nBases, w->nMissoes);

  struct fprio_t *fEventos = fprio_cria();
  if (!fEventos)
    printf("\nERRO");

  for (int i = 0; i < N_BASES; i++)
  {
    init_base(w);
    if (w->bases[i] == NULL)
      printf("\nERRO EM CRIAR BASE");
  }
  for (int i = 0; i < N_HEROIS; i++)
  {
    int tempo = aleat(0, 5);
    int base = aleat(0, w->nBases);
    struct heroi_ent *h = init_heroi(w);
    if (h == NULL)
      printf("\nERRO EM CRIAR HEROI");

    printf("\nID: %d VIVO: %d", h->id, h->status);

    struct evt_gen *evento;
    evento->h = h;
    evento->b = w->bases[h->id];
    fprio_insere(fEventos, evento, 1, tempo); // evento CHEGA = 1
  }
  for (int i = 0; i < w->nHerois; i++)
  {
    int tempo = aleat(0, T_FIM_DO_MUNDO);
    struct missao_ent *m = init_missao(w);
    fprio_insere(fEventos, m, 9, tempo); // evento MISSAO = 9
  }

  if (fEventos->num == 0)
    printf("\nERRO2");

  printf("\nnH: %d nB: %d nM: %d", w->nHerois, w->nBases, w->nMissoes);
  printf("\n");
  fprio_imprime(fEventos);
  printf("\n");

  while (fEventos->num > 0)
  {
    int tipoEvento;
    struct evt_gen *evento;
    evento = fprio_retira(fEventos, &tipoEvento, &w->relogio);

    switch (tipoEvento)
    {
    case 0:
      fim(w->relogio, w);
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

    default:
      printf("TIPO DE EVENTO NÃO DEFINIDO TIPO: %d", tipoEvento);
      break;
    }
  }

  printf("\n");
  return 0;
}