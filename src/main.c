#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "telas.h"

#define MAX 1920

struct Node {
    int pontuacao;
    char nome[100];
    struct Node *next;
}Node;

int pontuação = 0;
int snakeX[MAX];
int snakeY[MAX];
int snakeLength = 5;
int snakeDirection = 0;
int foodX, foodY; 
int gameRunning = 1;
float dificuldade = 0;
char nome[100];
struct Node *head = NULL;


void add(struct Node **head, int pontuacao, char nome[100]);
void escrever(struct Node *head);
void initializeGame();
void drawSnake();
void drawFood();
void moveSnake();
void handleInput(int key);

int main() {
    screenInit(0);
    keyboardInit();
    timerInit(100 * dificuldade);

    while (1) {
        int escolha = tela_inicio();

        if (escolha == 1) {
            screenClear();
            int difficuldade = escolha_dificuldade();

            if(difficuldade==1){
                dificuldade=0.5;
            }
            else if(difficuldade==2){
                dificuldade=1;
            }
            else if(difficuldade==3){
                dificuldade=2;
            }
            else{
                dificuldade=1;
            }

            screenClear();
            printf("Digite seu nome\n");
            scanf("%s", nome);
            screenClear();
            initializeGame();
            gameRunning = 1;

            while (gameRunning) {
                if (keyhit()) {
                    int key = readch();
                    handleInput(key);
                }

                moveSnake();
                screenClear();
                drawMap(&pontuação);
                drawSnake();
                drawFood();
                screenUpdate();
                screenClear();
                usleep(100000 / dificuldade);
            }

        } else if (escolha == 2) {
            screenClear();
            leaderboard();
            printf("\nPressione qualquer tecla para voltar ao menu principal...");
            getchar();
            getchar();
            screenClear();

        } else if (escolha == 3) {
            screenClear();
            instrucoes();
            printf("\nPressione qualquer tecla para voltar ao menu principal...");
            getchar();
            getchar();
            screenClear();

        } else {
            break;

        }
    }

    struct Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

void add(struct Node **head, int pontuacao, char nome[100]) {
    struct Node *novo = (struct Node *)malloc(sizeof(struct Node));
    novo->pontuacao = pontuacao;
    strcpy(novo->nome, nome);
    novo->next = NULL;

    if (*head == NULL || (*head)->pontuacao < pontuacao) {
        novo->next = *head;
        *head = novo;
        return;
    }

    struct Node *temp = *head;
    while (temp->next != NULL && temp->next->pontuacao >= pontuacao) {
        temp = temp->next;
    }

    novo->next = temp->next;
    temp->next = novo;
}

void escrever(struct Node *head) {
    FILE *arquivo = fopen("leaderboard.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    struct Node *temp = head;
    while (temp != NULL) {
        fprintf(arquivo, "%s %d\n", temp->nome,  temp->pontuacao);
        temp = temp->next;
    }

    fclose(arquivo);
}

void initializeGame() {
    screenInit(0);
    keyboardInit();
    timerInit(100);
    snakeX[0] = MAXX / 2;
    snakeY[0] = MAXY / 2;
    foodX = rand() % (MAXX - 3) + 1;
    foodY = rand() % (MAXY - 3) + 1;
    if (foodX == MAXX || foodY == MAXY || foodX == MINX || foodY == MINY) {
        foodX = rand() % (MAXX - 3) + 1;
        foodY = rand() % (MAXY - 3) + 1;
    }
    snakeDirection = 1;
}

void drawSnake() {
    screenSetColor(BLACK, GREEN);
    for (int i = 0; i < snakeLength; i++) {
        screenGotoxy(snakeX[i], snakeY[i]);
        switch (snakeDirection) {
            case 0: printf("▲"); break;
            case 1: printf("►"); break;
            case 2: printf("▼"); break;
            case 3: printf("◄"); break;
        }
    }
}

void drawFood() {
    screenSetColor(RED, BLACK);
    screenGotoxy(foodX, foodY);
    printf("@");
}

void moveSnake() {
    int nextX = snakeX[0], nextY = snakeY[0];

    switch (snakeDirection) {
        case 0: nextY--; break;
        case 1: nextX++; break;
        case 2: nextY++; break;
        case 3: nextX--; break;
    }

    if (nextX <= MINX || nextX >= MAXX || nextY <= MINY-1 || nextY >= MAXY-1) {
        gameRunning = 0;
        derrota();
        add(&head, pontuação, nome);
        escrever(head);
        pontuação = 0;
        screenClear();
    }

    if (snakeLength == MAXX * MAXY) {
        screenClear();
        vitoria();
        add(&head, pontuação, nome);
        escrever(head);
        pontuação = 0;
        screenClear();
        gameRunning = 0;
    }

    if (nextX == foodX && nextY == foodY) {
        pontuação++;
        foodX = rand() % (MAXX);
        foodY = rand() % (MAXY) + 1;

        if (foodX == MAXX || foodX == MINX || foodY == MAXY || foodY == MINY) {
            foodX = rand() % (MAXX - 3) + 1;
            foodY = rand() % (MAXY - 3) + 1;
        }

        if (snakeDirection == 0 || snakeDirection == 2) {
            snakeDirection = (snakeDirection + 2) % 4;

        } else if (snakeDirection == 1 || snakeDirection == 3) {
            snakeDirection = (snakeDirection - 2) % 4;

            if (snakeDirection < 0) {
                snakeDirection += 4;
            }

        }
        snakeLength++;

    } else {
        for (int i = snakeLength - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

    }

    snakeX[0] = nextX;
    snakeY[0] = nextY;
}

void handleInput(int key) {
    switch (key) {
        case 'w': if (snakeDirection != 2) snakeDirection = 0; break;
        case 'd': if (snakeDirection != 3) snakeDirection = 1; break;
        case 's': if (snakeDirection != 0) snakeDirection = 2; break;
        case 'a': if (snakeDirection != 1) snakeDirection = 3; break;
        case 27: gameRunning = 0; break;
    }
}
