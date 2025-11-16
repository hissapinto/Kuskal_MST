//Isabela Hissa Pinto | RA:10441873
//Caio Ariel | RA:
//Kaique Paiva | RA:


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int origem;
    int destino;
    int valor;
} ponto;

int conta_linhas(FILE *arquivo){
    int linhas = 0;
    char buffer[10];
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        linhas++;
    }
    rewind(arquivo);
    return linhas;
}

void imprime_vetor_pontos(const ponto *vetor, int tam){
    for (int i = 0; i < tam; i++) {
    printf("Linha %d: origem=%c destino=%c valor=%d\n",
           i,
           vetor[i].origem,
           vetor[i].destino,
           vetor[i].valor);
    }
}

//Inacabado e nao sei se to fazendo certo
void escolhe_pontos(ponto *opcoes, ponto *escolhidos, int qtd_letras, int qtd_linhas){
    for(int i = 0; i<qtd_letras-1; i++){ //Escolhe 7 caminhos
        ponto aux = {'z','z',100};
        for(int j = 0; j<qtd_linhas; j++){
            if(opcoes[j].valor < aux.valor){
                aux = opcoes[j];
            }
        }
        escolhidos[i] = aux;
        for(int k = 0; k<qtd_linhas; k++){

        }
    }
}

int main(){
    FILE *grafos = fopen("grafos.txt", "r");
    if(grafos == NULL){
        printf("Erro na leitura do arquivo.\n");
        return -1;
    }

    int qtd_linhas = conta_linhas(grafos);
    ponto *resultado = malloc(7 * sizeof(ponto));
    ponto *pontos = malloc(qtd_linhas * sizeof(ponto));
    if (pontos == NULL){
        printf("Erro ao criar malloc - pontos.");
        return -1;
    }
    if (resultado == NULL){
        printf("Erro ao criar malloc - resultado.");
        return -1;
    }

    char origem, destino;
    int valor;
    int i = 0;
    while(fscanf(grafos, " %c %c %d", &origem, &destino, &valor) == 3) {
         pontos[i].origem = origem;
         pontos[i].destino = destino;
         pontos[i].valor = valor;
         i++;
    }

    imprime_vetor_pontos(pontos, qtd_linhas);

    free(pontos);
    free(resultado);
    return 0;
}