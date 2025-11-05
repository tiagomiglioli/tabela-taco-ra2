#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "listaligada.h"
#include <strings.h>
#include "utilidades.h"

// compara strings ignorando maiúsc/minúsculas
static int cmp_icase(const char *a, const char *b)
{
    while (*a && *b)
    {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb)
            return (ca < cb) ? -1 : 1;
        a++;
        b++;
    }
    if (*a == *b)
        return 0;
    return (*a == '\0') ? -1 : 1;
}

// cria nó de alimento
static AlimentoNode *novoNoAlimento(Alimento *a)
{
    AlimentoNode *n = (AlimentoNode *)malloc(sizeof(AlimentoNode));
    if (!n)
        exit(1);
    n->dado = a;
    n->prox = NULL;
    return n;
}

CategoriaNode *inserirCategoriaOrdenada(CategoriaNode **inicio, const char *nome)
{
    // 1) se já existir com o MESMO nome (ignora caixa), retorna o existente
    CategoriaNode *p = *inicio, *ant = NULL;
    while (p)
    {
        int cmp = cmp_icase(nome, p->nome);
        if (cmp == 0)
        {
            return p; // já existe
        }
        // se o nome novo deve vir antes de p, paramos em 'ant'
        if (cmp < 0)
            break;
        ant = p;
        p = p->prox;
    }

    // 2) não existe: criar e inserir na posição (mantendo ordenação)
    CategoriaNode *novo = (CategoriaNode *)malloc(sizeof(CategoriaNode));
    if (!novo)
        exit(1);
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->listaAlimentos = NULL;
    novo->prox = p;

    if (ant == NULL)
    {
        *inicio = novo; // no começo
    }
    else
    {
        ant->prox = novo; // entre 'ant' e 'p'
    }
    return novo;
}

// Buscar categoria pelo nome (com normalização)
CategoriaNode *buscarCategoria(CategoriaNode *inicio, const char *nomeDigitado)
{
    char nomeProc[100], nomeCat[100];
    normalizarString(nomeDigitado, nomeProc); // Normaliza o nome digitado

    while (inicio != NULL)
    {
        normalizarString(inicio->nome, nomeCat); // Normaliza o nome da categoria
        if (strcmp(nomeProc, nomeCat) == 0)
            return inicio;
        inicio = inicio->prox;
    }
    return NULL;
}

int removerCategoria(CategoriaNode **inicio, const char *nome)
{
    CategoriaNode *p = *inicio, *ant = NULL;
    while (p && cmp_icase(p->nome, nome) != 0)
    {
        ant = p;
        p = p->prox;
    }
    if (!p)
        return 0; // não achou

    // desconecta
    if (ant)
        ant->prox = p->prox;
    else
        *inicio = p->prox;

    // libera lista de alimentos
    AlimentoNode *a = p->listaAlimentos;
    while (a)
    {
        AlimentoNode *tmp = a;
        a = a->prox;
        free(tmp);
    }
    free(p);
    return 1;
}

void listarCategorias(CategoriaNode *inicio)
{
    printf("\nCategorias cadastradas:\n");
    while (inicio)
    {
        printf("- %s\n", inicio->nome);
        inicio = inicio->prox;
    }
}

void inserirAlimentoOrdenado(CategoriaNode *categoria, Alimento *novo)
{
    AlimentoNode *no = novoNoAlimento(novo);

    // início da lista ou antes do primeiro (ordem alfabética pela descrição)
    if (categoria->listaAlimentos == NULL ||
        cmp_icase(novo->descricao_alimento, categoria->listaAlimentos->dado->descricao_alimento) < 0)
    {
        no->prox = categoria->listaAlimentos;
        categoria->listaAlimentos = no;
        return;
    }

    // encontra posição
    AlimentoNode *at = categoria->listaAlimentos;
    while (at->prox &&
           cmp_icase(novo->descricao_alimento, at->prox->dado->descricao_alimento) > 0)
    {
        at = at->prox;
    }
    no->prox = at->prox;
    at->prox = no;
}

void listarAlimentos(CategoriaNode *categoria)
{
    printf("\nAlimentos da categoria: %s\n", categoria->nome);
    AlimentoNode *a = categoria->listaAlimentos;
    while (a)
    {
        printf("  %d - %s (%.2f kcal | %.2f g proteína)\n",
               a->dado->numero_alimento,
               a->dado->descricao_alimento,
               a->dado->energia_kcal,
               a->dado->proteina_g);
        a = a->prox;
    }
}

int removerAlimento(CategoriaNode *categoria, int numeroAlimento)
{
    AlimentoNode *p = categoria->listaAlimentos, *ant = NULL;
    while (p && p->dado->numero_alimento != numeroAlimento)
    {
        ant = p;
        p = p->prox;
    }
    if (!p)
        return 0;

    if (ant)
        ant->prox = p->prox;
    else
        categoria->listaAlimentos = p->prox;
    free(p);
    return 1;
}

void liberarTudo(CategoriaNode *inicio)
{
    while (inicio)
    {
        CategoriaNode *cx = inicio;
        inicio = inicio->prox;

        AlimentoNode *a = cx->listaAlimentos;
        while (a)
        {
            AlimentoNode *t = a;
            a = a->prox;
            free(t);
        }
        free(cx);
    }
}