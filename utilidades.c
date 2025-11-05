#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"

void lerString(char *str, int tamanho)
{
    if (fgets(str, tamanho, stdin) != NULL)
    {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
            str[len - 1] = '\0';
    }
}

int ler_inteiro(void)
{
    int valor;
    scanf("%d", &valor);
    limparBuffer();
    return valor;
}

float ler_float(void)
{
    float valor;
    scanf("%f", &valor);
    limparBuffer();
    return valor;
}

void limparBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int carregar_binario(const char *nomeArquivo, Alimento *vetor, int max)
{
    FILE *arq = fopen(nomeArquivo, "rb");
    if (arq == NULL)
    {
        printf("Erro ao abrir %s.\n", nomeArquivo);
        return 0;
    }

    int qtd = fread(vetor, sizeof(Alimento), max, arq);
    fclose(arq);
    return qtd;
}
