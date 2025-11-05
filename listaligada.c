#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaligada.h"

// Função auxiliar pra criar nó de alimento
static AlimentoNode *novoNoAlimento(Alimento *a)
{
    AlimentoNode *n = (AlimentoNode *)malloc(sizeof(AlimentoNode));
    n->dado = a;
    n->prox = NULL;
    return n;
}

// Insere categoria em ordem alfabética
CategoriaNode *inserirCategoriaOrdenada(CategoriaNode **inicio, const char *nome)
{
    CategoriaNode *novo = (CategoriaNode *)malloc(sizeof(CategoriaNode));
    strcpy(novo->nome, nome);
    novo->listaAlimentos = NULL;
    novo->prox = NULL;

    // Se lista vazia ou é menor que o primeiro
    if (*inicio == NULL || strcmp(nome, (*inicio)->nome) < 0)
    {
        novo->prox = *inicio;
        *inicio = novo;
        return novo;
    }

    // Busca posição correta
    CategoriaNode *atual = *inicio;
    while (atual->prox != NULL && strcmp(nome, atual->prox->nome) > 0)
    {
        atual = atual->prox;
    }

    // Insere entre atual e o próximo
    novo->prox = atual->prox;
    atual->prox = novo;
    return novo;
}

// Insere alimento em ordem alfabética na categoria
void inserirAlimentoOrdenado(CategoriaNode *categoria, Alimento *novo)
{
    AlimentoNode *no = novoNoAlimento(novo);

    // Se a lista está vazia ou vem antes do primeiro
    if (categoria->listaAlimentos == NULL ||
        strcmp(novo->descricao_alimento, categoria->listaAlimentos->dado->descricao_alimento) < 0)
    {
        no->prox = categoria->listaAlimentos;
        categoria->listaAlimentos = no;
        return;
    }

    // Busca posição correta na lista ligada
    AlimentoNode *atual = categoria->listaAlimentos;
    while (atual->prox != NULL &&
           strcmp(novo->descricao_alimento, atual->prox->dado->descricao_alimento) > 0)
    {
        atual = atual->prox;
    }

    // Insere o nó na posição
    no->prox = atual->prox;
    atual->prox = no;
}

// Buscar categoria pelo nome
CategoriaNode *buscarCategoria(CategoriaNode *inicio, const char *nome)
{
    while (inicio != NULL)
    {
        if (strcmp(inicio->nome, nome) == 0)
            return inicio;
        inicio = inicio->prox;
    }
    return NULL;
}

// Listar categorias
void listarCategorias(CategoriaNode *inicio)
{
    printf("\nCategorias cadastradas:\n");
    while (inicio != NULL)
    {
        printf("- %s\n", inicio->nome);
        inicio = inicio->prox;
    }
}

// Listar alimentos de uma categoria
void listarAlimentos(CategoriaNode *categoria)
{
    printf("\nAlimentos da categoria: %s\n", categoria->nome);
    AlimentoNode *a = categoria->listaAlimentos;
    while (a != NULL)
    {
        printf("  %d - %s (%.2fkcal | %.2fg proteina)\n",
               a->dado->numero_alimento,
               a->dado->descricao_alimento,
               a->dado->energia_kcal,
               a->dado->proteina_g);
        a = a->prox;
    }
}

// Remover categoria inteira
int removerCategoria(CategoriaNode **inicio, const char *nome)
{
    CategoriaNode *atual = *inicio, *ant = NULL;
    while (atual != NULL && strcmp(atual->nome, nome) != 0)
    {
        ant = atual;
        atual = atual->prox;
    }
    if (!atual)
        return 0;

    // Remove da lista encadeada
    if (ant == NULL)
        *inicio = atual->prox;
    else
        ant->prox = atual->prox;

    // Libera alimentos daquela categoria
    AlimentoNode *a = atual->listaAlimentos;
    while (a)
    {
        AlimentoNode *tmp = a;
        a = a->prox;
        free(tmp);
    }
    free(atual);
    return 1;
}

// Remover alimento de uma categoria
int removerAlimento(CategoriaNode *categoria, int numero)
{
    AlimentoNode *atual = categoria->listaAlimentos;
    AlimentoNode *ant = NULL;

    while (atual != NULL && atual->dado->numero_alimento != numero)
    {
        ant = atual;
        atual = atual->prox;
    }
    if (!atual)
        return 0;

    if (ant == NULL)
        categoria->listaAlimentos = atual->prox;
    else
        ant->prox = atual->prox;

    free(atual);
    return 1;
}

// Liberar toda a memória da lista ligada
void liberarTudo(CategoriaNode *inicio)
{
    while (inicio != NULL)
    {
        CategoriaNode *tmpCat = inicio;
        inicio = inicio->prox;

        AlimentoNode *a = tmpCat->listaAlimentos;
        while (a != NULL)
        {
            AlimentoNode *tmpAli = a;
            a = a->prox;
            free(tmpAli);
        }
        free(tmpCat);
    }
}
