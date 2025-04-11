#ifndef FUNCOES_H
#define FUNCOES_H

#define QUANTIDADE_DE_LINHAS 1000
#define MAX_ID_ASSUNTOS 1000
#define TAM_CAMPO 15

typedef struct {
    long int id;
    char numero[22 + 1];
    char data_ajuizamento[23 + 1];
    char id_classe[TAM_CAMPO];
    char id_assunto[TAM_CAMPO];
    int ano_eleicao;
} processo;

processo *importar_csv(char *Arquivo);
void display_dados(processo *dados);
void quick_sort_id(processo *dados, int inicio, int fim);
void quick_sort_data_desc(processo *dados, int inicio, int fim);
int contar_por_id_classe(processo *dados, int tamanho, const char *id_classe_alvo);
int contar_id_assuntos_distintos(processo *dados, int tamanho);
void listar_processos_com_multiplos_assuntos(processo *dados, int tamanho);
int dias_em_tramitacao(const char *data_ajuizamento);

#endif