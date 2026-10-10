#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 15   // maximo de letras da palavra

// limpa o que sobrou no teclado (pra nao travar quando digitam letra no lugar de numero)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// le um inteiro e so aceita se estiver entre min e max
int lerInteiro(char *mensagem, int min, int max) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= min && valor <= max) {
            limparBuffer();
            return valor;
        }
        limparBuffer();
        printf("Valor invalido! Digite um numero de %d ate %d.\n", min, max);
    }
}

// confere se a palavra tem so letras (sem acento) e no maximo 15
int palavraValida(char *palavra) {
    int i;
    if (strlen(palavra) == 0 || strlen(palavra) > MAX) {
        return 0;
    }
    for (i = 0; palavra[i] != '\0'; i++) {
        if (!isalpha((unsigned char) palavra[i])) {
            return 0;
        }
    }
    return 1;
}

// le a palavra e ja converte tudo pra minusculo
void lerPalavra(char *palavra) {
    char texto[100];
    int i;
    while (1) {
        printf("Digite a palavra secreta (ate %d letras, sem acento): ", MAX);
        scanf("%99s", texto);
        limparBuffer();
        if (palavraValida(texto)) {
            for (i = 0; texto[i] != '\0'; i++) {
                palavra[i] = tolower((unsigned char) texto[i]);
            }
            palavra[i] = '\0';
            return;
        }
        printf("Palavra invalida! Use so letras, sem acento, de 1 a %d letras.\n", MAX);
    }
}

int ehPrimo(long long n) {
    long long d;
    if (n < 2) {
        return 0;
    }
    for (d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * Preenche o vetor seq com os n primeiros termos.
 * tipo: 1 = PA, 2 = PG, 3 = Fibonacci, 4 = Primos
 * a1 e r so servem pra PA (a1 e razao r) e PG (a1 e razao q)
 */
void gerarSequencia(int tipo, int n, long long a1, long long r, long long *seq) {
    int i;
    long long candidato = 2;

    for (i = 0; i < n; i++) {
        if (tipo == 1) {
            seq[i] = a1 + i * r;            // PA: an = a1 + (n-1)*r
        } else if (tipo == 2) {
            if (i == 0) {
                seq[i] = a1;
            } else {
                seq[i] = seq[i - 1] * r;    // PG: cada termo vezes a razao
            }
        } else if (tipo == 3) {
            if (i < 2) {
                seq[i] = 1;
            } else {
                seq[i] = seq[i - 1] + seq[i - 2];   // Fibonacci
            }
        } else {
            while (!ehPrimo(candidato)) {
                candidato++;
            }
            seq[i] = candidato;
            candidato++;
        }
    }
}

char *nomeSequencia(int tipo) {
    if (tipo == 1) return "PA";
    if (tipo == 2) return "PG";
    if (tipo == 3) return "Fibonacci";
    return "Primos";
}

/*
 * Faz as duas camadas. Se log for NULL nao escreve no log
 * (usei isso na comparacao pra nao encher o arquivo).
 */
void criptografar(char *palavra, int shift, long long *seq, char *resultado, FILE *log) {
    int i;
    int original, camada1, camada2;

    for (i = 0; palavra[i] != '\0'; i++) {
        original = palavra[i] - 'a';                          // a = 0, b = 1, ...
        camada1 = (original + shift) % 26;                    // Cesar
        camada2 = (camada1 + (int)(seq[i] % 26)) % 26;        // sequencia
        resultado[i] = 'a' + camada2;

        if (log != NULL) {
            fprintf(log, "  letra %d: '%c' | sequencia: %lld | camada 1: '%c' | camada 2: '%c'\n",
                    i + 1, palavra[i], seq[i], 'a' + camada1, resultado[i]);
        }
    }
    resultado[i] = '\0';
}

// escreve a data e hora no log
void escreverData(FILE *log) {
    time_t agora = time(NULL);
    fprintf(log, "Data/hora: %s", ctime(&agora));
}

void criptografarPalavra() {
    char palavra[MAX + 1];
    char codificada[MAX + 1];
    long long seq[MAX];
    long long a1 = 1, razao = 1;
    int shift, tipo, n, i;
    FILE *arq, *log;

    lerPalavra(palavra);
    n = strlen(palavra);
    shift = lerInteiro("Digite o SHIFT: ", 0, 25);

    printf("\n1 - PA\n2 - PG\n3 - Fibonacci\n4 - Primos\n");
    tipo = lerInteiro("Escolha a sequencia: ", 1, 4);

    if (tipo == 1) {
        a1 = lerInteiro("Primeiro termo (0 a 100): ", 0, 100);
        razao = lerInteiro("Razao (0 a 100): ", 0, 100);
    } else if (tipo == 2) {
        a1 = lerInteiro("Primeiro termo (1 a 10): ", 1, 10);
        razao = lerInteiro("Razao (1 a 10): ", 1, 10);
    }

    gerarSequencia(tipo, n, a1, razao, seq);

    log = fopen("log_execucao.txt", "a");
    if (log == NULL) {
        printf("Erro ao abrir o arquivo de log!\n");
        return;
    }

    fprintf(log, "===== Criptografia =====\n");
    escreverData(log);
    fprintf(log, "Palavra: %s | SHIFT: %d | Sequencia: %s | Letras: %d\n",
            palavra, shift, nomeSequencia(tipo), n);
    fprintf(log, "Termos da sequencia:");
    for (i = 0; i < n; i++) {
        fprintf(log, " %lld", seq[i]);
    }
    fprintf(log, "\n");

    criptografar(palavra, shift, seq, codificada, log);
    fprintf(log, "Palavra criptografada: %s\n\n", codificada);
    fclose(log);

    arq = fopen("resultado_criptografia.txt", "w");
    if (arq == NULL) {
        printf("Erro ao criar o arquivo de resultado!\n");
        return;
    }
    fprintf(arq, "Palavra codificada: %s | SHIFT: %d | Tipo: %d | Letras: %d\n",
            codificada, shift, tipo, n);
    fclose(arq);

    printf("\nSequencia usada:");
    for (i = 0; i < n; i++) {
        printf(" %lld", seq[i]);
    }
    printf("\nPalavra criptografada: %s\n", codificada);
    printf("Resultado salvo em resultado_criptografia.txt\n");
}

// mostra a mesma palavra criptografada com cada sequencia (PA com 1 e 1, PG com 1 e 2)
void compararSequencias() {
    char palavra[MAX + 1];
    char codificada[MAX + 1];
    long long seq[MAX];
    int shift, tipo, n;
    FILE *log;

    lerPalavra(palavra);
    n = strlen(palavra);
    shift = lerInteiro("Digite o SHIFT: ", 0, 25);

    log = fopen("log_execucao.txt", "a");
    if (log != NULL) {
        fprintf(log, "===== Comparacao =====\n");
        escreverData(log);
        fprintf(log, "Palavra: %s | SHIFT: %d\n", palavra, shift);
    }

    printf("\nPalavra: %s | SHIFT: %d\n", palavra, shift);
    for (tipo = 1; tipo <= 4; tipo++) {
        if (tipo == 2) {
            gerarSequencia(tipo, n, 1, 2, seq);
        } else {
            gerarSequencia(tipo, n, 1, 1, seq);
        }
        criptografar(palavra, shift, seq, codificada, NULL);
        printf("%-10s -> %s\n", nomeSequencia(tipo), codificada);
        if (log != NULL) {
            fprintf(log, "  %-10s -> %s\n", nomeSequencia(tipo), codificada);
        }
    }

    if (log != NULL) {
        fprintf(log, "\n");
        fclose(log);
    }
}

int main() {
    int opcao;

    do {
        printf("\n===== CRIPTOGRAFIA =====\n");
        printf("1 - Criptografar palavra\n");
        printf("2 - Comparar sequencias\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Opcao: ", 0, 2);

        switch (opcao) {
            case 1:
                criptografarPalavra();
                break;
            case 2:
                compararSequencias();
                break;
        }
    } while (opcao != 0);

    printf("Programa encerrado.\n");
    return 0;
}
