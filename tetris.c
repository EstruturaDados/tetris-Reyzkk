#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaCheia(Fila *f) { return f->qtd == TAM_FILA; }
int filaVazia(Fila *f) { return f->qtd == 0; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    return p->itens[p->topo--];
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n============================\n");
    printf("ESTADO ATUAL\n");
    printf("============================\n");

    printf("Fila de pecas: ");
    for (int i = 0; i < f->qtd; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n============================\n");
}

int main() {
    Fila fila;
    Pilha pilha;
    int idCounter = 0, opcao;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca(idCounter++));

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca da fila\n");
        printf("2 - Enviar peca da fila para pilha\n");
        printf("3 - Usar peca da pilha\n");
        printf("4 - Trocar peca da frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peca jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(idCounter++));
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 2:
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia!\n");
                } else if (!filaVazia(&fila)) {
                    Peca movida = desenfileirar(&fila);
                    empilhar(&pilha, movida);
                    printf("Peca [%c %d] movida para reserva.\n", movida.nome, movida.id);
                    enfileirar(&fila, gerarPeca(idCounter++));
                }
                break;

            case 3:
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("Peca [%c %d] usada da pilha.\n", usada.nome, usada.id);
                } else {
                    printf("Pilha vazia!\n");
                }
                break;

            case 4:
                if (!pilhaVazia(&pilha) && !filaVazia(&fila)) {
                    int idx = fila.frente;
                    Peca temp = fila.itens[idx];
                    fila.itens[idx] = pilha.itens[pilha.topo];
                    pilha.itens[pilha.topo] = temp;
                    printf("Troca entre topo e frente realizada.\n");
                } else {
                    printf("Impossivel trocar.\n");
                }
                break;

            case 5:
                if (pilha.topo >= 2 && fila.qtd >= 3) {
                    for (int i = 0; i < 3; i++) {
                        int idx = (fila.frente + i) % TAM_FILA;
                        Peca temp = fila.itens[idx];
                        fila.itens[idx] = pilha.itens[pilha.topo - i];
                        pilha.itens[pilha.topo - i] = temp;
                    }
                    printf("Troca multipla realizada!\n");
                } else {
                    printf("Faltam pecas para troca.\n");
                }
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
