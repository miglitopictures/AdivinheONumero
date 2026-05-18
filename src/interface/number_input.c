#include <raylib.h>
#include <ui.h>

DigitInput input = {0};

// ___number input____________________________________________________________________________________

void numberInputAdd(DigitInput *input, char key){
    input->text[input->count] = key;
    input->text[input->count + 1] = '\0';
    input->targetY[input->count] = 0;
    input->currentY[input->count] = 30; // starts offscreen, animates in
    input->count++;
}

void handleKeysNumberInput(DigitInput *input, int maxSize){
    int key = GetCharPressed();
    while (key > 0) {
        // NOTE: para numeros seria (48 até 57)
        if ((key >= 48) && (key <= 57) && input->count < maxSize) {
            numberInputAdd(input, (char)key);
            PlaySound(sfxChangeMark);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        input->targetY[input->count - 1] = -30;
    }

}

void updateNumberInput(DigitInput *input, int maxSize){
    handleKeysNumberInput(input, maxSize);
    
    for (int i = 0; i < input->count; i++) {
        
        input->currentY[i] = flerp(input->currentY[i], input->targetY[i], 0.3f);

        if (input->currentY[i] <= -20) {
            input->count--;
            if (input->count < 0) input->count = 0;
            input->text[i] = '\0';
        }
    }

    if (input->count == 0) inputClearing = false;  // ← done clearing

}

// limpa o number input com animação
void clearAnimNumberInput(DigitInput *input){
    for (int i = 0; i < input->count; i++){
        input->targetY[i] = -30;
    }
    inputClearing = 1;
}

// limpa o number input instantaneamente
void clearInstantNumberInput(DigitInput *input){
    inputClearing = 0; 

    for (int i = 0; i < input->count; i++) {

        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[i] = '\0';

    }
}


void drawAnimatedNumberInput(DigitInput input, int posX, int posY, int fontSize, int spacing, Color color, Font font){
    if (input.count <= 0) return; // Nao precisa desenhar nada

    Vector2 totalSize = MeasureTextEx(font, input.text, fontSize, spacing);

    float currentAdvance = 0.0f;

    for (int i = 0; i < input.count; i++){
        char buf[2] = { input.text[i], '\0' };

        DrawTextEx(font, buf,
                 (Vector2){(posX - totalSize.x / 2.0f) + currentAdvance,
                 posY + input.currentY[i] - totalSize.y / 2.0f},
                 fontSize, 0,  color);
                 
        Vector2 charSize = MeasureTextEx(font, buf, fontSize, 0);
        currentAdvance += charSize.x + spacing;
    }
}
