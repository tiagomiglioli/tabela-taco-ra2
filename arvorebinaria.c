#include <stdio.h>
#include <stdlib.h>
#include "arvorebinaria.h"

// Função auxiliar para criar novo nó
static NoArvore *criarNo(float chave, Alimento *ref)
{
    NoArvore *no = (NoArvore *)malloc(sizeof(NoArvore));
    no->chave = chave;
    no->ref = ref;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

// Insere nó na árvore binária
static NoArvore *inserir(NoArvore *raiz, float chave, Alimento *ref)
{
    if (raiz == NULL)
        return criarNo(chave, ref);

    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave, ref);
    else
        raiz->dir = inserir(raiz->dir, chave, ref);

    return raiz;
}

// Construir árvore de energia com base na lista ligada
NoArvore *criarArvoreEnergia(CategoriaNode *categoria)
{
    NoArvore *raiz = NULL;
    AlimentoNode *a = categoria->listaAlimentos;

    while (a != NULL)
    {
        raiz = inserir(raiz, a->dado->energia_kcal, a->dado);
        a = a->prox;
    }
    return raiz;
}

// Construir árvore de proteína com base na lista ligada
NoArvore *criarArvoreProteina(CategoriaNode *categoria)
{
    NoArvore *raiz = NULL;
    AlimentoNode *a = categoria->listaAlimentos;

    while (a != NULL)
    {
        raiz = inserir(raiz, a->dado->proteina_g, a->dado);
        a = a->prox;
    }
    return raiz;
}

// Exibe em ordem decrescente
void exibirDecrescente(NoArvore *raiz)
{
    if (raiz == NULL)
        return;
    exibirDecrescente(raiz->dir);
    printf("%s: %.2f\n", raiz->ref->descricao_alimento, raiz->chave);
    exibirDecrescente(raiz->esq);
}

// Exibe nós com chave entre
void exibirEntreFaixa(NoArvore *raiz, float min, float max)
{
    if (raiz == NULL)
        return;

    if (raiz->chave >= min)
        exibirEntreFaixa(raiz->esq, min, max);

    if (raiz->chave >= min && raiz->chave <= max)
        printf("%s: %.2f\n", raiz->ref->descricao_alimento, raiz->chave);

    if (raiz->chave <= max)
        exibirEntreFaixa(raiz->dir, min, max);
}

// Libera toda memória de uma árvore binária
void liberarArvore(NoArvore *raiz)
{
    if (!raiz)
        return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}
