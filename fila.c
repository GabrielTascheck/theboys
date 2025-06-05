#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fila.h"

static int verifica_repetido(struct fila_t *f, void *item)
{

  if (!f->num)
    return 0;

  struct fila_nodo_t *aux = f->prim;
  while (aux != NULL)
  {
    if (aux->item == item)
      return 1;
    aux = aux->prox;
  }
  return 0; // Não encontrou
}

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria()
{
  struct fila_t *fila = malloc(sizeof(struct fila_t));

  if (!fila)
    return 0;

  memset(fila, 0, sizeof(struct fila_t));

  return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi(struct fila_t *f)
{
  if (!f)
    return NULL;

  if(f->num == 1)
  {
    free(f->prim);
  }
  else
  {
    struct fila_nodo_t *aux;
    while (f->prim->prox)
    {
      aux = f->prim->prox;
      free(f->prim->item);
      free(f->prim);
      f->prim = aux;
    }
  }
  free(f);
  return NULL;
}

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere(struct fila_t *f, void *item)
{
  if (!f || !item)
    return -1;
  if (verifica_repetido(f, item))
    return -1;

  struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t));

  novo->item = item;
  novo->prox = NULL;
  if (!f->num)
  {
    f->prim = novo;
  }
  else
  {
    f->fim->prox = novo;
  }
  f->fim = novo;
  f->num++;

  return f->num;
}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira(struct fila_t *f)
{
  if (!f || !f->num)
    return NULL;

  struct fila_nodo_t *aux;
  void *item = f->prim->item;

  aux = f->prim;
  f->prim = aux->prox;
  free(aux);

  f->num--;
  if(!f->num)
    f->fim = NULL;

  return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho(struct fila_t *f)
{
  if (!f)
    return -1;
  return f->num;
}

// Imprime o conteúdo da fila
void fila_imprime(struct fila_t *f)
{
  {
    int i;
    struct fila_nodo_t *aux;
    aux = f->prim;
    for (i = 0; i < f->num - 1; i++)
    {
      printf("%d ", 2);
      aux = aux->prox;
    }
    if (f->num > 0)
      printf("%d", 2);
  }
}