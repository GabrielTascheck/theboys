#include <stdio.h>
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

  struct fprio_t *eventos = fprio_cria();

  for (int i = 0; i < w->nBases; i++)
  {
    init_base(w);
  }
  for (int i = 0; i < w->nHerois; i++)
  {
    int tempo = aleat(0, 5);
    int base = aleat(0, w->nBases);
    struct heroi_ent *h = init_heroi(w);
    fprio_insere(eventos, h, 1, tempo);
  }
  for (int i = 0; i < w->nHerois; i++)
  {
    int tempo = aleat(0, T_FIM_DO_MUNDO);
    struct missao_ent *m = init_missao(w);
    fprio_insere(eventos, m, 9, tempo);
  }

  return 0;
}