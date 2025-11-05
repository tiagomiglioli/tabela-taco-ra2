#ifndef ALIMENTO_H
#define ALIMENTO_H

#define MAX_TEXTO 100
#define MAX_ALIMENTOS 600

typedef struct
{
    int numero_alimento;
    char descricao_alimento[MAX_TEXTO];
    float energia_kcal;
    float proteina_g;
    char categoria[MAX_TEXTO];
} Alimento;

// Lê o arquivo taco.csv e preenche um vetor de Alimento
int ler_csv(Alimento *alimentos);

// Salva o vetor de Alimento no arquivo dados.bin em formato binário
void gravar_binario(Alimento *alimentos, int qtd);

#endif
