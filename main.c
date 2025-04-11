#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main(){
    
    processo *dados = importar_csv("dados.csv");
    printf("Arquivo CSV importado \n");

    display_dados(dados);

    //Ordenar, em ordem crescente, o conjunto de dados a partir do atributo “id”;
    quick_sort_id(dados, 0, QUANTIDADE_DE_LINHAS);
    printf("Dados ordenados em ordem crescente pelo ID \n");

    display_dados(dados);

    //Ordenar, em ordem decrescente, o conjunto de dados a partir do atributo “data_ajuizamento”;
    quick_sort_data_desc(dados, 0, QUANTIDADE_DE_LINHAS);
    printf("Dados ordenados em ordem decrescente por data \n");

    display_dados(dados);


    //Contar quantos processos estão vinculados a um determinado “id_classe”;
    char *id_alvo = "11541";
    int processos_por_classe = contar_por_id_classe(dados, QUANTIDADE_DE_LINHAS, id_alvo);

    printf("Processos por ID %s: %d \n", id_alvo, processos_por_classe);

    //Identificar quantos “id_assuntos” constam nos processos presentes na base de dados;
    int assuntos = contar_id_assuntos_distintos(dados, MAX_ID_ASSUNTOS);
    
    printf("Assuntos encontrados: %d \n", assuntos);


    //Listar todos os processos que estão vinculados a mais de um assunto;
    printf("Processos com mais de um assunto:\n");
    listar_processos_com_multiplos_assuntos(dados, QUANTIDADE_DE_LINHAS);

    //Indicar a quantos dias um processo está em tramitação na justiça.
    int dias = dias_em_tramitacao(dados[0].data_ajuizamento);
    printf("Processo ID %ld está em tramitação há %d dias\n", dados[0].id, dias);

    return 0;
}