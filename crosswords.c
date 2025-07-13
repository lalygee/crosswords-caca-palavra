#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 50
#define TAM_PALAVRA 50

void PegarDimensoes(const char* matrix_txt, int* m, int* n) {

    // verifica e abre o arquivo 
    FILE* matrix = fopen(matrix_txt, "r");
    if (!matrix) {
        printf("Erro ao abrir o arquivo!\n");
        *m = 0;
        *n = 0;
        return;
    }

    char buffer[1000];
    *m = 0;
    *n = 0;

    // le a primera linha e acha o n (colunas)
    if (fgets(buffer, sizeof(buffer), matrix) != NULL) {
        for (int i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++) {
            (*n)++;
        }
        *m = 1; 
    }

    // contar as linhas restantes
    while (fgets(buffer, sizeof(buffer), matrix) != NULL) {
        (*m)++;
    }

    fclose(matrix);
}

void LerMatrix(const char* matrix_txt, int m, int n, char matrix[m][n]) {

    // le arquivo
    FILE* matrixtxt = fopen(matrix_txt, "r");
    if (!matrixtxt) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char buffer[1000];

    for (int i = 0; i < m; i++) {
        if (fgets(buffer, sizeof(buffer), matrixtxt) != NULL) {
            // remove \n
            buffer[strcspn(buffer, "\n")] = '\0';
                // copia buffer - matrix
                for (int j = 0; j < n; j++) {
                    matrix[i][j] = buffer[j];
                }
        }
    }

    fclose(matrixtxt);
}

int LerPalavras(char palavras[MAX_PALAVRAS][TAM_PALAVRA]) {

    // le arquivo
    FILE *palavraslista = fopen("palavras.txt", "r");
    if (!palavraslista) {
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    char bufferpalavras[100];
    int total = 0;

    // loop para ler as linhas 
    while (fgets(bufferpalavras, 100, palavraslista) != NULL && total < MAX_PALAVRAS) {
        // remove \n e coloca \0
        bufferpalavras[strcspn(bufferpalavras, "\n")] = '\0';

        // coloca as letras em uppercase (função toupper)
        for (int i = 0; bufferpalavras[i] != '\0'; i++) {
            bufferpalavras[i] = toupper(bufferpalavras[i]);
        }

        //copia palavra sem o \n e em uppercase para o array palavras
        strcpy(palavras[total], bufferpalavras);
        total++;
    }

    fclose(palavraslista);
    return total;
}

int ChecarPalavra(int m, int n, char matrix[m][n], int x, int y, char* palavra, int direcao) {

    // Pega o tamanho da palavra
    int tam = strlen(palavra);

    // vetores para todas as direções
    int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

    // loop para cada letra da palavra
    for (int i = 0; i < tam; i++) {
        // próxima posição
        int nx = x + i * dx[direcao];
        int ny = y + i * dy[direcao];

        // verifica se passou dos limites da matrix
        if (nx < 0 || ny < 0 || nx >= m || ny >= n) {
            return 0;
        }

        // compara letra da matrix com a letra da palavra
        if (toupper(matrix[nx][ny]) != palavra[i]) {
            return 0;
        }
    }

    return 1;
}

int ContarOcorrencias(int m, int n, char matrix[m][n], char* palavra) {

    int count = 0;

    // para cada linha
    for (int i = 0; i < m; i++) {
        // para cada coluna
        for (int j = 0; j < n; j++) {
            // para cada uma das 8 direções
            for (int direcao = 0; direcao < 8; direcao++) {
                // se a palavra for encontrada, aumenta o contador
                if (ChecarPalavra(m, n, matrix, i, j, palavra, direcao)) {
                    count++;
                }
            }
        }
    }
    return count;
}

void Resultado(const char* resultado_txt, int TotalPalavras, char palavras[][50], int m, int n, char matrix[m][n]) {

    int ocorrencias[TotalPalavras];

    // Conta ocorrências
    for (int i = 0; i < TotalPalavras; i++) {
        ocorrencias[i] = ContarOcorrencias(m, n, matrix, palavras[i]);
    }

    // Ordena em ordem decrescente de ocorrências (alfabética em empate)
    for (int i = 0; i < TotalPalavras - 1; i++) {
        for (int j = i + 1; j < TotalPalavras; j++) {
            if (ocorrencias[i] < ocorrencias[j] ||
                (ocorrencias[i] == ocorrencias[j] && strcmp(palavras[i], palavras[j]) > 0)) {

                // troca ocorrencias
                int temp = ocorrencias[i];
                ocorrencias[i] = ocorrencias[j];
                ocorrencias[j] = temp;

                // troca palavras
                char temp_palavra[50];
                strcpy(temp_palavra, palavras[i]);
                strcpy(palavras[i], palavras[j]);
                strcpy(palavras[j], temp_palavra);
            }
        }
    }

    // Grava em resultado.txt
    FILE *saida = fopen(resultado_txt, "w");
    if (!saida) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // printa em resultados.txt e na tela
    for (int i = 0; i < TotalPalavras; i++) {
        fprintf(saida, "%s, %d\n", palavras[i], ocorrencias[i]);
        printf("%s, %d\n", palavras[i], ocorrencias[i]);
    }

    fclose(saida);
}

int main() {

    int m, n;

    PegarDimensoes("matrix.txt", &m, &n);

    char matrix[m][n];
   
    LerMatrix("matrix.txt", m, n, matrix);

    char palavras[MAX_PALAVRAS][TAM_PALAVRA];

    int TotalPalavras = LerPalavras(palavras);

    Resultado("resultado.txt", TotalPalavras, palavras, m, n, matrix);

    return 0;
}
