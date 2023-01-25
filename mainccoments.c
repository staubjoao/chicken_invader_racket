#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// função que retorna um número aleatorio no intervalo min e max
int aleatorio(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

// função para imprimir o tabuleiro
void imprimir_tabuleiro(int tabuleiro[9][9])
{
    int i, j, aux;
    // primeiro imprime o cabeçalho do tabuleiro, as colunas (j)
    printf("   | 1  2  3 | 4  5  6 | 7  8  9 | j\n");
    printf("----------------------------------\n");
    for (i = 0, aux = 1; i < 9; i++, aux++)
    {
        for (j = 0; j < 9; j++)
        {
            // imprime as linhas (i)
            if (j == 0)
                printf("%d  |", aux);
            // caso o resto da divisão de j por 3 seja igual a 0 imprime um pipe (|)
            else if (j % 3 == 0)
                printf("|");
            // se a lacula estiver vazia (igual a 0) imprime um _
            if (tabuleiro[i][j] == 0)
                printf(" _ ");
            // caso contrario imprime o valor
            else
                printf(" %d ", tabuleiro[i][j]);
            // ao final de cada linhas imprime um pipe
            if (j == 9 - 1)
                printf("|");
        }
        // quebra a linha
        printf("\n");
        // imprime uma linha que divide os blocos (ou quadrantes)
        if (aux % 3 == 0)
            printf("----------------------------------\n");
    }
    // imprime a represetação das linhas e uma quebra de linha
    printf("i\n");
}

// função para verificar se uma jogada é valida
int verifica_jogada(int tabuleiro[9][9], int linha, int coluna, int val)
{
    int i, j;
    int quadrante_linha, quadrante_coluna;

    // primeiro verifica a coluna
    for (i = 0; i < 9; i++)
    {
        // a partir da coluna que vem como parametro é analisado todas as linhas daquela mesma coluna
        // caso algum valor naquela coluna seja igual ao valor da jogada retorna 0
        if (tabuleiro[i][coluna] == val)
            return 0;
    }

    for (j = 0; j < 9; j++)
    {
        // a partir da linha que vem como parametro é analisado todas as colunas daquela mesma linha
        // caso algum valor naquela linha seja igual ao valor da jogada retorna 0
        if (tabuleiro[linha][j] == val)
            return 0;
    }

    // calcula o quadrante linha e coluna que o quadrante se inicia, por exemplo,
    // dado a linha 5 coluna 4 temos que 5 % 3 = 2 + 5 = 7 e 4 % 3 = 1 + 4 = 5
    // assim esse quadrante começa na linha 7 coluna 5.
    quadrante_linha = linha - (linha % 3);
    quadrante_coluna = coluna - (coluna % 3);

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            // vamos verifica o quadrante 3x3, caso algum valor dentro desse quadrante caso
            // algum valor nesse quadrante seja igual ao valor da jogada retorna 0
            if (tabuleiro[quadrante_linha + i][quadrante_coluna + j] == val)
                return 0;
        }
    }
    // se não retornar 0 em qualquer validação retorna 1, significa que a jogada é valida
    return 1;
}

// função feita para verificar quadrante, é igual a função interna do verifica jogada
// só que exclusiva para o quadrante
int verifica_quadrante(int tabuleiro[9][9], int linha, int coluna, int val)
{
    int i, j, quadrante_linha, quadrante_coluna;
    quadrante_linha = linha - linha % 3;
    quadrante_coluna = coluna - coluna % 3;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (tabuleiro[quadrante_linha + i][quadrante_coluna + j] == val)
                return 0;
        }
    }
    return 1;
}

// função para gerar os 3 quadrantes da diagonal principal do tabuleiro
void gerar_quadrante(int tabuleiro[9][9], int linha, int coluna)
{
    int valor, i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            do
            {
                // regramos um número aleatorio de 1 a 9 até que ele satisfaça a condição do quadrante
                valor = aleatorio(1, 9);
            } while (!verifica_quadrante(tabuleiro, linha, coluna, valor));
            // quando ele satisfazer atribuimos ao tabuleiro esse valor
            tabuleiro[linha + i][coluna + j] = valor;
        }
    }
}

// função para gerar um número aleatorio na diagonal principal
void gerar_diagonal(int tabuleiro[9][9])
{
    int i;
    for (i = 0; i < 9; i += 3)
        gerar_quadrante(tabuleiro, i, i);
}

// função para preencher o restante do tabuleiro
int gerar_restante(int tabuleiro[9][9], int i, int j)
{
    if (j >= 9 && i < 8)
        // caso o o paremetro j é maior ou igual a 9 e i < 8
        i++, j = 0; // j recebe 0 e i recebe o seu valor mais i
    if (i >= 9 && j >= 9)
        return 1; // quando i e j atingitem o valor 9 a função já gerou todo tabuleiro
    if (i < 3)
    {
        // se i é menor do que 3
        if (j < 3)
            // se j é menor do que 3
            j = 3; // j recebe 3
    }
    else if (i < 9 - 3)
    {
        // não se i é menor do que 9 - 3
        if (j == (int)(i / 3) * 3)
            // se j é igual a (i / 3) * 3
            j += 3; // j recebe o seu valor mais 3
    }
    else
    {
        // se não
        if (j == 6)
        {
            // se j é igual a 6
            i++;   // i recebe i mais 1
            j = 0; // j recebe 0
            if (i >= 9)
                // se i for maior ou igual a 9
                return 1; // retorna 1, finalizado
        }
    }

    for (int valor = 1; valor <= 9; valor++)
    {
        // para cada valor que pode ser jogado, é verificado se a jogda é valida
        if (verifica_jogada(tabuleiro, i, j, valor))
        {
            tabuleiro[i][j] = valor;                 // se sim declara ela no tabuleiro
            if (gerar_restante(tabuleiro, i, j + 1)) // faz a chamada da função para a proxima coluna
                return 1;                            // caso a cahamda retorne 1 (verdadeiro), retorna 1, finalizado

            tabuleiro[i][j] = 0; // caso não retorne 1 a casa em questão recebe 0
        }
    }
    return 0; // retorna 0 caso o tabuleiro não tenha solução
}

// função para remover uma quantidade de digitos
void remove_digito(int tabuleiro[9][9], int qtd)
{
    int cont = qtd;  // cont recebe a quantidade a ser removida (qtd, vem como parametro)
    while (cont > 0) // enquando cont for maior do que
    {
        int i = aleatorio(0, 8); // gera um número aleatorio para i
        int j = aleatorio(0, 8); // gera um número aleatorio para j

        if (tabuleiro[i][j] != 0)
        {
            // se o valor da linhas e coluna em questão for diferente de 0
            cont--;              // a quantidade a ser removida decrementa (cont recebe cont - 1)
            tabuleiro[i][j] = 0; // e declaramos o valor 0 para a coordenada em questão
        }
        // caso seja falso gera um novo valor para i e j
    }
}

// função para gerar o tabuleiro
void gerar_tabuleiro(int tabuleiro[9][9], int qtd)
{
    // essa função utiliza da seuinte logica
    // primeiro geramos a diagonal principal do tabuleiro 9x9,
    // já que na diagonal principal podemos simplesmente gerar números
    // aleatorios de 1 a 9 sem se preocupar com a validação
    // em segundo geramos os quadrandes 3x3 da diagonal principal
    // sempre validando se o valor não conflita com a condição do quadrante
    gerar_diagonal(tabuleiro);

    // em terceiro geramos o restante do tabuleiro conferindo as linhas, colunas e quadrantes
    gerar_restante(tabuleiro, 0, 3);

    // e em quarto pegamos o tabuleiro completo e removemos uma quantidade x de números aleatorios desse tabuleiro
    remove_digito(tabuleiro, qtd);
}

// inicializa todas as casas do tabuleiro como 0
void inicializa_tabuleiro(int tabuleiro[9][9])
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
            tabuleiro[i][j] = 0;
    }
}

int main()
{
    srand(time(0));

    setlocale(LC_ALL, "Portuguese_Brasil");

    int jogadai, jogadaj, jogadanum, dificuldade, cont;
    int tabuleiro[9][9];
    inicializa_tabuleiro(tabuleiro); // inicia o tabuleiro com valores 0

    do
    {
        printf("Selecione a dificuldade do tabuleiro\n1-Fácil\n2-Médio\n3-Difícil\nDigite a dificuldade: ");
        scanf("%d", &dificuldade);
        switch (dificuldade)
        {
        case 1:
            gerar_tabuleiro(tabuleiro, 20);
            cont = 20; // cont vai ser a quantidade de acertos que o jogador deve ter
            break;
        case 2:
            gerar_tabuleiro(tabuleiro, 50);
            cont = 50;
            break;
        case 3:
            gerar_tabuleiro(tabuleiro, 65);
            cont = 65;
            break;
        default:
            printf("\nDificuldade invalida!\n");
            break;
        }
    } while (dificuldade < 1 || dificuldade > 3);

    while (cont > 0) // enquanto cont for maior do que 0 execute
    {
        imprimir_tabuleiro(tabuleiro);
        do
        {
            printf("Digite as coordenadas da jogada (i j): ");
            scanf("%d %d", &jogadai, &jogadaj); // faz a leitura da jogada, coordenadas i e j
            // e enquanto essas coordenadas forem maior do 9 ou menor do que 1 ou o valor dela no tabuleiro seja diferente de 0
            // solicita outra coordenada ao jogador
        } while ((jogadai > 9) || (jogadai < 1) || (jogadaj > 9) || (jogadaj < 1) || (tabuleiro[jogadai - 1][jogadaj - 1] != 0));
        do
        {
            printf("Digite o número a ser jogado (1 a 9): ");
            scanf("%d", &jogadanum); // faz a leitura do valor da jogada
            // e enquanto a jogada for maior do que 9 ou menor do que 1 solicita outro valor
        } while ((jogadanum > 9) || (jogadanum < 1));

        if (verifica_jogada(tabuleiro, jogadai - 1, jogadaj - 1, jogadanum))
        {
            // se a jogada for valida, não conflita com nenhum outro número no quadrante, linha ou colunas
            // avisa ao jogador que a jogda é valida e decrementa o valor de lacunas vazias
            printf("\nJogada valida\n");
            tabuleiro[jogadai - 1][jogadaj - 1] = jogadanum;
            cont--;
        }
        else
            // se não avisa ao jogador que a jogada é invalida
            printf("\nJogada invalida\n");
    }
    // ao fim, apois preencher todas as lacunas vazias imprime o tabuleiro final e parabeniza o jogador pela vitoria
    imprimir_tabuleiro(tabuleiro);
    printf("\nParabéns, você finalizou o jogo!\n");

    return 0;
}