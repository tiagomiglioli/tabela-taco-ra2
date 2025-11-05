#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "alimento.h"

// Lê string com segurança (remove \n do fgets)
void lerString(char *str, int tamanho);

// Lê inteiro do teclado com validação
int ler_inteiro(void);

// Lê float com validação
float ler_float(void);

// Limpa o buffer do teclado
void limparBuffer(void);

// Carrega dados do arquivo binário para um vetor de Alimento
int carregar_binario(const char *nomeArquivo, Alimento *vetor, int max);

// Normaliza texto, removendo acentos e deixando tudo em minúsculas
void normalizarString(const char *entrada, char *saida);

#endif
