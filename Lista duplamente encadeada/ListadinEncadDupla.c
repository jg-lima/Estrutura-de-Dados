/* ============================================================
   ListaDinEncadDupla.c
   Estrutura de Dados I - UFPB - Aula 08
   Lista dinamica duplamente encadeada

   Implementacao identica a apresentada nos slides da Aula 08.
   A definicao de struct elemento esta aqui, e nao no .h: e isso
   que torna Lista um tipo opaco. Quem usa a biblioteca enxerga
   apenas Lista* e os prototipos declarados no cabecalho.

   Qualquer funcao nova que precise percorrer a lista no a no
   tem de ser escrita NESTE arquivo, porque e aqui que os campos
   ant, dados e prox sao visiveis.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "ListaDinEncadDupla.h"

/* Definicao do tipo lista: oculta de quem usa a biblioteca.
   Com struct aluno de 48 bytes, sizeof(Elem) = 64 bytes em uma
   plataforma de 64 bits, contra 56 na lista encadeada simples. */
struct elemento {
    struct elemento *ant;
    struct aluno dados;
    struct elemento *prox;
};
typedef struct elemento Elem;

/* --- criacao e destruicao ---------------------------------- */

Lista* cria_lista(void) {
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if (li != NULL)
        *li = NULL;
    return li;
}

void libera_lista(Lista* li) {
    if (li != NULL) {
        Elem* no;
        while ((*li) != NULL) {
            no = *li;
            *li = (*li)->prox;   /* avanca antes de liberar */
            free(no);
        }
        free(li);                /* libera o bloco do inicio */
    }
}

/* --- informacoes de estado --------------------------------- */

int tamanho_lista(Lista* li) {
    if (li == NULL)
        return 0;
    int cont = 0;
    Elem* no = *li;
    while (no != NULL) {
        cont++;
        no = no->prox;
    }
    return cont;
}

int lista_cheia(Lista* li) {
    (void) li;               /* parametro mantido por simetria com as demais */
    return 0;
}

int lista_vazia(Lista* li) {
    if (li == NULL)
        return 1;            /* lista invalida */
    if (*li == NULL)
        return 1;            /* nenhum elemento alocado */
    return 0;
}

/* --- insercao ---------------------------------------------- */

int insere_lista_inicio(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;    /* sem memoria: a lista esta cheia */
    no->dados = al;
    no->prox = (*li);
    no->ant = NULL;
    if (*li != NULL)             /* lista nao vazia */
        (*li)->ant = no;         /* o antigo primeiro ganha antecessor */
    *li = no;
    return 1;
}

int insere_lista_final(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = al;
    no->prox = NULL;
    if ((*li) == NULL) {         /* lista vazia: insere inicio */
        no->ant = NULL;
        *li = no;
    } else {
        Elem* aux = *li;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = no;
        no->ant = aux;
    }
    return 1;
}

int insere_lista_ordenada(Lista* li, struct aluno al) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = al;
    if ((*li) == NULL) {         /* lista vazia: insere inicio */
        no->prox = NULL;
        no->ant = NULL;
        *li = no;
        return 1;
    }
    else {
        /* O livro declara ante sem valor inicial. A inicializacao com
           NULL nao altera o comportamento e elimina o aviso
           "may be used uninitialized" do gcc com -O2. */
        Elem *ante = NULL, *atual = *li;
        while (atual != NULL &&
               atual->dados.matricula < al.matricula) {
            ante = atual;
            atual = atual->prox;
        }
        if (atual == *li) {      /* insere inicio */
            no->ant = NULL;
            (*li)->ant = no;
            no->prox = (*li);
            *li = no;
        } else {                 /* insere no meio ou no final */
            no->prox = ante->prox;
            no->ant = ante;
            ante->prox = no;
            if (atual != NULL)   /* nao e o ultimo */
                atual->ant = no;
        }
        return 1;
    }
}

/* --- remocao ----------------------------------------------- */

int remove_lista_inicio(Lista* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    *li = no->prox;
    if (no->prox != NULL)          /* existe um seguinte */
        no->prox->ant = NULL;      /* ele passa a ser o primeiro */
    free(no);
    return 1;
}

int remove_lista_final(Lista* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    while (no->prox != NULL)
        no = no->prox;
    if (no->ant == NULL)           /* primeiro e unico */
        *li = no->prox;
    else
        no->ant->prox = NULL;      /* penultimo obtido pelo proprio no */
    free(no);
    return 1;
}

int remove_lista(Lista* li, int mat) {
    if (li == NULL) return 0;
    if ((*li) == NULL) return 0;   /* lista vazia */
    Elem* no = *li;
    while (no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if (no == NULL)                /* chave ausente */
        return 0;
    if (no->ant == NULL)           /* remove o primeiro */
        *li = no->prox;
    else
        no->ant->prox = no->prox;
    if (no->prox != NULL)          /* nao e o ultimo */
        no->prox->ant = no->ant;
    free(no);
    return 1;
}

/* --- busca ------------------------------------------------- */

int busca_lista_pos(Lista* li, int pos, struct aluno *al) {
    if (li == NULL || pos <= 0)
        return 0;
    Elem *no = *li;
    int i = 1;
    while (no != NULL && i < pos) {
        no = no->prox;
        i++;
    }
    if (no == NULL)
        return 0;
    else {
        *al = no->dados;
        return 1;
    }
}

int busca_lista_mat(Lista* li, int mat, struct aluno *al) {
    if (li == NULL)
        return 0;
    Elem *no = *li;
    while (no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if (no == NULL)
        return 0;
    else {
        *al = no->dados;
        return 1;
    }
}
/* Questão 1*/
void imprime_lista_inversa(Lista *li){
    if(li == NULL || *li == NULL) return;
    Elem *no = *li;

    while(no->prox != NULL){
        no = no->prox;
    }

    while(no != NULL){
        printf("%d ", no->dados.matricula); 
        no = no->ant;
    }
}
/* Questão 2*/
int lista_consistente(Lista* li){
    if (li == NULL) return 0;
    if (*li == NULL) return 1;

    Elem *no = *li;
    if(no->ant != NULL) return 0;

    while(no->prox != NULL){
        if(no->prox->ant != no) return 0;
        no = no->prox;
    }
    return 1;
}
/* Questão 3 */
/*a) O elemento com a matrícula 33 é o primeiro elemento da lista, o que significa que o seu campo 'ant' é NULL.
Ao executar o ciclo while, a condição (no->dados.matricula != 33) é falsa logo no primeiro nó. O ciclo termina sem avançar.
De seguida, o programa tenta executar a instrução: no->ant->prox = no->prox;
Como no->ant é NULL, ocorre a tentativa de aceder ao membro 'prox' através de um apontador nulo (NULL->prox).
Isto resulta num erro grave de acesso à memória (Segmentation Fault / Falha de Segmentação), fazendo com que o programa termine abruptamente.*/

/*- b)

Estado e esquema da lista após a chamada remove_lista(li, 23):
 
[*li] -> [ Nó 33 ] ---> [ Nó 16 ] -> NULL
           ant: NULL      ant: [endereço inválido do Nó 23 libertado]
           prox: 16       prox: NULL
 1. Por que motivo tamanho_lista devolve o valor esperado (2):
A função tamanho_lista percorre a lista do início ao fim avançando exclusivamente pelos apontadores 'prox'.
Como a ligação para a frente ficou correta (33 -> 16 -> NULL), a contagem contabiliza com sucesso os 2 nós.
2. Por que motivo lista_consistente devolve 0:
  A função lista_consistente valida a propriedade (no->prox->ant == no).
  No Nó 33, o nó sucessor (no->prox) é o Nó 16. Contudo, 16->ant aponta para o endereço libertado
  do antigo Nó 23 e não de volta para o Nó 33.
  Como a ligação bidirecional foi violada, lista_consistente devolve 0.
 */

/*c)*/
int remove_lista(Lista* li, int mat){
    if (li == NULL || *li == NULL) return 0;

    Elem* no = *li;

    while(no != NULL && no->dados.matricula != mat)
        no = no->prox;
    if(no == NULL) return 0;
    if (no->ant == NULL){
        *li = no->prox;
    } else{
        no->ant->prox = no->prox;
    }

    if (no->prox != NULL) {
        no->prox->ant = no->ant;
    }    
    free(no);
    return 1;
}

/* Questão 4*/
int remove_lista_pos(Lista* li, int pos){
    if (li == NULL || *li == NULL || pos < 1) return 0;

    Elem *no = *li;

    for (int i = 1; i < pos; i++){
        no = no->prox;
        if(no == NULL) return 0;
    }
    if (no->prox != NULL) {
        no->prox->ant = no->ant;
    }

    free(no);
    return 1;
} /* Custo O(n)*/

/* Questão 5*/
int move_para_inicio(Lista* li, int mat) {
    if (li == NULL || *li == NULL) return 0;

    Elem* no = *li;
    while (no != NULL && no->dados.matricula != mat) {
        no = no->prox;
    }
    if (no == NULL) return 0;
    if (no == *li) return 1;

    no->ant->prox = no->prox; 
    if (no->prox != NULL) {
        no->prox->ant = no->ant;
    }

    no->prox = *li; 
    no->ant = NULL; 
    (*li)->ant = no; 
    *li = no; 

    return 1; 
}
/*Quando o elemento a ser movido se encontra no meio da lista, são escritos exatamente 5 campos de ponteiro 
pertencentes às estruturas dos nós. Ao retirar o nó da sua posição original, altera-se o campo no->ant->prox 
para apontar para o nó sucessor e o campo no->prox->ant para apontar para o nó antecessor. Em seguida, ao 
posicioná-lo no início da lista, atualiza-se o campo no->prox para apontar para o antigo primeiro elemento (*li), 
define-se no->ant como NULL e ajusta-se o campo (*li)->ant do antigo primeiro elemento para apontar de volta 
para o nó movido (além da escrita no ponteiro externo da cabeça da lista *li). Por outro lado, quando o elemento 
é o último da lista, são escritos apenas 4 campos de ponteiro nas estruturas dos nós. Neste caso, como no->prox 
é igual a NULL, o bloco condicional contendo a instrução no->prox->ant é ignorado, realizando-se unicamente as 
escritas em no->ant->prox (que recebe NULL), no->prox (que aponta para a antiga cabeça), no->ant (que recebe NULL) 
e (*li)->ant (que aponta para o nó movido).
 */

