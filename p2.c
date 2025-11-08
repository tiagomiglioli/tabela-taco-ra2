#include <stdio.h>
#include <locale.h>
#include "alimento.h"
#include "listaligada.h"
#include "arvorebinaria.h"
#include "utilidades.h"

// ===== Helpers de UI =====

static void exibirMenu(void)
{
    printf("\n=========== MENU DE OPÇÕES ===========\n");
    printf("1. Listar categorias\n");
    printf("2. Listar alimentos de uma categoria (alfabética)\n");
    printf("3. Listar alimentos por energia (decrescente)\n");
    printf("4. Listar alimentos por proteína (decrescente)\n");
    printf("5. Listar alimentos por faixa de energia\n");
    printf("6. Listar alimentos por faixa de proteína\n");
    printf("7. Remover categoria\n");
    printf("8. Remover alimento\n");
    printf("9. Encerrar programa\n");
    printf("======================================\n");
    printf("Escolha uma opção: ");
}

// Conta quantas categorias existem
static int contarCategorias(CategoriaNode *ini)
{
    int n = 0;
    while (ini)
    {
        n++;
        ini = ini->prox;
    }
    return n;
}

// Preenche um vetor com ponteiros para as categorias (na ordem da lista)
static void listarCategoriasNumeradas(CategoriaNode *ini, CategoriaNode **vet, int *n_out)
{
    int i = 0;
    printf("\nCategorias cadastradas:\n");
    while (ini)
    {
        printf("%2d) %s\n", i + 1, ini->nome);
        if (vet)
            vet[i] = ini;
        i++;
        ini = ini->prox;
    }
    if (n_out)
        *n_out = i;
}

// Deixa o usuário escolher a categoria pelo número (retorna NULL se inválido)
static CategoriaNode *escolherCategoriaPorNumero(CategoriaNode *lista)
{
    int total = contarCategorias(lista);
    if (total == 0)
    {
        printf("Não há categorias.\n");
        return NULL;
    }

    // Monta vetor de ponteiros
    CategoriaNode *vet[64]; // 64 é mais que suficiente para este trabalho
    if (total > 64)
        total = 64; // segurança
    listarCategoriasNumeradas(lista, vet, &total);

    printf("Digite o número da categoria: ");
    int idx = ler_inteiro();
    if (idx < 1 || idx > total)
    {
        printf("Número inválido.\n");
        return NULL;
    }
    return vet[idx - 1];
}

// Copia todos os alimentos da lista encadeada para um vetor contíguo (para salvar no bin)
static int categoriasParaVetor(CategoriaNode *lista, Alimento *out, int max)
{
    int k = 0;
    while (lista && k < max)
    {
        AlimentoNode *a = lista->listaAlimentos;
        while (a && k < max)
        {
            out[k++] = *(a->dado);
            a = a->prox;
        }
        lista = lista->prox;
    }
    return k;
}

// ===== Programa principal =====

int main(void)
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    // 1) Carregar dados do binário
    Alimento vetor[MAX_ALIMENTOS];
    int qtd = carregar_binario("dados.bin", vetor, MAX_ALIMENTOS);
    if (qtd <= 0)
    {
        printf("Erro ao carregar dados.bin! Rode o P1 primeiro.\n");
        return 0;
    }
    printf("Carregados %d alimentos do arquivo binário!\n", qtd);

    // 2) Construir lista de categorias (ordenada) e inserir alimentos (ordenados)
    CategoriaNode *categorias = NULL;
    for (int i = 0; i < qtd; i++)
    {
        CategoriaNode *cat = inserirCategoriaOrdenada(&categorias, vetor[i].categoria);
        inserirAlimentoOrdenado(cat, &vetor[i]);
    }

    int rodando = 1;
    int sujo = 0; // houve remoções?

    while (rodando)
    {
        exibirMenu();
        int op = ler_inteiro();

        switch (op)
        {
        case 1: // Listar categorias
            listarCategoriasNumeradas(categorias, NULL, NULL);
            break;

        case 2: // Listar alimentos de uma categoria (alfabética)
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (cat)
                listarAlimentos(cat);
            break;
        }

        case 3: // Energia decrescente (usar árvore)
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (cat)
            {
                NoArvore *raiz = criarArvoreEnergia(cat);
                exibirDecrescente(raiz);
                liberarArvore(raiz);
            }
            break;
        }

        case 4: // Proteína decrescente (usar árvore)
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (cat)
            {
                NoArvore *raiz = criarArvoreProteina(cat);
                exibirDecrescente(raiz);
                liberarArvore(raiz);
            }
            break;
        }

        case 5: // Faixa de energia
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (cat)
            {
                printf("Valor mínimo (kcal): ");
                float minv = ler_float();
                printf("Valor máximo (kcal): ");
                float maxv = ler_float();

                NoArvore *raiz = criarArvoreEnergia(cat);
                exibirEntreFaixa(raiz, minv, maxv);
                liberarArvore(raiz);
            }
            break;
        }

        case 6: // Faixa de proteína
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (cat)
            {
                printf("Valor mínimo (g): ");
                float minv = ler_float();
                printf("Valor máximo (g): ");
                float maxv = ler_float();

                NoArvore *raiz = criarArvoreProteina(cat);
                exibirEntreFaixa(raiz, minv, maxv);
                liberarArvore(raiz);
            }
            break;
        }

        case 7: // Remover categoria
        {
            // Mostrar numerado e remover pelo nome real (interno)
            int total = contarCategorias(categorias);
            if (total == 0)
            {
                printf("Não há categorias.\n");
                break;
            }

            CategoriaNode *vet[64];
            if (total > 64)
                total = 64;
            listarCategoriasNumeradas(categorias, vet, &total);

            printf("Número da categoria a remover: ");
            int idx = ler_inteiro();
            if (idx < 1 || idx > total)
            {
                printf("Número inválido.\n");
                break;
            }

            if (removerCategoria(&categorias, vet[idx - 1]->nome))
            {
                printf("Categoria removida.\n");
                sujo = 1;
            }
            else
            {
                printf("Falha ao remover categoria.\n");
            }
            break;
        }

        case 8: // Remover alimento
        {
            CategoriaNode *cat = escolherCategoriaPorNumero(categorias);
            if (!cat)
                break;

            listarAlimentos(cat);

            printf("Número do alimento a ser removido: ");
            int num = ler_inteiro();

            if (removerAlimento(cat, num))
            {
                printf("Alimento removido!\n");
                sujo = 1;
            }
            else
            {
                printf("Alimento não encontrado nessa categoria.\n");
            }
            break;
        }

        case 9: // Encerrar
        {
            if (sujo)
            {
                int n = categoriasParaVetor(categorias, vetor, MAX_ALIMENTOS);
                gravar_binario(vetor, n);
                printf("Arquivo dados.bin atualizado com %d alimentos!\n", n);
            }
            rodando = 0;
            break;
        }

        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }
    }

    liberarTudo(categorias);
    printf("Programa encerrado.\n");
    return 0;
}
