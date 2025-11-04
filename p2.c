#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "alimento.h"
#include "listaligada.h"
#include "arvorebinaria.h"
#include "utilidades.h"

static void construir_estruturas(CategoriaNode **lista, Alimento *v, int qtd)
{
    int i = 0;
    while (i < qtd)
    {
        CategoriaEnum ce = categoria_from_str(v[i].categoria);
        CategoriaNode *cat = categorias_inserir_ordenado(lista, ce);
        alimentos_inserir_ordenado(cat, &v[i]);
        i++;
    }
    CategoriaNode *c = *lista;
    while (c)
    {
        indices_reconstruir(c);
        c = c->prox;
    }
}

static void op1_listar_categorias(CategoriaNode *lista)
{
    printf("\n[1] Categorias (ordem alfabética)\n");
    categorias_listar(lista);
}

static void op2_listar_alimentos_categoria(CategoriaNode *lista)
{
    printf("\n[2] Listar alimentos (ordem alfabética) de uma categoria\n");
    CategoriaEnum ce = escolher_categoria_menu();
    CategoriaNode *cat = categorias_buscar(lista, ce);
    if (!cat)
        printf("Categoria sem itens nos dados.\n");
    else
    {
        printf("\n%s:\n", cat->nome);
        alimentos_listar_alfabetico(cat);
    }
}

static void op3_energia_dec(CategoriaNode *lista)
{
    printf("\n[3] Listar por energia (decrescente) usando árvore\n");
    CategoriaEnum ce = escolher_categoria_menu();
    CategoriaNode *cat = categorias_buscar(lista, ce);
    if (!cat)
        printf("Categoria sem itens nos dados.\n");
    else
    {
        printf("\n%s:\n", cat->nome);
        alimentos_listar_por_energia_desc(cat);
    }
}

static void op4_proteina_dec(CategoriaNode *lista)
{
    printf("\n[4] Listar por proteína (decrescente) usando árvore\n");
    CategoriaEnum ce = escolher_categoria_menu();
    CategoriaNode *cat = categorias_buscar(lista, ce);
    if (!cat)
        printf("Categoria sem itens nos dados.\n");
    else
    {
        printf("\n%s:\n", cat->nome);
        alimentos_listar_por_proteina_desc(cat);
    }
}

static void op5_faixa_energia(CategoriaNode *lista)
{
    printf("\n[5] Alimentos com energia entre MIN e MAX (kcal)\n");
    CategoriaEnum ce = escolher_categoria_menu();
    CategoriaNode *cat = categorias_buscar(lista, ce);
    if (!cat)
        printf("Categoria sem itens nos dados.\n");
    else
    {
        printf("Mínimo (kcal): ");
        float minv = ler_float();
        printf("Máximo (kcal): ");
        float maxv = ler_float();
        printf("\n%s entre %.2f e %.2f kcal:\n", cat->nome, minv, maxv);
        alimentos_buscar_energia_faixa(cat, minv, maxv);
    }
}

static void op6_faixa_proteina(CategoriaNode *lista)
{
    printf("\n[6] Alimentos com proteína entre MIN e MAX (g)\n");
    CategoriaEnum ce = escolher_categoria_menu();
    CategoriaNode *cat = categorias_buscar(lista, ce);
    if (!cat)
        printf("Categoria sem itens nos dados.\n");
    else
    {
        printf("Mínimo (g): ");
        float minv = ler_float();
        printf("Máximo (g): ");
        float maxv = ler_float();
        printf("\n%s entre %.2f e %.2f g proteína:\n", cat->nome, minv, maxv);
        alimentos_buscar_proteina_faixa(cat, minv, maxv);
    }
}

static int op7_remover_categoria(CategoriaNode **lista)
{
    printf("\n[7] Remover uma categoria\n");
    CategoriaEnum ce = escolher_categoria_menu();
    int ok = categoria_remover(lista, ce);
    if (ok)
        printf("Categoria removida.\n");
    else
        printf("Categoria não encontrada.\n");
    return ok;
}

static int op8_remover_alimento(CategoriaNode *lista)
{
    printf("\n[8] Remover um alimento específico\n");
    CategoriaEnum ce = escolher_categoria_menu();
    printf("Número do alimento: ");
    int num = ler_inteiro();
    int ok = alimento_remover_por_numero(lista, ce, num);
    if (ok)
        printf("Alimento removido (índices atualizados).\n");
    else
        printf("Alimento não encontrado nessa categoria.\n");
    return ok;
}

int main()
{
    setlocale(LC_ALL, "");

    Alimento buf[MAX_ALIMENTOS];
    int qtd = carregar_binario("dados.bin", buf, MAX_ALIMENTOS);
    if (qtd <= 0)
    {
        printf("Não foi possível carregar dados.bin. Gere pelo P1 primeiro.\n");
        return 0;
    }
    printf("Carregados %d alimentos.\n", qtd);

    CategoriaNode *lista = NULL;
    construir_estruturas(&lista, buf, qtd);

    int sair = 0, sujo = 0;
    while (!sair)
    {
        printf("\n=====================================================\n");
        printf("                    MENU DE OPÇÕES                   \n");
        printf("=====================================================\n");
        printf("1) Liste todas as categorias\n");
        printf("2) Liste alimentos de uma categoria (ordem alfabética)\n");
        printf("3) Liste alimentos por energia (decrescente) [árvore]\n");
        printf("4) Liste alimentos por proteína (decrescente) [árvore]\n");
        printf("5) Liste alimentos de uma categoria por FAIXA de energia [árvore]\n");
        printf("6) Liste alimentos de uma categoria por FAIXA de proteína [árvore]\n");
        printf("7) Remova uma categoria de alimentos\n");
        printf("8) Remova um alimento específico\n");
        printf("9) Encerrar (atualiza dados.bin se houve remoções)\n");
        printf("Escolha: ");
        int op = ler_inteiro();

        if (op == 1)
            op1_listar_categorias(lista);
        else if (op == 2)
            op2_listar_alimentos_categoria(lista);
        else if (op == 3)
            op3_energia_dec(lista);
        else if (op == 4)
            op4_proteina_dec(lista);
        else if (op == 5)
            op5_faixa_energia(lista);
        else if (op == 6)
            op6_faixa_proteina(lista);
        else if (op == 7)
        {
            if (op7_remover_categoria(&lista))
                sujo = 1;
        }
        else if (op == 8)
        {
            if (op8_remover_alimento(lista))
                sujo = 1;
        }
        else if (op == 9)
        {
            if (sujo)
            {
                Alimento tmp[MAX_ALIMENTOS];
                int n = estruturas_para_vetor(lista, tmp);
                int ok = salvar_binario("dados.bin", tmp, n);
                if (ok)
                    printf("dados.bin atualizado com %d registros.\n", n);
                else
                    printf("Falha ao atualizar dados.bin.\n");
            }
            sair = 1;
        }
        else
        {
            printf("Opção inválida.\n");
        }
    }

    estruturas_destruir(lista);
    return 0;
}
