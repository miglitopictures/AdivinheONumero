#include <stdlib.h>
#include <ui.h>

Ruler basicRuler;
CircleMark circlemarks[100];
FeedbackArrow arrow;

// ___state playing______________________________________________________________________________________

void updatePlaying(Session *game){
    float dt = GetFrameTime(); 
    State prevState = game->state; 

    atualizarTempoRealScore(game, dt);
    processarGameover(game);

    if (prevState == STATE_PLAYING && game->state == STATE_GAMEOVER) PlaySound(sfxLose);

    updateCircleMarks(game); 

    if (IsKeyPressed(KEY_D)) debugMode *= -1;

    // Confirmar tentativa
    if (activeMarkIndex >= 0 && (IsKeyPressed(KEY_ENTER) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON))) {
        lockActiveCircleMark(game->currentPlayer);
        processarTentativa(game, atoi(input.text));
        clearAnimNumberInput(&input);
        activeMarkIndex = -1;
        arrow.pos.y = ALTURA / 2 + 50;

        // sfx
        PlaySound(sfxSelectSynth);
        // play end sounds based on resulting state
        SetSoundVolume(sfxWin, 0.6f);
        SetSoundVolume(sfxLose, 0.6f);
        if (game->state == STATE_WIN)      PlaySound(sfxWin);
        if (game->state == STATE_GAMEOVER) PlaySound(sfxLose);
    }


    // update arrow feedback
    if (game->guessCount != 0 && input.count == 0){
        arrow.shoudDraw = 1;
        arrow.pos.y = flerp(arrow.pos.y, arrow.target.y, 0.3);

    } else {
        arrow.shoudDraw = 0;
    }

    arrow.dir = game->guess < game->target ? 1 : 0;

    updateNumberInput(&input, 3);
}

void drawPlaying(Session *game){

    drawTopBar(game->score, startingScore, PS_BLUE);
    drawRuler(basicRuler, PS_WHITE, PS_BLACK);
    drawCircleMarks(game);   
    drawAnimatedNumberInput(input, (Vector2){ LARGURA / 2, ALTURA / 2}, PS_RED);
    if (arrow.shoudDraw) drawArrow(arrow, 200, 20);

    // DEBUG DRAW // aperte "D" para ativar e desativar o desenho de debug.
    if (debugMode == 1){
        DrawText(TextFormat("splayer = %d", game->currentPlayer), 20, ALTURA-260, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("shoulddrawArrow = %d", arrow.shoudDraw), 20, ALTURA-240, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("dificulty = %d", game->difficulty), 20, ALTURA-220, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("mode = %d", game->mode), 20, ALTURA-200, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("guessCount = %d",game->guessCount), 20, ALTURA-180, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("inputCount = %d",input.count), 20, ALTURA-160, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("Numero randomizado = %d",game->target), 20, ALTURA-140, DEBUGFONT, PS_DEBUG); 
        DrawText(TextFormat("Pontuação atual = %.0f",game->score), 20, ALTURA-120, DEBUGFONT, PS_DEBUG); 
        DrawText(game->trivia, 20, ALTURA-100, DEBUGFONT, PS_DEBUG); 
        if (game->guessCount > 0){
            float currentAdvance = 0.0f;
            for (int i =0; i < game->guessCount; i++){
                Vector2 itemSize = MeasureTextEx(font, TextFormat("%d", game->guessHistory[i]), 20, 2);
                DrawTextEx(font, TextFormat("%d", game->guessHistory[i]), (Vector2){50 + currentAdvance,50}, 20, 2, PS_DEBUG);
                currentAdvance += itemSize.x + 10;
            }
        }
    }
}