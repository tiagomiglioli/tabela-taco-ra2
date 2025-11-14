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

// =======================================================
// MUDANÇA (Requisito 1: Função helper do Enum)
// =======================================================
// Função para converter string em enum
EnumCategoria getEnumPeloNome(const char* nome)
{
    EnumCategoria id = CAT_INVALIDA;

    // Usamos strstr (contém) para mais segurança
    if (strstr(nome, "Cereais")) id = CAT_CEREAIS;
    else if (strstr(nome, "Verduras")) id = CAT_VERDURAS;
    else if (strstr(nome, "Frutas")) id = CAT_FRUTAS;
    else if (strstr(nome, "Gorduras")) id = CAT_GORDURAS;
    else if (strstr(nome, "Pescados")) id = CAT_PESCADOS;
    else if (strstr(nome, "Carnes")) id = CAT_CARNES;
    else if (strstr(nome, "Leite")) id = CAT_LEITE;
    else if (strstr(nome, "Bebidas")) id = CAT_BEBIDAS;
    else if (strstr(nome, "Ovos")) id = CAT_OVOS;
    else if (strstr(nome, "Produtos açucarados")) id = CAT_ACUCARADOS;
    else if (strstr(nome, "Miscelâneas")) id = CAT_MISCELANEAS;
    else if (strstr(nome, "industrializados")) id = CAT_INDUSTRIALIZADOS;
    else if (strstr(nome, "Alimentos preparados")) id = CAT_PREPARADOS;
    else if (strstr(nome, "Leguminosas")) id = CAT_LEGUMINOSAS;
    else if (strstr(nome, "Nozes")) id = CAT_NOZES;
    
    return id;
}


CategoriaNode *inserirCategoriaOrdenada(CategoriaNode **inicio, const char *nome)
{
    CategoriaNode *p = *inicio, *ant = NULL;
    CategoriaNode *resultado = NULL; // <-- Variável de resultado
    int cmp = 1;
    int continuar = 1; // Flag para parar o loop (Req 9: sem 'break')

    // 1) Loop para encontrar posição ou nó existente
    while (p && continuar)
    {
        cmp = cmp_icase(nome, p->nome);
        if (cmp == 0)
        {
            resultado = p; // já existe
            continuar = 0; // Para o loop
        }
        else if (cmp < 0)
        {
            continuar = 0; // Achou ponto de inserção
        }
        else
        {
            // Avança
            ant = p;
            p = p->prox;
        }
    }

    // 2) Se não achou nó existente, cria um novo
    if (resultado == NULL)
    {
        CategoriaNode *novo = (CategoriaNode *)malloc(sizeof(CategoriaNode));
        if (!novo)
            exit(1);
        strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
        novo->nome[sizeof(novo->nome) - 1] = '\0';
        novo->listaAlimentos = NULL;
        novo->prox = p;
        
        // =======================================================
        // MUDANÇA (Requisito 1: Preencher o campo enum)
        // =======================================================
        novo->id_enum = getEnumPeloNome(novo->nome);
        // =======================================================

        if (ant == NULL)
        {
            *inicio = novo; // no começo
        }
        else
        {
            ant->prox = novo; // entre 'ant' e 'p'
        }
        resultado = novo; // Seta o resultado
    }

    return resultado; // <-- Return Único
}


CategoriaNode *buscarCategoria(CategoriaNode *inicio, const char *nomeDigitado)
{
    char nomeProc[100], nomeCat[100];
    normalizarString(nomeDigitado, nomeProc); // Normaliza o nome digitado

    CategoriaNode *resultado = NULL; // <-- Variável de resultado
    int continuar = 1; // Flag de loop (sem 'break')

    while (inicio != NULL && continuar == 1)
    {
        normalizarString(inicio->nome, nomeCat); // Normaliza o nome da categoria
        if (strcmp(nomeProc, nomeCat) == 0)
        {
            resultado = inicio;
            continuar = 0; // Para o loop
        }
        
        if (continuar == 1) // Só avança se não achou
        {
            inicio = inicio->prox;
        }
    }
    return resultado; // <-- Return Único
}


int removerCategoria(CategoriaNode **inicio, const char *nome)
{
    int sucesso = 0; // <-- Variável de resultado
    CategoriaNode *p = *inicio, *ant = NULL;
    int continuar = 1; // Flag de loop

    while (p && continuar)
    {
        if (cmp_icase(p->nome, nome) == 0)
            continuar = 0; // Achou
        else
        {
            ant = p;
            p = p->prox;
        }
    }

    if (p != NULL) // Se achou (p não é NULL)
    {
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
        sucesso = 1; // Seta sucesso
    }

    return sucesso; // <-- Return Único
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
    int cmp;

    // início da lista ou antes do primeiro (ordem alfabética pela descrição)
    if (categoria->listaAlimentos == NULL)
    {
        cmp = -1; // Força inserção no início
    }
    else
    {
        cmp = cmp_icase(novo->descricao_alimento, categoria->listaAlimentos->dado->descricao_alimento);
    }
    
    if (cmp < 0)
    {
        no->prox = categoria->listaAlimentos;
        categoria->listaAlimentos = no;
    }
    else
    {
        // encontra posição
        AlimentoNode *at = categoria->listaAlimentos;
        int continuar = 1; // Flag de loop
        while (at->prox && continuar)
        {
            if (cmp_icase(novo->descricao_alimento, at->prox->dado->descricao_alimento) > 0)
                at = at->prox;
            else
                continuar = 0; // Achou ponto de inserção
        }
        no->prox = at->prox;
        at->prox = no;
    }
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
    int sucesso = 0; // <-- Variável de resultado
    AlimentoNode *p = categoria->listaAlimentos, *ant = NULL;
    int continuar = 1; // Flag de loop

    while (p && continuar)
    {
        if (p->dado->numero_alimento == numeroAlimento)
            continuar = 0; // Achou
        else
        {
            ant = p;
            p = p->prox;
        }
    }
    
    if (p != NULL) // Se achou
    {
        if (ant)
            ant->prox = p->prox;
        else
            categoria->listaAlimentos = p->prox;
        free(p);
        sucesso = 1; // Seta sucesso
    }
    
    return sucesso; // <-- Return Único
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