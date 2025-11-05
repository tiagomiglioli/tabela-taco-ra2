#include "utilidades.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Remove o \n ao final de uma string lida com fgets
void lerString(char *str, int tamanho)
{
    if (fgets(str, tamanho, stdin) != NULL)
    {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
            str[len - 1] = '\0';
    }
}

// Lê um inteiro do teclado
int ler_inteiro(void)
{
    int valor;
    scanf("%d", &valor);
    limparBuffer();
    return valor;
}

// Lê um float do teclado
float ler_float(void)
{
    float valor;
    scanf("%f", &valor);
    limparBuffer();
    return valor;
}

// Limpa o buffer do teclado para evitar problemas com fgets
void limparBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Carrega alimentos do arquivo binário para o vetor
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

// Remove acentos, substitui vírgulas e converte para minúsculas
void normalizarString(const char *entrada, char *saida)
{
    int i = 0, j = 0;

    while (entrada[i] != '\0')
    {
        unsigned char c = entrada[i];

        // Se for acento UTF-8
        if (c == 195 && entrada[i + 1] != '\0')
        {
            unsigned char prox = entrada[++i];
            switch (prox)
            {
            case 161:
            case 160:
            case 163:
            case 162:
                saida[j++] = 'a';
                break;
            case 169:
            case 170:
                saida[j++] = 'e';
                break;
            case 173:
                saida[j++] = 'i';
                break;
            case 179:
            case 181:
            case 180:
                saida[j++] = 'o';
                break;
            case 186:
                saida[j++] = 'u';
                break;
            case 167:
                saida[j++] = 'c';
                break;
            }
        }
        else if (c < 128)
        {
            // Substitui vírgulas por espaço
            if (c == ',' || c == '-')
                saida[j++] = ' ';
            else
                saida[j++] = tolower(c);
        }
        i++;
    }
    saida[j] = '\0';

    // Remover espaços extras no início e no fim
    i = 0;
    while (saida[i] == ' ')
        i++;
    memmove(saida, saida + i, strlen(saida + i) + 1);

    // Remover espaços duplicados no meio
    j = 0;
    for (i = 0; saida[i] != '\0'; i++)
    {
        if (!(saida[i] == ' ' && saida[i + 1] == ' '))
            saida[j++] = saida[i];
    }
    saida[j] = '\0';
}
