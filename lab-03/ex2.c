#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <sys/time.h> // Para gettimeofday
#include "matriz.h"


/*
 * Descrição: 
 * O código realiza o cálculo da média aritmética das linhas e da média geométrica 
 * das colunas de uma matriz, utilizando múltiplas threads para acelerar o processamento. 
 * O número máximo de threads simultâneas pode ser definido pelo usuário (1, 2, 4, 8 ou 16).
 * 
 * O programa permite ao usuário escolher entre gerar uma matriz aleatória ou 
 * ler uma matriz a partir de um arquivo. O tempo de execução total também é medido.
 * 
 * Autores: Daniel Victor Silva Bonfim, Elder Henrique Alves Correia, Winicius Abilio de Britto e
 * Yagho Junior Petini
 * Data de criação: 21/04/2025
 * Última atualização: 21/04/2025
 * 
 * Parâmetros de entrada:
 * - Número de threads: define o número de threads que podem executar simultaneamente.
 * - Tipo de entrada:
 *     * 1 - Gerar matriz aleatória
 *     * 2 - Ler matriz de um arquivo
 * - Dimensões da matriz (para entrada aleatória) ou nome do arquivo (para entrada via arquivo)
 * 
 * Parâmetros de saída:
 * - Imprime a matriz lida ou gerada
 * - Imprime a média aritmética de cada linha
 * - Imprime a média geométrica de cada coluna
 * - Tempo total de execução do cálculo com o número especificado de threads
 * 
 * Função principal:
 * - O programa principal controla a entrada de dados, a criação das threads e a execução das funções 
 *   que calculam as médias. Também realiza a medição de tempo e exibe os resultados.
 */
typedef struct {
    int** matrix;
    int index;
    int row;
    int col;
    double* result;
} ThreadData;

// Função para calcular média aritmética de uma linha
void* media_aritmetica_linha(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    double soma = 0;
    for (int j = 0; j < data->col; j++) {
        soma += data->matrix[data->index][j];
    }
    data->result[data->index] = soma / data->col;
    pthread_exit(NULL);
}

// Função para calcular média geométrica de uma coluna
void* media_geometrica_coluna(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    double produto = 1.0;
    for (int i = 0; i < data->row; i++) {
        int val = data->matrix[i][data->index];
        if (val <= 0) val = 1; // Evita log(0) e números negativos
        produto *= val;
    }
    data->result[data->index] = pow(produto, 1.0 / data->row);
    pthread_exit(NULL);
}

// Executa threads com um número máximo simultâneo
void executar_threads_limitadas(int total, int limite, pthread_t* threads, ThreadData* dados,
                                void* (*func)(void*)) {
    int i = 0;
    while (i < total) {
        int ativo = 0;
        while (ativo < limite && (i + ativo) < total) {
            pthread_create(&threads[i + ativo], NULL, func, &dados[i + ativo]);
            ativo++;
        }
        for (int j = 0; j < ativo; j++) {
            pthread_join(threads[i + j], NULL);
        }
        i += ativo;
    }
}

int main() {
    int row, col;
    int** matrix = NULL;
    int NUM_THREADS = 0;

    // 1. Pergunta o número de threads
    printf("Digite o número de threads (1, 2, 4, 8, 16): ");
    if (scanf("%d", &NUM_THREADS) != 1 || 
        (NUM_THREADS != 1 && NUM_THREADS != 2 && NUM_THREADS != 4 &&
         NUM_THREADS != 8 && NUM_THREADS != 16)) {
        fprintf(stderr, "Número de threads inválido.\n");
        return 1;
    }

    // 2. Pergunta o tipo de entrada
    int escolha;
    printf("Escolha o tipo de entrada:\n");
    printf("1 - Gerar matriz aleatória\n");
    printf("2 - Ler matriz de um arquivo\n");
    printf("Opção: ");
    if (scanf("%d", &escolha) != 1 || (escolha != 1 && escolha != 2)) {
        fprintf(stderr, "Opção inválida.\n");
        return 1;
    }

    if (escolha == 2) {
        char nome_arquivo[256];
        printf("Digite o nome do arquivo: ");
        scanf("%s", nome_arquivo);

        matrix = read_matrix_from_file(nome_arquivo, &row, &col);
        if (!matrix) {
            fprintf(stderr, "Erro ao ler a matriz do arquivo.\n");
            return 1;
        }
    } else {
        printf("Digite o número de colunas e linhas para a matriz aleatória (exemplo: 2 5): ");
        if (scanf("%d %d", &col, &row) != 2 || col <= 0 || row <= 0) {
            fprintf(stderr, "Entrada inválida.\n");
            return 1;
        }
        matrix = create_matrix(row, col);
        if (!matrix) {
            fprintf(stderr, "Erro ao criar a matriz.\n");
            return 1;
        }
        generate_elements(matrix, row, col, 100);
    }

    printf("\nMatriz:\n");
    print_matrix(matrix, row, col);

    // Alocação dinâmica
    pthread_t* threads_linhas = malloc(sizeof(pthread_t) * row);
    pthread_t* threads_colunas = malloc(sizeof(pthread_t) * col);
    ThreadData* dados_linhas = malloc(sizeof(ThreadData) * row);
    ThreadData* dados_colunas = malloc(sizeof(ThreadData) * col);
    double* medias_linhas = malloc(sizeof(double) * row);
    double* medias_colunas = malloc(sizeof(double) * col);

    if (!threads_linhas || !threads_colunas || !dados_linhas || !dados_colunas ||
        !medias_linhas || !medias_colunas) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 1;
    }

    for (int i = 0; i < row; i++) {
        dados_linhas[i] = (ThreadData){matrix, i, row, col, medias_linhas};
    }
    for (int j = 0; j < col; j++) {
        dados_colunas[j] = (ThreadData){matrix, j, row, col, medias_colunas};
    }

    // Medição de tempo
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    executar_threads_limitadas(row, NUM_THREADS, threads_linhas, dados_linhas, media_aritmetica_linha);
    executar_threads_limitadas(col, NUM_THREADS, threads_colunas, dados_colunas, media_geometrica_coluna);

    gettimeofday(&fim, NULL);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1e6;

    printf("\nMédia Aritmética das Linhas:\n");
    for (int i = 0; i < row; i++) {
        printf("Linha %d: %.2f\n", i, medias_linhas[i]);
    }

    printf("\nMédia Geométrica das Colunas:\n");
    for (int j = 0; j < col; j++) {
        printf("Coluna %d: %.2f\n", j, medias_colunas[j]);
    }

    printf("\nTempo de execução com %d threads: %.6f segundos\n", NUM_THREADS, tempo);

    // Liberação de memória
    free(threads_linhas);
    free(threads_colunas);
    free(dados_linhas);
    free(dados_colunas);
    free(medias_linhas);
    free(medias_colunas);
    for (int i = 0; i < row; i++) free(matrix[i]);
    free(matrix);

    return 0;
}
