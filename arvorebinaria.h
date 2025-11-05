#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H

#include "alimento.h"
#include "listaligada.h"

// Nó da árvore binária
typedef struct NoArvore
{
    float chave;          // pode ser energia ou proteína
    Alimento *ref;        // ponteiro para o alimento
    struct NoArvore *esq; // filho da esquerda
    struct NoArvore *dir; // filho da direita
} NoArvore;

// Cria uma árvore com base nos alimentos da categoria, usando energia como chave
NoArvore *criarArvoreEnergia(CategoriaNode *categoria);

// Cria uma árvore com base nos alimentos da categoria, usando proteína como chave
NoArvore *criarArvoreProteina(CategoriaNode *categoria);

// Exibe em ordem decrescente
void exibirDecrescente(NoArvore *raiz);

// Exibe valores entre um mínimo e máximo
void exibirEntreFaixa(NoArvore *raiz, float min, float max);

// Libera toda memória da árvore
void liberarArvore(NoArvore *raiz);

#endif