#ifndef LISTALIGADA_H
#define LISTALIGADA_H

#include "alimento.h"

// =======================================================
// MUDANÇA (Requisito 1: Implementar Enum)
// =======================================================
// 1. O Enum (são 15 categorias)
typedef enum {
    CAT_CEREAIS,
    CAT_VERDURAS,
    CAT_FRUTAS,
    CAT_GORDURAS,
    CAT_PESCADOS,
    CAT_CARNES,
    CAT_LEITE,
    CAT_BEBIDAS,
    CAT_OVOS,
    CAT_ACUCARADOS,
    CAT_MISCELANEAS,
    CAT_INDUSTRIALIZADOS,
    CAT_PREPARADOS,
    CAT_LEGUMINOSAS,
    CAT_NOZES,
    CAT_INVALIDA // Para erros
} EnumCategoria;

// 2. Protótipo da função de conversão
EnumCategoria getEnumPeloNome(const char* nome);
// =======================================================


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
    
    // =======================================================
    // MUDANÇA (Requisito 1: Adicionar campo do Enum)
    // =======================================================
    EnumCategoria id_enum;
    // =======================================================
    
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