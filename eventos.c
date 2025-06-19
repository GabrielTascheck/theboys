#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "conjunto.h"
#include "fila.h"
#include "math.h"

int aleat(int min, int max)
{
  return min + rand() % (max - min + 1);
}

// Imprime conjunto conj que pode conter itens de 0..n
static void imprime_conjunto(struct cjto_t *conj, int n)
{
  printf("[ ");
  for (int i = 0; i < n; i++)
  {
    if (cjto_pertence(conj, i))
      printf("%d ", i);
  }
  printf("]");
}

int encontra_base_para_missao(struct missao_ent *m, struct mundo_ent *w, struct base_ent *b, struct cjto_t *habs)
{

  int t = w->relogio;
  // Fila de prioridade para ordenar as bases por distância da missão.
  // A prioridade é a distância; quanto menor, maior a prioridade.
  struct fprio_t *ordemBases = fprio_cria();
  for (int i = 0; i < w->nBases; i++)
  {
    int distBase = hypot(m->coordX - w->bases[i]->coordX, m->coordY - w->bases[i]->coordY);
    fprio_insere(ordemBases, w->bases[i], 0, distBase);
  }

  struct base_ent *basePerto = ordemBases->prim->item;

  struct base_ent *baseEncontrada = NULL;
  // Enquanto houver bases na fila para verificar
  while (fprio_tamanho(ordemBases) && baseEncontrada == NULL)
  {
    struct base_ent *base_atual;
    int dist_atual;

    // Retira a base mais próxima (maior prioridade) da fila
    base_atual = fprio_retira(ordemBases, NULL, &dist_atual);

    printf("\n%6d: MISSAO %d BASE %d DIST %d HEROIS ", t, m->id, base_atual->id, dist_atual);
    imprime_conjunto(base_atual->presentes, w->nHerois);

    // Cria um conjunto para acumular as habilidades dos heróis na base atual
    struct cjto_t *habs_acumuladas = cjto_cria(w->nHabs);

    // Itera por todos os heróis do mundo para ver quem está na base atual
    for (int i = 0; i < w->nHerois; i++)
    {
      // Se o herói 'i' pertence ao conjunto de presentes da base atual
      if (cjto_pertence(base_atual->presentes, i))
      {
        printf("\n%6d: MISSAO %d HAB HEROI %2d: ", t, m->id, i);
        imprime_conjunto(w->herois[i]->hab, w->nHabs);
        // Une as habilidades do herói ao conjunto de habilidades acumuladas
        struct cjto_t *habs_acumuladas = cjto_uniao(habs_acumuladas, w->herois[i]->hab);
      }
    }

    // Verifica se as habilidades acumuladas na base são suficientes para a missão
    if (cjto_contem(habs_acumuladas, m->habs))
    {
      habs = cjto_copia(habs_acumuladas);
      baseEncontrada = base_atual;
    }

    printf("\n%6d: MISSAO %d UNIAO HAB BASE %d: ", t, m->id, base_atual->id);
    imprime_conjunto(habs_acumuladas, w->nHabs);
    cjto_destroi(habs_acumuladas);
  }

  // Libera a memória da fila de prioridade
  fprio_destroi(ordemBases);
  if (baseEncontrada)
  {
    b = baseEncontrada;
    return 1;
  }
  b = basePerto;
  return 0;
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
    printf("\n%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA", t, h->id, b->id, b->presentes->num, b->lot);
    // fprio_insere ESPERA(agora,H,B)
  }
  else if (h->pac > (10 * b->espera->num)) // espera = (paci^encia de H) > (10 * tamanho da fila em B)
  {
    printf("\n%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA", t, h->id, b->id, b->presentes->num, b->lot);
    // fprio_insere ESPERA(agora,H,B)
  }
  printf("\n%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE", t, h->id, b->id, b->presentes->num, b->lot);
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
  if (b->espera->num > b->filaMax) // se fila estiver maior que a maior fila registrada
    b->filaMax = b->espera->num;

  printf("\n%6d: ESPERA HEROI %2d BASE %d (%2d)", t, h->id, b->id, b->espera->num);
  // fprio_insere AVISA(agora,B)
}

// O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
// para l´a
void desiste(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  int proxBase = aleat(0, m->nBases - 1);

  struct base_ent *d = m->bases[proxBase];

  printf("\n%6d: DESIST HEROI %2d BASE %d", t, h->id, b->id);
  // fprio_insere VIAJA(agora, H, D)
}

// O porteiro da base B trata a fila de espera:
void avisa(int t, struct base_ent *b, struct fprio_t *f)
{
  printf("\n%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id, b->presentes->num, b->lot);
  fila_imprime(b->espera);
  printf(" ]");
  while (b->espera->num < b->lot && b->espera->num > 0) // enquanto houver vaga em B e houver her´ois esperando na fila
  {
    struct heroi_ent *heroi = fila_retira(b->espera);

    printf("\n%6d: AVISA PORTEIRO BASE %d ADMITE %2d", t, b->id, heroi->id);
    cjto_insere(b->presentes, heroi->id);

    // fprio_insere ENTRA(agora, H', B)
  }
}

// O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
// agenda sua sa´ıda da base
void entra(int t, struct heroi_ent *h, struct base_ent *b, struct fprio_t *f)
{
  int tpb = 15 + h->pac * aleat(1, 20);
  h->base = b->id;
  printf("\n%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d", t, h->id, b->id, b->presentes->num, b->lot, tpb);
  // fprio_insere SAI(agora+tpb, H, B)
}

// O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
// porteiro de B ´e avisado, pois uma vaga foi liberada:
void sai(int t, struct heroi_ent *h, struct base_ent *b, struct mundo_ent *m, struct fprio_t *f)
{
  cjto_retira(b->presentes, h->id);
  printf("\n%6d: SAI HEROI %2d BASE %d (%2d/%2d)", t, h->id, b->id, b->presentes->num, b->lot);

  struct base_ent *d = m->bases[aleat(0, m->nBases - 1)];
  // fprio_insere VIAJA(agora, H, D)
  // fprio_insere AVISA(agora, B)
}

// O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a esta)
void viaja(int t, struct heroi_ent *h, struct base_ent *b, struct base_ent *d, struct mundo_ent *m, struct fprio_t *f)
{
  int distancia = hypot(b->coordX - d->coordX, b->coordY - d->coordY);
  int duracao = distancia / h->vel;
  printf("\n%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d", t, h->id, b->id, d->id, distancia, h->vel, t + duracao);
  // fprio_insere CHEGA(agora + duração, H, D)
}

// O her´oi H morre no instante T.
void morre(int t, struct heroi_ent *h, struct base_ent *b, struct missao_ent *m, struct fprio_t *f)
{
  cjto_retira(b->presentes, h->id);
  h->status = 0;
  h->base = -1;
  printf("\n%6d: MORRE HEROI %2d MISSAO %d", t, h->id, m->id);
  // fprio_insere AVISA (agora , B)
}

// Uma miss˜ao M ´e disparada no instante T
void missao(int t, struct missao_ent *m, struct fprio_t *f, struct mundo_ent *w)
{
  m->tentativas++;

  printf("\n%6d: MISSAO %d TENT %d HAB REQ: ", t, m->id, m->tentativas);
  imprime_conjunto(m->habs, w->nHabs);

  struct base_ent *baseEncontrada;
  struct cjto_t *habsBaseEncontrada;

  if (encontra_base_para_missao(m, w, baseEncontrada, habsBaseEncontrada))
  {
    struct heroi_ent *heroisBase[baseEncontrada->presentes->num];
    encontra_herois_da_base(heroisBase, baseEncontrada, w);
    m->cumprida = 1;
    for (int i = 0; i < baseEncontrada->presentes->num; i++)
    {
      heroisBase[i]->xp++;
    }
    baseEncontrada->missoes++;
    printf("\n%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", t, m->id, baseEncontrada->id);
    imprime_conjunto(baseEncontrada->presentes, baseEncontrada->presentes->num);
  }
  else
  {
    if (w->nCompostosV && t % 2500 == 0)
    {
      struct heroi_ent *heroisBase[baseEncontrada->presentes->num];
      encontra_herois_da_base(heroisBase, baseEncontrada, w);
      int maisXP = 0;
      for (int i = 0; i < baseEncontrada->presentes->num; i++)
      {
        if (heroisBase[i]->xp > heroisBase[maisXP]->xp)
          maisXP = i;
        heroisBase[i]->xp++;
      }
      heroisBase[maisXP]->xp--;
      w->nCompostosV--;
      m->cumprida = 1;
      baseEncontrada->missoes++;
      printf("\n%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", t, m->id, baseEncontrada->id);
      imprime_conjunto(habsBaseEncontrada, w->nHabs);
      // fprio_insere MORRE(agora, H mais experiente)
    }
    else
    {
      printf("\n%6d: MISSAO %d IMPOSSIVEL", t, m->id);
      // fprio_insere MISSAO(t + 24*60, M)
    }
  }
}

// Encerra a simula¸c˜ao no instante T:
// O evento FIM encerra a simula¸c˜ao, gerando um relat´orio com informa¸c˜oes sobre
// her´ois, bases e miss˜oes:
void fim(int t, struct mundo_ent *m)
{
  int hMortos = 0;
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
      hMortos++;
    }
    imprime_conjunto(heroi->hab, m->nHabs);
  }

  for (int i = 0; i < m->nBases; i++)
  {
    struct base_ent *base = m->bases[i];
    printf("\nBASE %2d LOT %2d FILA MAX %2d MISSOES %2d", base->id, base->lot, base->filaMax, base->missoes);
  }
  printf("\nEVENTOS TRATADOS: %d", m->eventos);

  int missoesCump = 0;
  int tentativasTotal = 0;
  int minTentativas = -1; 
  int maxTentativas = -1; 
  float mediaTentativas = 0;

  for (int i = 0; i < m->nMissoes; i++)
  {
    struct missao_ent *missao = m->missoes[i];

    if (missao->cumprida == 1)
      missoesCump++;

    tentativasTotal += missao->tentativas;

    if (minTentativas == -1 || missao->tentativas < minTentativas)
      minTentativas = missao->tentativas;

    if (maxTentativas == -1 || missao->tentativas > maxTentativas)
      maxTentativas = missao->tentativas;
  }

  float pMissoes;
  if (m->nMissoes > 0)
    pMissoes = (missoesCump / (float)m->nMissoes) * 100.0;
  else
    pMissoes = 0;
  printf("\nMISSOES CUMPRIDAS: %d/%d (%.1f%%)", missoesCump, m->nMissoes, pMissoes);

  if (m->nMissoes > 0)
    mediaTentativas = (float)tentativasTotal / m->nMissoes;
  else
    mediaTentativas = 0;

  printf("\nTENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f", minTentativas, maxTentativas, mediaTentativas);

  float taxaM = (float)hMortos / m->nHerois * 100;
  printf("TAXA MORTALIDADE: %.1f%%");
}
