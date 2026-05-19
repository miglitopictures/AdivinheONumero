#include <ui.h>

// ___state menu__________________________________________________________________________________________

Button btnPlay, btnExit, btnStats; // MAIN MENU

Button btnPlayAgain; // END MENU

OptionPicker modePicker, difficultyPicker;
Button btnStart;

enum MenuState menuState = MAIN;

// ___button______________________________________________________________________________________________


int updateButton(Button *btn, Vector2 mouse){

    if(CheckCollisionPointRec(mouse, btn->rect)){
        btn->state = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? BT_PRESSED : BT_HOVER;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return 1;
    } else {
        btn->state = BT_IDLE;
    }
    return 0;
}

void drawButton(Button *btn){
    Color colors[3] = { PS_WHITE, PS_GREY, PS_DARKGREY };
    DrawRectangleRec(btn->rect, colors[btn->state]);
    DrawRectangleLinesEx(btn->rect, 1, PS_BLACK);
    
    Vector2 textSize = MeasureTextEx(font, btn->label, 20, 1);
    DrawTextEx(font,
               btn->label,
               (Vector2) {
                   btn->rect.x + btn->rect.width/2  - textSize.x/2,
                   btn->rect.y + btn->rect.height/2 - textSize.y/2 
               },
               20, 1,
               PS_BLACK);
}

// ___option picker_______________________________________________________________________________________

void updatePicker(OptionPicker *picker, Vector2 mouse, int posX, int posY) {
    if (!picker->isActive) return;
    int btnW = 30, totalW = 200, h = 36;
    Rectangle leftBtn  = { posX,                   posY, btnW, h };
    Rectangle rightBtn = { posX + totalW - btnW,   posY, btnW, h };

    picker->hoverLeft  = CheckCollisionPointRec(mouse, leftBtn);
    picker->hoverRight = CheckCollisionPointRec(mouse, rightBtn);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (picker->hoverLeft)
            picker->current = (picker->current - 1 + picker->count) % picker->count;
        if (picker->hoverRight)
            picker->current = (picker->current + 1) % picker->count;
    }

    // Keep keyboard fallback
    if (IsKeyPressed(KEY_RIGHT)) picker->current = (picker->current + 1) % picker->count;
    if (IsKeyPressed(KEY_LEFT))  picker->current = (picker->current - 1 + picker->count) % picker->count;
}

void drawPicker(OptionPicker *picker, int posX, int posY) {
    float alpha = picker->isActive ? 1.0f : 0.35f;  // NEW
    int totalW = 200, h = 36, btnW = 30;
//    int radius = 6;

    Color colBg     = ColorAlpha(PS_WHITE,    alpha);
    Color colBorder = ColorAlpha(PS_BLACK,    alpha);
    Color colLabel  = ColorAlpha(PS_BLACK,    alpha);
    Color colHover  = ColorAlpha(PS_GREY,     alpha);
    Color colValue  = ColorAlpha(PS_BLUE,     alpha);
    Color colDotOn  = ColorAlpha(PS_BLACK,    alpha);
    Color colDotOff = ColorAlpha(PS_DARKGREY, alpha);

    // Background pill
    DrawRectangleRec((Rectangle){posX, posY, totalW, h}, colBg);
    
    
    // Left button highlight
    if (picker->hoverLeft)
    DrawRectangleRec((Rectangle){posX, posY, btnW, h}, colHover);
    
    // Right button highlight
    if (picker->hoverRight)
    DrawRectangleRec((Rectangle){posX + totalW - btnW, posY, btnW, h}, colHover);
    
    // Outline
    DrawRectangleLinesEx((Rectangle){posX, posY, totalW, h}, 1.0f, colBorder);

    // Divider lines
    DrawLineEx((Vector2){posX + btnW, posY + 6}, (Vector2){posX + btnW, posY + h - 6}, 1.0f, colLabel);
    DrawLineEx((Vector2){posX + totalW - btnW, posY + 6}, (Vector2){posX + totalW - btnW, posY + h - 6}, 1.0f, colLabel);

    // Chevron arrows (simple triangles or text)
    DrawTextEx(font, "<", (Vector2){posX + 9, posY + h/2 - 9}, 20, 0, colLabel);
    DrawTextEx(font, ">", (Vector2){posX + totalW - btnW + 9, posY + h/2 - 9}, 20, 0, colLabel);

    // Current option label (centered)
    const char *val = picker->options[picker->current].label;
    Vector2 valSize = MeasureTextEx(font, val, 20, 1);
    DrawTextEx(font, val,
        (Vector2){posX + totalW/2 - valSize.x/2, posY + h/2 - valSize.y/2},
        20, 1, colValue);

    // Label above
    Vector2 labelSize = MeasureTextEx(font, picker->label, 20, 1);
    DrawTextEx(font, picker->label,
        (Vector2){posX + totalW/2 - labelSize.x/2, posY - 22},
        20, 1, colLabel);

    // Dot indicators below
    float dotR = 3.0f, dotSpacing = 10.0f;
    float dotsW = picker->count * dotSpacing - dotSpacing;
    float dotStartX = posX + totalW/2 - dotsW/2;
    for (int i = 0; i < picker->count; i++) {
        Color dotColor = (i == picker->current) ? colDotOn : colDotOff;
        DrawCircleV((Vector2){dotStartX + i * dotSpacing, posY + h + 10}, dotR, dotColor);
    }
}

void initMenu(Session *game){
    // Main Menu
    btnPlay = (Button){{LARGURA/2 - 150 - 75, ALTURA/2, 140, 40}, "JOGAR", BT_IDLE};
    btnStats = (Button){{LARGURA/2 - 75, ALTURA/2, 140, 40}, "ANALISAR", BT_IDLE};
    btnExit = (Button){{LARGURA/2 + 150 - 75, ALTURA/2, 140, 40}, "SAIR", BT_IDLE};

    modePicker = (OptionPicker){
    "MODO",
    {
        {"NORMAL", MODO_NORMAL},
        {"ARCADE", MODO_ARCADE}
    },
    2, 0, 0, 0, 1  // count, current, hoverLeft, hoverRight, isActive
};

difficultyPicker = (OptionPicker){
    "DIFICULDADE",
    {
        {"FACIL",   EASY},
        {"MEDIO",   MEDIUM},
        {"DIFICIL", HARD}
    },
    3, 0, 0, 0, 1  // count, current, hoverLeft, hoverRight, isActive
};

    btnStart = (Button){{LARGURA/2 - 75, ALTURA/2 + 60, 150, 40}, "COMEÇAR", BT_IDLE};


    btnPlayAgain = (Button){{LARGURA/2 - 100, ALTURA - 160, 200, 44}, "PLAY AGAIN", BT_IDLE}; // ENDMENU

}

void updateMenu(Session *game){
    Vector2 mousePosition = GetMousePosition();
    switch (menuState)
    {
    case LOGO:
        break;
    case MAIN:
        updateButton(&btnStats, mousePosition);
        if (updateButton(&btnPlay, mousePosition)) menuState = MODES;
        if (updateButton(&btnExit, mousePosition)) game->state = STATE_EXIT;
        break;
    case MODES:
        updatePicker(&modePicker, mousePosition, LARGURA/2 - 220, ALTURA/2);

        difficultyPicker.isActive = (modePicker.options[modePicker.current].value == MODO_NORMAL);

        updatePicker(&difficultyPicker, mousePosition, LARGURA/2 + 20, ALTURA/2);
        // apply selection on start
        if (updateButton(&btnStart, mousePosition)) {
            game->mode       = modePicker.options[modePicker.current].value;
            game->difficulty = difficultyPicker.isActive
                            ? difficultyPicker.options[difficultyPicker.current].value
                            : EASY;
            game->state = STATE_PLAYING;
        
        }
        break;
    case STATS:
        break;
    default:
        break;
    }    
}

void drawMenu(Session *game){
    switch (menuState)
    {
    case LOGO:
        break;
    case MAIN:
        drawButton(&btnPlay);
        drawButton(&btnStats);
        drawButton(&btnExit);
        break;
    case MODES:
        drawButton(&btnStart);
        drawPicker(&modePicker,       LARGURA/2 - 220, ALTURA/2);
        drawPicker(&difficultyPicker, LARGURA/2 + 20,  ALTURA/2);
        break;
    case STATS:
        break;
    default:
        break;
    }   
    
}
