#include <ui.h>


// ___state win___________________________________________________________________________________________

void updateWin(Session *game) {
    Vector2 mouse = GetMousePosition();
    if (IsKeyPressed(KEY_R) || updateButton(&btnPlayAgain, mouse)) {
        IniciarJogo(game);
        clearInstantNumberInput(&input);
        for (int i = 0; i < 100; i++) {
            circlemarks[i].currentX = -10;
            circlemarks[i].y        = ALTURA - 90;
            circlemarks[i].raio     = 10;
            circlemarks[i].state    = CM_FREE;
        }
        activeMarkIndex = -1;
        menuState = MODES;
    }
}

void drawWin(Session *game) {
    // trivia at the top area
    Vector2 triviaSize = MeasureTextEx(font, game->trivia, 20, 1);
    DrawTextEx(font, game->trivia,
        (Vector2){LARGURA/2 - triviaSize.x/2, ALTURA/2 - 220},
        20, 1, PS_BLACK);

    // big target number as hero
    const char *targetStr = TextFormat("%d", game->target);
    Vector2 targetSize = MeasureTextEx(font, targetStr, 200, 1);
    DrawTextEx(font, targetStr,
        (Vector2){LARGURA/2 - targetSize.x/2, ALTURA/2 - 160},
        200, 1, PS_GREEN);

    // stats below, two columns: label left, value right
    int statsX     = LARGURA/2 - 160;
    int statsValX  = LARGURA/2 + 160;
    int statsY     = ALTURA/2 + 80;
    int statsStep  = 40;


    const char *scoreLabel = "SCORE";
    DrawTextEx(font, scoreLabel, (Vector2){statsX, statsY}, 24, 1, PS_DARKGREY);
    const char *scoreVal = TextFormat("%d", game->score);
    Vector2 scoreValSize = MeasureTextEx(font, scoreVal, 24, 1);
    DrawTextEx(font, scoreVal, (Vector2){statsValX - scoreValSize.x, statsY}, 24, 1, PS_BLACK);

    const char *guessLabel = "TENTATIVAS";
    DrawTextEx(font, guessLabel, (Vector2){statsX, statsY + statsStep}, 24, 1, PS_DARKGREY);
    const char *guessVal = TextFormat("%d", game->guessCount);
    Vector2 guessValSize = MeasureTextEx(font, guessVal, 24, 1);
    DrawTextEx(font, guessVal, (Vector2){statsValX - guessValSize.x, statsY + statsStep}, 24, 1, PS_BLACK);

    // button
    drawButton(&btnPlayAgain);
}

// ___state gameover______________________________________________________________________________________

void updateGameover(Session *game) {
    Vector2 mouse = GetMousePosition();
    if (IsKeyPressed(KEY_R) || updateButton(&btnPlayAgain, mouse)) {
        IniciarJogo(game);
        clearInstantNumberInput(&input);
        for (int i = 0; i < 100; i++) {
            circlemarks[i].currentX = -10;
            circlemarks[i].y        = ALTURA - 90;
            circlemarks[i].raio     = 10;
            circlemarks[i].state    = CM_FREE;
        }
        activeMarkIndex = -1;
        menuState = MODES;
    }
}

void drawGameover(Session *game) {
    // GAME OVER title
    const char *title = "GAME OVER";
    Vector2 titleSize = MeasureTextEx(font, title, 60, 1);
    DrawTextEx(font, title,
        (Vector2){LARGURA/2 - titleSize.x/2, ALTURA/2 - 220},
        60, 1, PS_RED);

    // big target number as hero
    const char *targetStr = TextFormat("%d", game->target);
    Vector2 targetSize = MeasureTextEx(font, targetStr, 200, 1);
    DrawTextEx(font, targetStr,
        (Vector2){LARGURA/2 - targetSize.x/2, ALTURA/2 - 160},
        200, 1, PS_DARKGREY);

    // stats
    int statsX    = LARGURA/2 - 160;
    int statsValX = LARGURA/2 + 160;
    int statsY    = ALTURA/2 + 80;
    int statsStep = 40;

    if (game->mode == MODO_ARCADE) {
        const char *roundsLabel = "RODADAS";
        DrawTextEx(font, roundsLabel, (Vector2){statsX, statsY}, 24, 1, PS_DARKGREY);
        const char *roundsVal = TextFormat("%d", game->round);
        Vector2 roundsValSize = MeasureTextEx(font, roundsVal, 24, 1);
        DrawTextEx(font, roundsVal, (Vector2){statsValX - roundsValSize.x, statsY}, 24, 1, PS_BLACK);

        const char *guessLabel = "TENTATIVAS";
        DrawTextEx(font, guessLabel, (Vector2){statsX, statsY + statsStep}, 24, 1, PS_DARKGREY);
        const char *guessVal = TextFormat("%d", game->totalGuesses + game->guessCount);
        Vector2 guessValSize = MeasureTextEx(font, guessVal, 24, 1);
        DrawTextEx(font, guessVal, (Vector2){statsValX - guessValSize.x, statsY + statsStep}, 24, 1, PS_BLACK);

        const char *scoreLabel = "SCORE";
        DrawTextEx(font, scoreLabel, (Vector2){statsX, statsY + statsStep*2}, 24, 1, PS_DARKGREY);
        const char *scoreVal = TextFormat("%d", game->score);
        Vector2 scoreValSize = MeasureTextEx(font, scoreVal, 24, 1);
        DrawTextEx(font, scoreVal, (Vector2){statsValX - scoreValSize.x, statsY + statsStep*2}, 24, 1, PS_BLUE);

    } else {
        const char *scoreLabel = "SCORE";
        DrawTextEx(font, scoreLabel, (Vector2){statsX, statsY}, 24, 1, PS_DARKGREY);
        const char *scoreVal = TextFormat("%d", game->score);
        Vector2 scoreValSize = MeasureTextEx(font, scoreVal, 24, 1);
        DrawTextEx(font, scoreVal, (Vector2){statsValX - scoreValSize.x, statsY}, 24, 1, PS_BLUE);

        const char *guessLabel = "TENTATIVAS";
        DrawTextEx(font, guessLabel, (Vector2){statsX, statsY + statsStep}, 24, 1, PS_DARKGREY);
        const char *guessVal = TextFormat("%d", game->guessCount);
        Vector2 guessValSize = MeasureTextEx(font, guessVal, 24, 1);
        DrawTextEx(font, guessVal, (Vector2){statsValX - guessValSize.x, statsY + statsStep}, 24, 1, PS_BLACK);
    }

    drawButton(&btnPlayAgain);
}
