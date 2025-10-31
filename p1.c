#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_texto 100
#define max_alimentos 600

typedef enum
{
    CARNES_DERIVADOS = 1,
    CEREAIS_DERIVADOS,
    GORDURAS_OLEOS,
    LEGUMINOSAS_DERIVADOS,
    LEITE_DERIVADOS,
    MISCELEANEAS,
    NOZES_SEMENTES,
    PESCADOS_MAR,
    PRODUTOS_ACUCARADOS,
    VERDURAS_HORTALICAS
} Categoria;

typedef struct
{
    int numero_alimento;
    char descricao_alimento[max_texto];
    float energia_kcal;
    float proteina_g;
    char categoria[max_texto];
} Alimento;

Alimento interpretar_alimento(char *linha)
{
    Alimento a;
    char *token;
    int coluna = 0;

    token = strtok(linha, ";");

    while (token != NULL)
    {
        switch (coluna)
        {
        case 0:
            a.numero_alimento = atoi(token);
            break;
        case 1:
            strncpy(a.descricao_alimento, token, max_texto);
            a.descricao_alimento[max_texto - 1] = '\0';
            break;
        case 2:
            a.energia_kcal = atof(token);
            break;
        case 3:
            a.proteina_g = atof(token);
            break;
        case 4:
            strncpy(a.categoria, token, max_texto);
            a.categoria[max_texto - 1] = '\0'; // coloca o /0
            break;
        }
        coluna++;
        token = strtok(NULL, ";");
    }
    return a;
}

int ler_csv(Alimento *alimentos)
{
    FILE *arq_read = fopen("taco.csv", "r");
    if (arq_read == NULL)
    {
        printf("Erro!");
        return 0;
    }

    int count = 0;   // iniciar o contador do nosso while
    char linha[265]; // dar uma folga

    // Pular a primeira pq é cabeçário

    fgets(linha, sizeof(linha), arq_read); // char = 1 byte, o sizeof vai ler 265

    while (fgets(linha, sizeof(linha), arq_read) != NULL && count < max_alimentos)
    {
        alimentos[count] = interpretar_alimento(linha);
        count++;
    }
    fclose(arq_read);
    return count;
}

void gravar_binario(Alimento *alimentos, int qtd)
{
    FILE *arq_bin = fopen("dados.bin", "wb");
    if (arq_bin == NULL)
    {
        printf("Erro ao abrir dados.bin\n");
        exit(1);
    }

    fwrite(alimentos, sizeof(Alimento), qtd, arq_bin);

    fclose(arq_bin);
}

int main()
{
    Alimento alimentos[600]; // 597 alimentos no CSV original
    int qtd = ler_csv(alimentos);

    if (qtd > 0)
    {
        gravar_binario(alimentos, qtd);
        printf("Arquivo dados.bin gerado com sucesso contendo %d alimentos\n", qtd);
    }
    else
    {
        printf("Erro ao ler taco.csv\n");
    }
    return 0;
}
