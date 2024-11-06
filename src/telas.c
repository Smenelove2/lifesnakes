#include "telas.h"
#include "screen.h"
#include <stdio.h>

int tela_inicio() {
    int escolha = 0;
    screenSetColor(WHITE, WHITE);
    printf("LifeSnakes\n");
    printf("1 - Jogar\n");
    printf("2 - Leaderboard\n");
    printf("3 - Como Jogar\n");
    printf("aperte qualquer botão pra sair\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
    return escolha;
}

int escolha_dificuldade(){
    int dificuldade=0;

    screenSetColor(WHITE, WHITE);

    printf("Escolha o modo:\n");
    printf("1 - Fácil\n");
    printf("2 - Normal\n");
    printf("3 - Difícil\n");
    printf("Escolha: ");
    scanf("%d", &dificuldade);

    return dificuldade;
}

int drawMap(int *pontuacao) {
    screenSetColor(BLUE, BLUE);

    for (int i = MINX; i <= MAXX; i++) {
        screenGotoxy(i, MINY);
        printf(" ");
        screenGotoxy(i, MAXY);
        printf(" ");
    }

    for (int i = MINY; i <= MAXY; i++) {
        screenGotoxy(MINX, i);
        printf(" ");
        screenGotoxy(MAXX, i);
        printf(" ");
    }

    int middleX = (MINX + MAXX) / 2;
    int posY = MAXY + 1;

    screenGotoxy(middleX - 6, posY);

    printf("Pontuação: %d", *pontuacao);
}

void derrota(){
    screenSetColor(BLUE, BLUE);

    for (int i = MINX; i <= MAXX; i++) {
        screenGotoxy(i, MINY);
        printf(" ");
        screenGotoxy(i, MAXY);
        printf(" ");
    }

    for (int i = MINY; i <= MAXY; i++) {
        screenGotoxy(MINX, i);
        printf(" ");
        screenGotoxy(MAXX, i);
        printf(" ");
    }

    screenSetColor(WHITE, BLACK);

    int middleX = (MINX + MAXX) / 2;
    int middleY = (MINY + MAXY) / 2;

    screenGotoxy(middleX - 10, middleY - 1); 
    printf("======================");
    screenGotoxy(middleX - 4, middleY);
    printf("GAME OVER");
    screenGotoxy(middleX - 10, middleY + 1);
    printf("======================");

    getchar();
    getchar();
}

void vitoria(){
    screenSetColor(BLUE, BLUE);

    for (int i = MINX; i <= MAXX; i++) {
        screenGotoxy(i, MINY);
        printf(" ");
        screenGotoxy(i, MAXY);
        printf(" ");
    }

    for (int i = MINY; i <= MAXY; i++) {
        screenGotoxy(MINX, i);
        printf(" ");
        screenGotoxy(MAXX, i);
        printf(" ");
    }

    screenSetColor(WHITE, BLACK);

    int middleX = (MINX + MAXX) / 2;
    int middleY = (MINY + MAXY) / 2;

    screenGotoxy(middleX - 10, middleY - 1);
    printf("======================");
    screenGotoxy(middleX - 4, middleY);
    printf("GAME OVER");
    screenGotoxy(middleX - 10, middleY + 1);
    printf("======================");

    getchar();
    getchar();
}

int instrucoes(){
    screenSetColor(WHITE, WHITE);
    printf("Como jogar!\n");
    printf("1 - W,A,S,D para se mover\n");
    printf("2 - Não bata nas paredes\n");
    printf("3 - Coma a comida para crescer\n");
    printf("4 - Sempre que comer a cobra inverte de lado movendo-se ao oposto do que estava quando comeu a fruta\n");
    printf("5 - Para sair do jogo aperte ""ESC""\n");
}

void leaderboard() {
    FILE *recordes;
    recordes = fopen("registro.txt", "r");
    if (recordes == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    char c;
    while ((c = fgetc(recordes)) != EOF) {
        printf("%c", c);
    }
    fclose(recordes);
}
