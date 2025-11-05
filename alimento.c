#include "alimento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            strncpy(a.descricao_alimento, token, MAX_TEXTO - 1);
            a.descricao_alimento[MAX_TEXTO - 1] = '\0';
            break;
        case 2:
            a.energia_kcal = atof(token);
            break;
        case 3:
            a.proteina_g = atof(token);
            break;
        case 4:
            strncpy(a.categoria, token, MAX_TEXTO - 1);
            a.categoria[MAX_TEXTO - 1] = '\0';
            break;
        }
        coluna++;
        token = strtok(NULL, ";");
    }
    return a;
}

int ler_csv(Alimento *alimentos)
{
    FILE *arq = fopen("taco.csv", "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir taco.csv!\n");
        return 0;
    }

    int count = 0;
    char linha[300];

    // Pular cabeçalho
    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL && count < MAX_ALIMENTOS)
    {
        alimentos[count] = interpretar_alimento(linha);
        count++;
    }

    fclose(arq);
    return count;
}

void gravar_binario(Alimento *alimentos, int qtd)
{
    FILE *arq = fopen("dados.bin", "wb");
    if (arq == NULL)
    {
        printf("Erro ao criar dados.bin!\n");
        exit(1);
    }

    fwrite(alimentos, sizeof(Alimento), qtd, arq);
    fclose(arq);
}
