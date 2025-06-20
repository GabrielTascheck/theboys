#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fprio.h"

static int verifica_repetido(struct fprio_t *f, void *item) {

    if(!f->num)
      return 0;

    struct fpnodo_t *aux = f->prim;
    while (aux != NULL) { 
        if (aux->item == item)
            return 1;      
        aux = aux->prox;
    }
    return 0; // Não encontrou
}

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria ()
{
  struct fprio_t *fila = malloc(sizeof(struct fprio_t));

  if (!fila)
    return NULL;

  memset(fila, 0, sizeof(struct fprio_t));

  return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f)
{
  if (!f)
    return NULL;

  struct fpnodo_t *aux;
 
  while(f->prim != NULL)
  {
    aux = f->prim->prox;
    free(f->prim->item);
    free(f->prim);
    f->prim = aux;
  }

  free(f);
  return NULL;
}

struct fprio_t *fprio_destroi_keepItems (struct fprio_t *f)
{
  if (!f)
    return NULL;

  struct fpnodo_t *aux;
 
  while(f->prim != NULL)
  {
    aux = f->prim->prox;
    free(f->prim);
    f->prim = aux;
  }

  free(f);
  return NULL;
}


// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
  if (!f || !item)
    return -1;
  if (verifica_repetido(f, item))
      return -2;

  struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t));

  novo->prio = prio;
  novo->tipo = tipo;
  novo->item = item;
  novo->prox = NULL;

  if (f->num == 0)
  {
    f->prim = novo;
    f->fim = novo;
  }
  else
  {
    if (prio < f->prim->prio)
    {
      novo->prox = f->prim;
      f->prim = novo;
    }
    else if (prio >= f->fim->prio)
    {
      f->fim->prox = novo;
      f->fim = novo;
    }
    else
    {
      struct fpnodo_t *aux, *aux2;
      aux = f->prim;
      while (prio >= aux->prio && aux)
      {
        aux2 = aux;
        aux = aux->prox;
      }
      aux2->prox = novo;
      novo->prox = aux;
    }
  }

  f->num++;
  return f->num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
  if (!f || !f->num || !f->prim || !tipo || !prio)
    return NULL;

  struct fpnodo_t *aux = f->prim;

  *tipo = f->prim->tipo;
  *prio = f->prim->prio;

  if (f->num > 1)
    f->prim = f->prim->prox;
  else
  {
    f->prim = NULL;
    f->fim = NULL;
  }

  void *item = aux->item;
  free(aux);

  f->num--;
  return item;
}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f)
{
  if(!f || f->num < 0)
    return -1;
  return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
  if(!f || !f->num)
    return;
  
  struct fpnodo_t *aux = f->prim;
  for(int i = 0; i < f->num - 1; i++)
  {
    printf("(%d %d) ",aux->tipo, aux->prio);
    aux = aux->prox;
  }
  printf("(%d %d)",aux->tipo, aux->prio);
  
}


