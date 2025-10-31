#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_alimentos 100
#define max_texto 100

void clear()
{
#ifdef _WIN32
    system("cls"); // Comando do Windows
#else
    system("clear"); // Comando do Linux e macOS
#endif
}

// Definição do Enum para as categorias
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

// Função para converter o número em categoria (string)
const char *categoria_para_str(Categoria categoria)
{
    switch (categoria)
    {
    case CARNES_DERIVADOS:
        return "Carnes e derivados";
    case CEREAIS_DERIVADOS:
        return "Cereais e derivados";
    case GORDURAS_OLEOS:
        return "Gorduras e óleos";
    case LEGUMINOSAS_DERIVADOS:
        return "Leguminosas e derivados";
    case LEITE_DERIVADOS:
        return "Leite e derivados";
    case MISCELEANEAS:
        return "Miscelâneas";
    case NOZES_SEMENTES:
        return "Nozes e sementes";
    case PESCADOS_MAR:
        return "Pescados e frutos do mar";
    case PRODUTOS_ACUCARADOS:
        return "Produtos açucarados";
    case VERDURAS_HORTALICAS:
        return "Verduras, hortaliças e derivados";
    default:
        return "Categoria inválida";
    }
}

typedef struct
{
    int numero_alimento;
    char descricao_alimento[max_texto];
    float umidade_percent;
    float energia_kcal;
    float proteina_g;
    float carboidrato_g;
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
            a.umidade_percent = atof(token);
            break;
        case 3:
            a.energia_kcal = atof(token);
            break;
        case 4:
            a.proteina_g = atof(token);
            break;
        case 5:
            a.carboidrato_g = atof(token);
            break;
        case 6:
            strncpy(a.categoria, token, max_texto);
            a.categoria[max_texto - 1] = '\0'; // coloca o /0
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

int pedir_n(int total_elementos)
{
    int n;
    do
    {
        printf("Digite o valor N(max: %d): ", total_elementos);
        scanf("%d", &n);
        getchar();

        if (n <= 0 || n > total_elementos)
        {
            printf("Valor inválido, o N deve ser entre 1 e %d.\n", total_elementos);
        }
    } while (n <= 0 || n > total_elementos);
    return n;
}
// funções para passar para os usuarios vou fzr um esqueletinho segundo o padrão q coloquei no comentário da main

void opt_a(Alimento *vetor, int size)
{
    printf("Todas as categorias: \n\n");
    for (int i = 1; i < 10; i++)
    {
        printf("Categoria %s \n", categoria_para_str(i));
        printf("----------------------------------------------------------------------------\n");
    }

    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_b(Alimento *vetor, int size)
{
    printf("Categorias disponíveis:\n");
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    int numero_categoria;
    // pedir ao user o num da categoria
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    while (getchar() != '\n')
        ; // limpa o teclado removendo o que sobrou da última leitura.

    // trans em str
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0';

    Alimento copia_vetor[size];
    for (int i = 0; i < size; i++)
    {
        strcpy(copia_vetor[i].categoria, vetor[i].categoria);
        strcpy(copia_vetor[i].descricao_alimento, vetor[i].descricao_alimento);
    }
    // bubble sort pra deixar em ordem alfabética
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - 1 - i; j++)
        {
            if (strcmp(copia_vetor[j].descricao_alimento, copia_vetor[j + 1].descricao_alimento) > 0)
            {
                // Trocar as posições de descrição e categoria pra n embaralhar td
                Alimento temp = copia_vetor[j];
                copia_vetor[j] = copia_vetor[j + 1];
                copia_vetor[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, copia_vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0; // rm o '\n' do csv

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            printf("Categoria: %s\n", copia_vetor[i].categoria);
            printf("Descrição: %s\n", copia_vetor[i].descricao_alimento);
            printf("------------------------------------------------------------\n");
        }
    }
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_c(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }

    int numero_categoria;

    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    getchar();

    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0';

    Alimento *filtrado[size];
    int count = 0;

    // Filtrar apenas os alimentos da categoria escolhida
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0; // remove \n do CSV

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // Ordenar em ordem DECRESCENTE por energia_kcal
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (filtrado[j]->energia_kcal < filtrado[j + 1]->energia_kcal)
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    printf("\nAlimentos da categoria '%s' em ordem decrescente de capacidade energética:\n", categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%s — Energia: %.2f kcal\n", filtrado[i]->descricao_alimento, filtrado[i]->energia_kcal);
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_d(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // vetor de ponteiros  filtrando com as categorias
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort decrescente
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (filtrado[j]->umidade_percent < filtrado[j + 1]->umidade_percent)
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    //  caso user digite um num maior do q os disponíveis
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior umidade:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s — Umidade: %.2f%%\n", filtrado[i]->descricao_alimento, filtrado[i]->umidade_percent);
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_e(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // criar vetor filtrado
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort pela energia
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (filtrado[j]->energia_kcal < filtrado[j + 1]->energia_kcal)
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    // caso o user forneca um num maior, msm coisa do ultimo
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior capacidade energética:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s - Capacidade energética: %.2f Kcal\n", filtrado[i]->descricao_alimento, filtrado[i]->energia_kcal);
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_f(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // criar vetor filtrado
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort pela proteina
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (filtrado[j]->proteina_g < filtrado[j + 1]->proteina_g)
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    // caso o user forneca um num maior, msm coisa do ultimo
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior quantidade de proteina:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s - quantidade de proteina: %.2f g \n", filtrado[i]->descricao_alimento, filtrado[i]->proteina_g);
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_g(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // criar vetor filtrado
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort pelo carboidrato
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (filtrado[j]->carboidrato_g < filtrado[j + 1]->carboidrato_g)
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    // caso o user forneca um num maior, msm coisa do ultimo
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior quantidade de carboidratos:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s - carboidratos: %.2f g\n", filtrado[i]->descricao_alimento, filtrado[i]->carboidrato_g);
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_h(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // criar vetor filtrado
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort pela relação de energia e proteína
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if ((filtrado[j]->energia_kcal) / (filtrado[j]->proteina_g) < (filtrado[j + 1]->energia_kcal) / (filtrado[j + 1]->proteina_g))
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    // caso o user forneca um num maior, msm coisa do ultimo
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior relação de energia e proteína:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s - relação de energia e proteína: %.2f Kcal/g \n", filtrado[i]->descricao_alimento, (filtrado[i]->energia_kcal) / (filtrado[i]->proteina_g));
    }

    printf("------------------------------------------------------------\n");

    printf("Pressione enter para continuar\n");
    getchar();
}

void opt_i(Alimento *vetor, int size)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - %s\n", i, categoria_para_str(i));
    }
    // input do usuario da categoria
    int numero_categoria;
    printf("Digite um número de 1 a 10 para obter a categoria do alimento: ");
    scanf("%d", &numero_categoria);
    // transformar na str pra comparar com a str do vetor
    char categoria_usuario[50];
    strncpy(categoria_usuario, categoria_para_str(numero_categoria), sizeof(categoria_usuario) - 1);
    categoria_usuario[sizeof(categoria_usuario) - 1] = '\0'; // tira o \n
    // pedir um n p/ o user
    int n_usuario;
    n_usuario = pedir_n(size);

    // criar vetor filtrado
    Alimento *filtrado[size];
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        char categoria_temp[50];
        strcpy(categoria_temp, vetor[i].categoria);
        categoria_temp[strcspn(categoria_temp, "\n")] = 0;

        if (strcmp(categoria_temp, categoria_usuario) == 0)
        {
            filtrado[count++] = &vetor[i];
        }
    }

    // sort pela relação de energia e carbo
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if ((filtrado[j]->energia_kcal) / (filtrado[j]->carboidrato_g) < (filtrado[j + 1]->energia_kcal) / (filtrado[j + 1]->carboidrato_g))
            {
                Alimento *temp = filtrado[j];
                filtrado[j] = filtrado[j + 1];
                filtrado[j + 1] = temp;
            }
        }
    }

    // caso o user forneca um num maior, msm coisa do ultimo
    if (n_usuario > count)
    {
        n_usuario = count;
    }
    // print
    printf("\nTop %d alimentos da categoria '%s' com maior relação de energia e carboidrato:\n", n_usuario, categoria_usuario);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n_usuario; i++)
    {
        printf("%s - relação de energia e carboidrato: %.2f Kcal/g \n", filtrado[i]->descricao_alimento, (filtrado[i]->energia_kcal) / (filtrado[i]->carboidrato_g));
    }

    printf("------------------------------------------------------------\n");
    printf("Pressione enter para continuar\n");
    getchar();
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    Alimento vetor[100];
    int qtd = ler_csv(vetor);

    if (qtd == 0)
    {
        printf("Erro, não foi carregado.");
        // coloquei um return 0 pra encerrar o programa caso ocorra um erro
        return 0;
    }
    printf("Quantidade de alimentos = %d", qtd);

    // vai ser char pra bater mlr com o txt
    char opcao;

    do
    {
        printf("\n------------------------------------------------------------\n");
        printf("Menu de Opções:\n");
        printf("------------------------------------------------------------\n");
        printf("a. Liste todas as categorias de alimentos.\n");
        printf("b. Liste todos os alimentos de certa categoria selecionada pelo usuário, em ordem alfabética de acordo com a descrição do alimento.\n");
        printf("c. Liste todos os alimentos de certa categoria selecionada pelo usuário, em ordem decrescente com respeito à capacidade energética dos alimentos.\n");
        printf("d. Listar os N alimentos de certa categoria com maior percentual de umidade, em ordem decrescente com respeito a esse percentual, sendo a categoria e N definidos pelo usuário.\n");
        printf("e. Listar os N alimentos de certa categoria com maior capacidade energética, em ordem decrescente com respeito a essa capacidade, sendo a categoria e N definidos pelo usuário.\n");
        printf("f. Listar os N alimentos de certa categoria com maior quantidade de proteína, em ordem decrescente com respeito a essa quantidade, sendo a categoria e N definidos pelo usuário.\n");
        printf("g. Listar os N alimentos de certa categoria com maior quantidade de carboidrato, em ordem decrescente com respeito a essa quantidade, sendo a categoria e N definidos pelo usuário.\n");
        printf("h. Listar os N alimentos de certa categoria que possuam a relação mais alta entre energia e proteína, em ordem decrescente com respeito a essa relação, sendo a categoria e N definidos pelo usuário.\n");
        printf("i. Listar os N alimentos de certa categoria que possuam a relação mais alta entre energia e carboidrato, em ordem decrescente com respeito a essa relação, sendo a categoria e N definidos pelo usuário.\n");
        printf("j. Encerrar o programa.\n");
        printf("------------------------------------------------------------\n");
        printf("Digite o caracter de 'a' a 'j' e pressione enter: ");
        opcao = getchar();
        getchar();
        /*por mim a pode seguir o seguinte formato:
        o switch e a resposta chama a função, seguindo um padrão tipo opt de option _ a letra. (ex: opt_a )
        john days*/
        switch (opcao)
        {
        case 'a':
            opt_a(vetor, 100);
            break;
        case 'b':
            clear();
            opt_b(vetor, 100);
            break;
        case 'c':
            clear();
            opt_c(vetor, 100);
            break;
        case 'd':
            clear();
            opt_d(vetor, 100);
            break;
        case 'e':
            clear();
            opt_e(vetor, 100);
            break;
        case 'f':
            clear();
            opt_f(vetor, 100);
            break;
        case 'g':
            clear();
            opt_g(vetor, 100);
            break;
        case 'h':
            clear();
            opt_h(vetor, 100);
            break;
        case 'i':
            clear();
            opt_i(vetor, 100);
            break;
        case 'j':
            clear();
            printf("saindo. . . \n");
            break;
        default:
            printf("opcao invalida, pressione enter\n");
            while (getchar() != '\n')
                ;
        }

    } while (opcao != 'j');

    return 0;
}