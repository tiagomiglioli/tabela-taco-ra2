#ifndef LISTALIGADA_H
#define LISTALIGADA_H

#include "alimento.h"

// Nó simples de alimento em lista ligada
typedef struct AlimentoNode
{
    Alimento *dado;
    struct AlimentoNode *prox;
} AlimentoNode;

// Nó simples de categoria em lista ligada
typedef struct CategoriaNode
{
    char nome[100];
    AlimentoNode *listaAlimentos;
    struct CategoriaNode *prox;
} CategoriaNode;

// Insere categoria em ordem alfabética, se já não existir
CategoriaNode *inserirCategoriaOrdenada(CategoriaNode **inicio, const char *nome);

// Insere alimento numa categoria em ordem alfabética
void inserirAlimentoOrdenado(CategoriaNode *categoria, Alimento *novo);

// Lista todas as categorias
void listarCategorias(CategoriaNode *inicio);

// Lista alimentos de uma categoria (ordem alfabética)
void listarAlimentos(CategoriaNode *categoria);

// Busca categoria pelo nome
CategoriaNode *buscarCategoria(CategoriaNode *inicio, const char *nome);

// Remove uma categoria inteira
int removerCategoria(CategoriaNode **inicio, const char *nome);

// Remove um alimento específico dentro de uma categoria
int removerAlimento(CategoriaNode *categoria, int numeroAlimento);

// Libera a memória de todas as categorias e seus alimentos
void liberarTudo(CategoriaNode *inicio);

#endif
