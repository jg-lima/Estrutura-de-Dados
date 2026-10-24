/* ============================================================
   ListaDinEncadCirc.c
   Estrutura de Dados I - UFPB - Aula 07
   Lista dinamica encadeada circular

   Implementacao identica a apresentada nos slides da Aula 07.
   A definicao de struct elemento esta aqui, e nao no .h: e isso
   que torna Lista um tipo opaco. Quem usa a biblioteca enxerga
   apenas Lista* e os prototipos declarados no cabecalho.

   Qualquer funcao nova que precise percorrer a lista no a no
   tem de ser escrita NESTE arquivo, porque e aqui que os campos
   dados e prox sao visiveis.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "ListaDinEncadCirc.h"

/* Definicao do tipo lista: oculta de quem usa a biblioteca. */
struct elemento {
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
    if (li == NULL)
        return;
    if ((*li) != NULL) {
        Elem *aux, *no = *li;
        /* para quando o seguinte ao atual e o inicio: uma volta completa */
        while ((*li) != no->prox) {
            aux = no;
            no = no->prox;   /* avanca antes de liberar */
            free(aux);
        }
        free(no);
    }
    free(li);               /* libera o bloco do inicio */
}

/* --- informacoes de estado --------------------------------- */

int tamanho_lista(Lista* li) {
    if (li == NULL || (*li) == NULL)
        return 0;
    int cont = 0;
    Elem* no = *li;
    do {                    /* do-while: o primeiro elemento tem de ser contado */
        cont++;
        no = no->prox;
    } while (no != (*li));
    return cont;
}

int lista_cheia(Lista* li) {
    (void) li;              /* parametro mantido por simetria com as demais */
    return 0;
}

int lista_vazia(Lista* li) {
    if (li == NULL)
        return 1;           /* lista invalida */
    if (*li == NULL)
        return 1;           /* nenhum elemento alocado */
    return 0;
}

/* --- insercao ---------------------------------------------- */

int insere_lista_inicio(Lista* li, struct aluno al) {
    if (li == NULL)
        return 0;
    Elem *no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL)
        return 0;           /* sem memoria: a lista esta cheia */
    no->dados = al;
    if ((*li) == NULL) {    /* lista vazia: o no aponta para si mesmo */
        *li = no;
        no->prox = no;
    } else {
        Elem *aux = *li;
        while (aux->prox != (*li)) {   /* procura o ultimo */
            aux = aux->prox;
        }
        aux->prox = no;
        no->prox = *li;
        *li = no;           /* unica linha que difere da insercao no final */
    }
    return 1;
}

int insere_lista_final(Lista* li, struct aluno al) {
    if (li == NULL)
        return 0;
    Elem *no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL)
        return 0;
    no->dados = al;
    if ((*li) == NULL) {    /* lista vazia: o no aponta para si mesmo */
        *li = no;
        no->prox = no;
    } else {
        Elem *aux = *li;
        while (aux->prox != (*li)) {   /* procura o ultimo */
            aux = aux->prox;
        }
        aux->prox = no;
        no->prox = *li;     /* o inicio nao muda */
    }
    return 1;
}

int insere_lista_ordenada(Lista* li, struct aluno al) {
    if (li == NULL)
        return 0;
    Elem *no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL)
        return 0;
    no->dados = al;
    if ((*li) == NULL) {    /* lista vazia */
        *li = no;
        no->prox = no;
        return 1;
    }
    if ((*li)->dados.matricula > al.matricula) {   /* insercao no inicio */
        Elem *atual = *li;
        while (atual->prox != (*li))               /* procura o ultimo */
            atual = atual->prox;
        no->prox = *li;
        atual->prox = no;
        *li = no;
        return 1;
    }
    /* insercao no meio ou no final: ant acompanha atual uma posicao atras */
    Elem *ant = *li, *atual = (*li)->prox;
    while (atual != (*li) && atual->dados.matricula < al.matricula) {
        ant = atual;
        atual = atual->prox;
    }
    ant->prox = no;
    no->prox = atual;
    return 1;
}

/* --- remocao ----------------------------------------------- */

int remove_lista_inicio(Lista* li) {
    if (li == NULL)
        return 0;
    if ((*li) == NULL)
        return 0;           /* lista vazia */
    if ((*li) == (*li)->prox) {   /* elemento unico: a lista fica vazia */
        free(*li);
        *li = NULL;
        return 1;
    }
    Elem *atual = *li;
    while (atual->prox != (*li))  /* procura o ultimo */
        atual = atual->prox;
    Elem *no = *li;
    atual->prox = no->prox;       /* o ultimo passa a apontar para o segundo */
    *li = no->prox;               /* o segundo passa a ser o inicio */
    free(no);
    return 1;
}

int remove_lista_final(Lista* li) {
    if (li == NULL)
        return 0;
    if ((*li) == NULL)
        return 0;           /* lista vazia */
    if ((*li) == (*li)->prox) {   /* elemento unico: a lista fica vazia */
        free(*li);
        *li = NULL;
        return 1;
    }
    Elem *ant = NULL, *no = *li;
    while (no->prox != (*li)) {   /* procura o ultimo, guardando o anterior */
        ant = no;
        no = no->prox;
    }
    ant->prox = no->prox;         /* o penultimo passa a apontar para o inicio */
    free(no);
    return 1;
}

int remove_lista(Lista* li, int mat) {
    if (li == NULL)
        return 0;
    if ((*li) == NULL)
        return 0;           /* lista vazia */
    Elem *no = *li;
    if (no->dados.matricula == mat) {      /* remocao do inicio */
        if (no == no->prox) {              /* a lista fica vazia */
            free(no);
            *li = NULL;
            return 1;
        } else {
            Elem *ult = *li;
            while (ult->prox != (*li))     /* procura o ultimo */
                ult = ult->prox;
            ult->prox = (*li)->prox;
            *li = (*li)->prox;
            free(no);
            return 1;
        }
    }
    Elem *ant = no;
    no = no->prox;
    /* para ao encontrar a matricula ou ao completar uma volta */
    while (no != (*li) && no->dados.matricula != mat) {
        ant = no;
        no = no->prox;
    }
    if (no == (*li))
        return 0;           /* chave ausente */
    ant->prox = no->prox;
    free(no);
    return 1;
}

/* --- busca ------------------------------------------------- */

int busca_lista_pos(Lista* li, int pos, struct aluno *al) {
    if (li == NULL || (*li) == NULL || pos <= 0)
        return 0;
    Elem *no = *li;
    int i = 1;
    /* para no ultimo elemento ou ao alcancar a posicao desejada */
    while (no->prox != (*li) && i < pos) {
        no = no->prox;
        i++;
    }
    if (i != pos)
        return 0;           /* posicao inexistente */
    else {
        *al = no->dados;
        return 1;
    }
}

int busca_lista_mat(Lista* li, int mat, struct aluno *al) {
    if (li == NULL || (*li) == NULL)
        return 0;
    Elem *no = *li;
    /* para no ultimo elemento ou ao encontrar a matricula */
    while (no->prox != (*li) && no->dados.matricula != mat)
        no = no->prox;
    if (no->dados.matricula != mat)
        return 0;           /* chave ausente */
    else {
        *al = no->dados;
        return 1;
    }
}
/* Q1 */
void imprime_lista(Lista* li) {
    if (li == NULL || *li == NULL)
        return;
    Elem *no = *li;
    do {
        printf("%d ", no->dados.matricula);
        no = no->prox;
    } while (no != *li);
    printf("\n");
}

/* Q2 */
int maior_matricula(Lista* li, int *mat) {
    if (li == NULL || *li == NULL || mat == NULL)
        return 0;
    Elem *no = (*li)->prox;
    int maior = (*li)->dados.matricula;   /* começa pelo primeiro, não por 0 */
    while (no != *li) {
        if (no->dados.matricula > maior)
            maior = no->dados.matricula;
        no = no->prox;
    }
    *mat = maior;
    return 1;
}

/* Q3 */
int conta_aprovados(Lista* li) {
    if (li == NULL || *li == NULL)
        return 0;
    int cont = 0;
    Elem *no = *li;
    do {
        float media = (no->dados.n1 + no->dados.n2 + no->dados.n3) / 3;
        if (media >= 7.0)
            cont++;
        no = no->prox;
    } while (no != *li);
    return cont;
}
/* Q4 */
int busca_ciclica(Lista* li, int k, struct aluno *al) {
    if (li == NULL || *li == NULL || k <= 0 || al == NULL)
        return 0;

    int n = tamanho_lista(li);
    int avancos = (k - 1) % n;
    Elem *no = *li;
    for (int i = 0; i < avancos; i++)
        no = no->prox;

    *al = no->dados;
    return 1;
}
/* Q5 */
int remove_lista_pos(Lista* li, int pos) {
    if (li == NULL || *li == NULL || pos <= 0)
        return 0;

    if (pos == 1)
        return remove_lista_inicio(li);

    Elem *inicio = *li;
    Elem *ant = inicio;
    Elem *no = inicio->prox;
    int i = 2;

    while (no != inicio && i < pos) {
        ant = no;
        no = no->prox;
        i++;
    }
    if (no == inicio)
        return 0; /* pos passou do ultimo elemento */

    ant->prox = no->prox;
    free(no);
    return 1;
}
/* Q6 */
int rotaciona_lista(Lista* li, int k) {
    if (li == NULL || *li == NULL || k < 0)
        return 0;

    for (int i = 0; i < k; i++)
        *li = (*li)->prox;
    return 1;
}
/* Q7 */
int concatena_listas(Lista* li1, Lista* li2) {
    if (li1 == NULL || li2 == NULL || li1 == li2)
        return 0;

    if (*li2 == NULL)
        return 1; /* nada a anexar */

    if (*li1 == NULL) {
        *li1 = *li2;
        *li2 = NULL;
        return 1;
    }

    Elem *inicio1 = *li1;
    Elem *inicio2 = *li2;
    Elem *fim1 = inicio1;
    Elem *fim2 = inicio2;

    while (fim1->prox != inicio1)
        fim1 = fim1->prox;
    while (fim2->prox != inicio2)
        fim2 = fim2->prox;

    fim1->prox = inicio2;
    fim2->prox = inicio1;
    *li2 = NULL;
    return 1;
}
/* Q8 */
int josephus(Lista* li, int k, int *sobrevivente) {
    if (li == NULL || *li == NULL || k <= 0 || sobrevivente == NULL)
        return 0;

    /* um unico no: nao entra no laco de eliminacao */
    if ((*li)->prox == *li) {
        *sobrevivente = (*li)->dados.matricula;
        free(*li);
        *li = NULL;
        return 1;
    }

    Elem *inicio = *li;
    Elem *ant = inicio;
    while (ant->prox != inicio)
        ant = ant->prox; /* predecessor inicial e o ultimo no */
    Elem *atual = inicio;

    while ((*li)->prox != *li) { /* enquanto restar mais de uma pessoa */
        for (int i = 1; i < k; i++) {
            ant = atual;
            atual = atual->prox;
        }

        ant->prox = atual->prox;
        if (atual == *li)
            *li = atual->prox;

        Elem *removido = atual;
        atual = ant->prox; /* proxima contagem comeca no sucessor */
        free(removido);
    }

    *sobrevivente = (*li)->dados.matricula;
    free(*li);
    *li = NULL;
    return 1;
}