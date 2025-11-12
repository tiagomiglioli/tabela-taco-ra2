#include <stdio.h>
#include "alimento.h"
#include "utilidades.h"

int main()
{
    Alimento alimentos[MAX_ALIMENTOS];
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
