#include <stdio.h>
#include <locale.h>
#include "alimento.h"
#include "listaligada.h"
#include "arvorebinaria.h"
#include "utilidades.h"

// Exibe menu principal
void exibirMenu()
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

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    // Carregar dados de arquivo binário
    Alimento vetor[MAX_ALIMENTOS];
    int qtd = carregar_binario("dados.bin", vetor, MAX_ALIMENTOS);

    if (qtd <= 0)
    {
        printf("Erro ao carregar dados.bin! Rode o P1 primeiro.\n");
        return 0;
    }
    printf("Carregados %d alimentos do arquivo binário!\n", qtd);

    // Construir lista de categorias com alimentos ordenados
    CategoriaNode *categorias = NULL;
    for (int i = 0; i < qtd; i++)
    {
        CategoriaNode *cat = inserirCategoriaOrdenada(&categorias, vetor[i].categoria);
        inserirAlimentoOrdenado(cat, &vetor[i]);
    }

    // 3. Menu de opções com switch-case 🌟
    int rodando = 1, houveRemocao = 0;

    while (rodando)
    {
        exibirMenu();
        int opcao = ler_inteiro();

        switch (opcao)
        {
        case 1: // Listar categorias
            listarCategorias(categorias);
            break;

        case 2:
        { // Listar alimentos de categoria
            char nome[100];
            printf("Digite nome da categoria: ");
            lerString(nome, 100);

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat)
                listarAlimentos(cat);
            else
                printf("Categoria não encontrada!\n");
            break;
        }

        case 3:
        { // Listar por energia (decrescente)
            char nome[100];
            printf("Digite nome da categoria: ");
            lerString(nome, 100);

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat)
            {
                NoArvore *raiz = criarArvoreEnergia(cat);
                exibirDecrescente(raiz);
                liberarArvore(raiz);
            }
            else
                printf("Categoria não encontrada!\n");
            break;
        }

        case 4:
        { // Listar por proteína (decrescente)
            char nome[100];
            printf("Digite nome da categoria: ");
            lerString(nome, 100);

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat)
            {
                NoArvore *raiz = criarArvoreProteina(cat);
                exibirDecrescente(raiz);
                liberarArvore(raiz);
            }
            else
                printf("Categoria não encontrada!\n");
            break;
        }

        case 5:
        { // Listar por faixa de energia
            char nome[100];
            printf("Categoria: ");
            lerString(nome, 100);

            printf("Valor mínimo (kcal): ");
            float min = ler_float();
            printf("Valor máximo (kcal): ");
            float max = ler_float();

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat)
            {
                NoArvore *raiz = criarArvoreEnergia(cat);
                exibirEntreFaixa(raiz, min, max);
                liberarArvore(raiz);
            }
            else
                printf("Categoria não encontrada!\n");
            break;
        }

        case 6:
        { // Listar por faixa de proteína
            char nome[100];
            printf("Categoria: ");
            lerString(nome, 100);

            printf("Valor mínimo (g proteína): ");
            float min = ler_float();
            printf("Valor máximo (g proteína): ");
            float max = ler_float();

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat)
            {
                NoArvore *raiz = criarArvoreProteina(cat);
                exibirEntreFaixa(raiz, min, max);
                liberarArvore(raiz);
            }
            else
                printf("Categoria não encontrada!\n");
            break;
        }

        case 7:
        { // Remover categoria
            char nome[100];
            printf("Nome da categoria a remover: ");
            lerString(nome, 100);
            if (removerCategoria(&categorias, nome))
            {
                printf("Categoria removida com sucesso!\n");
                houveRemocao = 1;
            }
            else
            {
                printf("Categoria não encontrada.\n");
            }
            break;
        }

        case 8:
        { // Remover alimento
            char nome[100];
            printf("Nome da categoria: ");
            lerString(nome, 100);

            printf("Número do alimento: ");
            int num = ler_inteiro();

            CategoriaNode *cat = buscarCategoria(categorias, nome);
            if (cat && removerAlimento(cat, num))
            {
                printf("Alimento removido!\n");
                houveRemocao = 1;
            }
            else
            {
                printf("Alimento ou categoria não encontrados.\n");
            }
            break;
        }

        case 9: // Encerrar
            if (houveRemocao)
            {
                int count = 0;
                CategoriaNode *c = categorias;
                while (c)
                {
                    AlimentoNode *a = c->listaAlimentos;
                    while (a)
                    {
                        vetor[count++] = *(a->dado);
                        a = a->prox;
                    }
                    c = c->prox;
                }
                gravar_binario(vetor, count);
                printf("Arquivo dados.bin atualizado com %d alimentos!\n", count);
            }
            rodando = 0;
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    }

    // 4. Liberar memória antes de finalizar
    liberarTudo(categorias);
    printf("Programa encerrado.\n");
    return 0;
}
