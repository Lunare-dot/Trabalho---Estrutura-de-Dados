#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "funcoes.h"

char *extrair_campo_csv(char **linha_ptr) {
    char *inicio = *linha_ptr;
    char *ptr = inicio;
    bool dentro_aspas = false;

    while (*ptr) {
        if (*ptr == '"') {
            dentro_aspas = !dentro_aspas;
        } else if (*ptr == ',' && !dentro_aspas) {
            break;
        }
        ptr++;
    }

    if (*ptr == ',') {
        *ptr = '\0';
        *linha_ptr = ptr + 1;
    } else {
        *linha_ptr = ptr;
    }

    if (inicio[0] == '"' && ptr[-1] == '"') {
        inicio++;
        ptr[-1] = '\0';
    }

    return inicio;
}

processo *importar_csv(char *Arquivo) {
    FILE *fp = fopen(Arquivo, "r");
    if (!fp) return NULL;

    processo *V = malloc(QUANTIDADE_DE_LINHAS * sizeof(processo));
    char linha[1024];
    fgets(linha, sizeof(linha), fp); // pula cabeçalho

    for (int i = 0; i < QUANTIDADE_DE_LINHAS; i++) {
        if (fgets(linha, sizeof(linha), fp) == NULL) break;

        char *ptr = linha;

        char *token = extrair_campo_csv(&ptr);
        V[i].id = atol(token);

        token = extrair_campo_csv(&ptr);
        strncpy(V[i].numero, token, sizeof(V[i].numero));
        V[i].numero[sizeof(V[i].numero) - 1] = '\0';

        token = extrair_campo_csv(&ptr);
        strncpy(V[i].data_ajuizamento, token, sizeof(V[i].data_ajuizamento));
        V[i].data_ajuizamento[sizeof(V[i].data_ajuizamento) - 1] = '\0';

        token = extrair_campo_csv(&ptr);
        strncpy(V[i].id_classe, token, sizeof(V[i].id_classe));
        V[i].id_classe[sizeof(V[i].id_classe) - 1] = '\0';

        token = extrair_campo_csv(&ptr);
        strncpy(V[i].id_assunto, token, sizeof(V[i].id_assunto));
        V[i].id_assunto[sizeof(V[i].id_assunto) - 1] = '\0';

        token = extrair_campo_csv(&ptr);
        V[i].ano_eleicao = atoi(token);
    }

    fclose(fp);
    return V;
}

int comparar_datas(const char *d1, const char *d2) {
    for (int i = 0; d1[i] != '\0' && d2[i] != '\0'; i++) {
        if (d1[i] != d2[i])
            return d1[i] - d2[i];
    }
    return 0;
}

void trocar(processo *a, processo *b) {
    processo temp = *a;
    *a = *b;
    *b = temp;
}

int particionar_data(processo *dados, int inicio, int fim) {
    char *pivo = dados[fim].data_ajuizamento;
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (comparar_datas(dados[j].data_ajuizamento, pivo) > 0) {
            i++;
            trocar(&dados[i], &dados[j]);
        }
    }

    trocar(&dados[i + 1], &dados[fim]);
    return i + 1;
}

int particionar_id(processo *dados, int inicio, int fim) {
    long int pivo = dados[fim].id;
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (dados[j].id < pivo) {
            i++;
            trocar(&dados[i], &dados[j]);
        }
    }

    trocar(&dados[i + 1], &dados[fim]);
    return i + 1;
}

void quick_sort_id(processo *dados, int inicio, int fim) {
    if (inicio < fim) {
        int indice_pivo = particionar_id(dados, inicio, fim);
        quick_sort_id(dados, inicio, indice_pivo - 1);
        quick_sort_id(dados, indice_pivo + 1, fim);
    }
}

void quick_sort_data_desc(processo *dados, int inicio, int fim) {
    if (inicio < fim) {
        int indice_pivo = particionar_data(dados, inicio, fim);
        quick_sort_data_desc(dados, inicio, indice_pivo - 1);
        quick_sort_data_desc(dados, indice_pivo + 1, fim);
    }
}

void display_dados(processo *V){
    for (int i = 0; i < QUANTIDADE_DE_LINHAS; i++){
         printf("%ld,%s,%s,%s,%s,%c\n", V[i].id, V[i].numero, V[i].data_ajuizamento, V[i].id_classe, V[i].id_assunto, V[i].ano_eleicao);
    }
}

bool contem_id_classe(const char *id_classe_campo, const char *id_classe_procurado) {
    char buffer[21];
    int i = 0, j = 0;

    while (id_classe_campo[i] != '\0') {
        if (id_classe_campo[i] == '{' || id_classe_campo[i] == ',' || id_classe_campo[i] == '}') {
            if (j > 0) {
                buffer[j] = '\0';
                if (strcmp(buffer, id_classe_procurado) == 0) {
                    return true;
                }
                j = 0;
            }
        } else {
            buffer[j++] = id_classe_campo[i];
        }
        i++;
    }
    return false;
}

int contar_por_id_classe(processo *dados, int tamanho, const char *id_classe_alvo) {
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (contem_id_classe(dados[i].id_classe, id_classe_alvo)) {
            contador++;
        }
    }
    return contador;
}

bool ja_existe(const char id_assuntos_encontrados[][TAM_CAMPO], int total, const char *novo) {
    for (int i = 0; i < total; i++) {
        if (strcmp(id_assuntos_encontrados[i], novo) == 0) {
            return true;
        }
    }
    return false;
}

int contar_id_assuntos_distintos(processo *dados, int tamanho) {
    char id_assuntos_encontrados[MAX_ID_ASSUNTOS][TAM_CAMPO];
    int total_distintos = 0;

    for (int i = 0; i < tamanho; i++) {
        const char *campo = dados[i].id_assunto;
        char buffer[TAM_CAMPO];
        int j = 0;

        for (int k = 0; campo[k] != '\0'; k++) {
            if (campo[k] == '{' || campo[k] == ',' || campo[k] == '}') {
                if (j > 0) {
                    buffer[j] = '\0';
                    if (!ja_existe(id_assuntos_encontrados, total_distintos, buffer)) {
                        strcpy(id_assuntos_encontrados[total_distintos], buffer);
                        total_distintos++;
                    }
                    j = 0;
                }
            } else {
                if (j < TAM_CAMPO - 1) {
                    buffer[j++] = campo[k];
                }
            }
        }
    }

    return total_distintos;
}

void listar_processos_com_multiplos_assuntos(processo *dados, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        const char *campo = dados[i].id_assunto;
        int contador_virgulas = 0;

        for (int j = 0; j < TAM_CAMPO; j++) {
            if (campo[j] == ',') {
                contador_virgulas++;
            }
            if (campo[j] == '}'){
                break;
            }
        }

        if (contador_virgulas > 0) {
            printf("ID: %ld | Assuntos: %s\n", dados[i].id, dados[i].id_assunto);
        }
    }
}

int dias_em_tramitacao(const char *data_ajuizamento) {
    int ano, mes, dia, hora, min, seg;

    if (sscanf(data_ajuizamento, "%4d-%2d-%2d %2d:%2d:%2d", &ano, &mes, &dia, &hora, &min, &seg) != 6) {
        return -1;
    }

    struct tm data_aju = {0};
    data_aju.tm_year = ano - 1900;
    data_aju.tm_mon = mes - 1;
    data_aju.tm_mday = dia;
    data_aju.tm_hour = hora;
    data_aju.tm_min = min;
    data_aju.tm_sec = seg;

    time_t t_aju = mktime(&data_aju);
    time_t agora = time(NULL);

    double segundos = difftime(agora, t_aju);
    return (int)(segundos / (60 * 60 * 24));
}