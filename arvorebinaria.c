#include <stdio.h>
#include <stdlib.h>
#include "arvorebinaria.h"

// Função auxiliar para criar novo nó (já estava ok, mas verificando malloc)
static NoArvore *criarNo(float chave, Alimento *ref)
{
    NoArvore *no = (NoArvore *)malloc(sizeof(NoArvore));
    if (no != NULL)
    {
        no->chave = chave;
        no->ref = ref;
        no->esq = NULL;
        no->dir = NULL;
    }
    return no;
}


static NoArvore *inserir(NoArvore *raiz, float chave, Alimento *ref)
{
    NoArvore *resultado = NULL; // <-- Variável de resultado

    if (raiz == NULL)
    {
        resultado = criarNo(chave, ref); // Base da recursão
    }
    else
    {
        if (chave < raiz->chave)
            raiz->esq = inserir(raiz->esq, chave, ref);
        else
            raiz->dir = inserir(raiz->dir, chave, ref);
        
        resultado = raiz; // Nó raiz (pai) não muda
    }

    return resultado; // <-- Return Único
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
    return raiz; // Este 'return' é único
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
    return raiz; // Este 'return' é único
}


void exibirDecrescente(NoArvore *raiz)
{
    if (raiz != NULL) // <-- Lógica invertida
    {
        exibirDecrescente(raiz->dir);
        printf("%s: %.2f\n", raiz->ref->descricao_alimento, raiz->chave);
        exibirDecrescente(raiz->esq);
    }
    // Return implícito e único no final
}


void exibirEntreFaixa(NoArvore *raiz, float min, float max)
{
    if (raiz != NULL) // <-- Lógica invertida
    {
        if (raiz->chave >= min)
            exibirEntreFaixa(raiz->esq, min, max);

        if (raiz->chave >= min && raiz->chave <= max)
            printf("%s: %.2f\n", raiz->ref->descricao_alimento, raiz->chave);

        if (raiz->chave <= max)
            exibirEntreFaixa(raiz->dir, min, max);
    }
    // Return implícito e único no final
}


void liberarArvore(NoArvore *raiz)
{
    if (raiz != NULL) // <-- Lógica invertida
    {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
    // Return implícito e único no final
}