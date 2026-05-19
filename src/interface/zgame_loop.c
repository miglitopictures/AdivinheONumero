#include <ui.h>

// ___globals_________________________________________________________________________________________

int activeMarkIndex = -1;
int inputClearing = 0;

Sound sfxChangeMark, sfxSelectSynth, sfxWin, sfxLose;

// debug info toggler
int debugMode = 1;

// janela aplicacao
int LARGURA = 1280;
int ALTURA = 720;

// paleta de cores
Color PS_BLACK = BLACK;
Color PS_DARKGREY = {190, 190, 190, 255};
Color PS_GREY = {205, 205, 205, 255};
Color PS_WHITE = WHITE;
Color PS_BLUE = {55, 60, 218, 255};
Color PS_RED = {218, 55, 55, 255};
Color PS_GREEN = {28, 121, 0, 255};
Color PS_DEBUG = PURPLE;

int startingScore;

Font font;

// ___main loop__________________________________________________________________________________________________

void init(Session *game){

    // Initialize stuff
    IniciarJogo(game); // state
    initMenu(game);    // menu

    startingScore = game->score; // pega score inicial para mapear no scoreBar
    input.text[0] = '\0';
    basicRuler = createRuler(101, 70); // cria a régua

    // init arrow
    arrow.pos.x = LARGURA / 2;
    arrow.pos.y = ALTURA / 2 + 50;
    arrow.target.x = LARGURA / 2;
    arrow.target.y = ALTURA / 2;
    
    InitWindow(LARGURA, ALTURA, "Pablo Software's Numbers");
    InitAudioDevice();

    // Load sound files
    sfxChangeMark = LoadSound("assets/sfx/changeMarkPoint_Beep.wav");
    sfxSelectSynth = LoadSound("assets/sfx/select_synth.wav");
    sfxLose = LoadSound("assets/sfx/gameLose.wav");
    sfxWin = LoadSound("assets/sfx/gameWin.wav");

    SetTargetFPS(60);
    font = GetFontDefault();

}

void update(Session *game){
    switch (game->state) {
        case STATE_MENU:     updateMenu(game);     break;
        case STATE_PLAYING:  updatePlaying(game);  break;
        case STATE_WIN:      updateWin(game);      break;
        case STATE_GAMEOVER: updateGameover(game); break;
        case STATE_EXIT:                           break;
    }
}

void draw(Session *game){
    BeginDrawing();
        DrawFPS(20,20);
        ClearBackground(PS_GREY);

        switch (game->state) {
            case STATE_MENU:     drawMenu(game);     break;
            case STATE_PLAYING:  drawPlaying(game);  break;
            case STATE_WIN:      drawWin(game);      break;
            case STATE_GAMEOVER: drawGameover(game); break;
            default: break;
        }

    EndDrawing();   
}

//___raylib entrypoint____

void startRaylibMode(Session *game){

    init(game);

    while (!WindowShouldClose() && (game->state != STATE_EXIT)) {
        update(game);
        draw(game);
    }

    CloseAudioDevice();
    CloseWindow();
}