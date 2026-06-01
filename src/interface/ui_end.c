#include <ui.h>

TextInput playerNameInput;


// ___state highscore screen___________________________________________________________________________________________

// literalmente peguei a logica desse exemplo do raylib
// https://www.raylib.com/examples/text/loader.html?name=text_input_box
void updateHighscoreScreen(Session *game){
    // Update
    //----------------------------------------------------------------------------------
    if (CheckCollisionPointRec(GetMousePosition(), playerNameInput.textBox)) playerNameInput.isActive = true;
    else playerNameInput.isActive = false;

    if (playerNameInput.isActive)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (playerNameInput.lettercount < playerNameInput.maxInput))
            {
                playerNameInput.text[playerNameInput.lettercount] = (char)key;
                playerNameInput.text[playerNameInput.lettercount + 1] = '\0'; // Add null terminator at the end of the string
                playerNameInput.lettercount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            playerNameInput.lettercount--;
            if (playerNameInput.lettercount < 0) playerNameInput.lettercount = 0;
            playerNameInput.text[playerNameInput.lettercount] = '\0';
        }


        if (IsKeyPressed(KEY_ENTER))
        {
            atualizarNomePlayer(game, playerNameInput.text);
            atualizarHighscore(game);
            game->state = STATE_WIN;
        }
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (playerNameInput.isActive) playerNameInput.framecounter++;
}

void drawHighscoreScreen(Session *game){
            DrawText("PLACE MOUSE OVER INPUT BOX!", 240, 140, 20, GRAY);

            DrawRectangleRec(playerNameInput.textBox, LIGHTGRAY);
            if (playerNameInput.isActive) DrawRectangleLines((int)playerNameInput.textBox.x, (int)playerNameInput.textBox.y, (int)playerNameInput.textBox.width, (int)playerNameInput.textBox.height, RED);
            else DrawRectangleLines((int)playerNameInput.textBox.x, (int)playerNameInput.textBox.y, (int)playerNameInput.textBox.width, (int)playerNameInput.textBox.height, DARKGRAY);

            DrawText(playerNameInput.text, (int)playerNameInput.textBox.x + 5, (int)playerNameInput.textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("INPUT CHARS: %i/%i", playerNameInput.lettercount, playerNameInput.maxInput), 315, 250, 20, DARKGRAY);

            if (playerNameInput.isActive)
            {
                if (playerNameInput.lettercount < playerNameInput.maxInput)
                {
                    // Draw blinking underscore char
                    if (((playerNameInput.framecounter/20)%2) == 0) DrawText("_", (int)playerNameInput.textBox.x + 8 + MeasureText(playerNameInput.text, 40), (int)playerNameInput.textBox.y + 12, 40, MAROON);
                }
                else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
            }
}

// ___state win___________________________________________________________________________________________

void updateWin(Session *game) {
    Vector2 mouse = GetMousePosition();
    if (IsKeyPressed(KEY_R) || updateButton(&btnPlayAgain, mouse)) {
        iniciarJogo(game);
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
    const char *scoreVal = TextFormat("%d", (int) game->score);
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
        iniciarJogo(game);
        clearInstantNumberInput(&input);
        for (int i = 0; i < 100; i++) {
            circlemarks[i].currentX = -10;
            circlemarks[i].y        = ALTURA - 90;
            circlemarks[i].raio     = 10;
            circlemarks[i].state    = CM_FREE;
        }
        activeMarkIndex = -1;
        menuState = MAIN;
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
        const char *scoreVal = TextFormat("%d", (int) game->score);
        Vector2 scoreValSize = MeasureTextEx(font, scoreVal, 24, 1);
        DrawTextEx(font, scoreVal, (Vector2){statsValX - scoreValSize.x, statsY + statsStep*2}, 24, 1, PS_BLUE);

    } else {
        const char *scoreLabel = "SCORE";
        DrawTextEx(font, scoreLabel, (Vector2){statsX, statsY}, 24, 1, PS_DARKGREY);
        const char *scoreVal = TextFormat("%d", (int) game->score);
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
