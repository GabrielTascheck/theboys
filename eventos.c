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

static struct base_ent *encontra_base_para_missao(struct missao_ent *m, struct mundo_ent *w, struct base_ent *bP)
{
  // Fila de prioridade para ordenar as bases por distância da missão.
  // A prioridade é a distância; quanto menor, maior a prioridade.
  struct fprio_t *ordemBases = fprio_cria();
  for (int i = 0; i < w->nBases; i++)
  {
    int distBase = hypot(m->coordX - w->bases[i]->coordX, m->coordY - w->bases[i]->coordY);
    fprio_insere(ordemBases, w->bases[i], 0, distBase);
  }

  bP = ordemBases->prim->item;

  struct base_ent *base_encontrada = NULL;
  // Enquanto houver bases na fila para verificar
  while (fprio_tamanho(ordemBases) && base_encontrada == NULL)
  {
    struct base_ent *base_atual;
    int dist_atual;

    // Retira a base mais próxima (maior prioridade) da fila
    fprio_retira(ordemBases, &base_atual, &dist_atual);

    // Cria um conjunto para acumular as habilidades dos heróis na base atual
    struct cjto_t *habs_acumuladas = cjto_cria(w->nHabs);

    // Itera por todos os heróis do mundo para ver quem está na base atual
    for (int i = 0; i < w->nHerois; i++)
    {
      // Se o herói 'i' pertence ao conjunto de presentes da base atual
      if (cjto_pertence(base_atual->presentes, i))
      {
        // Une as habilidades do herói ao conjunto de habilidades acumuladas
        struct cjto_t *habs_acumuladas = cjto_uniao(habs_acumuladas, w->herois[i]->hab);
      }
    }

    // Verifica se as habilidades acumuladas na base são suficientes para a missão
    if (cjto_contem(habs_acumuladas, m->habs))
    {
      base_encontrada = base_atual;
    }

    cjto_destroi(habs_acumuladas);
  }

  // Libera a memória da fila de prioridade, pois não é mais necessária
  fprio_destroi(ordemBases);

  return base_encontrada;
}

static void encontra_herois_da_base(struct heroi_ent *vHerois[], struct base_ent *b, struct mundo_ent *w)
{
  int qtdH = b->presentes->num;
  int cont = 0;
  for (int i = 0; i < w->nHerois && cont < qtdH; i++)
  {
    if (cjto_pertence(b->presentes, i))
    {
      vHerois[cont] = w->herois[i];
      cont++;
    }
  }
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
  if (b->espera->num > b->filaMax)
    b->filaMax = b->espera->num;

  // fprio_insere AVISA(agora,B)
}

// O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
// para l´a
void desiste(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  int proxBase = aleat(0, m->nBases - 1);

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
  // fprio_insere SAI(agora+tpb, H, B)
}

// O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
// porteiro de B ´e avisado, pois uma vaga foi liberada:
void sai(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  cjto_retira(b->presentes, h->id);
  struct base_ent *d = m->bases[aleat(0, m->nBases - 1)];

  // fprio_insere VIAJA(agora, H, D)
  // fprio_insere AVISA(agora, B)
}

// O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a esta)
void viaja(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  double distancia = hypot(m->bases[h->base]->coordX - b->coordX, m->bases[h->base]->coordY - b->coordY);
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
void missao(int t, struct missao_ent *m, struct fprio_t *f, struct mundo_ent *w)
{
  m->tentativas++;

  struct base_ent *basePerto;
  struct base_ent *baseEncontrada = encontra_base_para_missao(m, w, basePerto);

  if (baseEncontrada)
  {
    struct heroi_ent *heroisBase[baseEncontrada->presentes->num];
    encontra_herois_da_base(heroisBase, baseEncontrada, w);
    m->cumprida = 1;
    for (int i = 0; i < baseEncontrada->presentes->num; i++)
    {
      heroisBase[i]->xp++;
    }
    baseEncontrada->missoes++;
  }
  else
  {
    if (w->nCompostosV && t % 2500 == 0)
    {
      struct heroi_ent *heroisBase[basePerto->presentes->num];
      encontra_herois_da_base(heroisBase, basePerto, w);
      int maisXP = 0;
      for (int i = 0; i < basePerto->presentes->num; i++)
      {
        if (heroisBase[i]->xp > heroisBase[maisXP]->xp)
          maisXP = i;
        heroisBase[i]->xp++;
      }
      heroisBase[maisXP]->xp--;
      w->nCompostosV--;
      m->cumprida = 1;
      basePerto->missoes++;
      // fprio_insere MORRE(agora, H mais experiente)
    }
    else
    {
      // fprio_insere MISSAO(t + 24*60, M)
    }
  }
}

static void imprime_habilidades_herois(struct heroi_ent *h, struct mundo_ent *m)
{
  printf("[ ");
  for (int i = 0; i < m->nHabs; i++)
  {
    if (cjto_pertence(h->hab, i))
      printf("%d ", i);
  }
  printf("]");
}
// Encerra a simula¸c˜ao no instante T:
// O evento FIM encerra a simula¸c˜ao, gerando um relat´orio com informa¸c˜oes sobre
// her´ois, bases e miss˜oes:
/*
HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ %d %d ... ]
HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ %d %d ... ]

BASE %2d LOT %2d FILA MAX %2d MISSOES %d
EVENTOS TRATADOS: %d
MISSOES CUMPRIDAS: %d/%d (%.1f%%)
TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f
TAXA MORTALIDADE: %.1f%%
*/
void fim(int t, struct mundo_ent *m)
{
  printf("%d: FIM", t);

  for (int i = 0; i < m->nHerois; i++)
  {
    struct heroi_ent *heroi = m->herois[i];
    if (heroi->status == 1)
    {
      printf("\nHEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS ", heroi->id, heroi->pac, heroi->vel, heroi->xp);
    }
    else
    {
      printf("\nHEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS ", heroi->id, heroi->pac, heroi->vel, heroi->xp);
    }
    imprime_habilidades_herois(heroi, m);
  }

  for (int i = 0; i < m->nBases; i++)
  {
    struct base_ent *base = m->bases[i];
    printf("\nBASE %2d LOT %2d FILA MAX %2d MISSOES %2d", base->id, base->lot, base->filaMax, base->missoes);
  }
  printf("\nEVENTOS TRATADOS: %d", m->eventos);

  int missoesCump = 0;
  int tentativasTotal = 0;
  for (int i = 0; i < m->nMissoes; i++)
  {
    if (m->missoes[i]->cumprida == 1)
      missoesCump++;
    tentativasTotal += m->missoes[i]->tentativas;
  }
  float pMissoes = m->nMissoes / 100.0 * missoesCump;
  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)", missoesCump, m->nMissoes, pMissoes);
}