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

    if (input->count == 0) input->isClearing = 0;  // ← done clearing

}

void clearAnimNumberInput(DigitInput *input){
    input->isClearing = 1; // ← clearing started
    for (int i = 0; i < input->count; i++) { input->targetY[i] = -30; }
}

void clearInstantNumberInput(DigitInput *input){

    for (int i = 0; i < input->count; i++) {
        input->count--;
        if (input->count < 0) input->count = 0;
        input->text[i] = '\0';
    }

    input->isClearing  = 0; // ← done clearing, intant
}


void drawAnimatedNumberInput(DigitInput input, Vector2 pos, Color color){
    if (input.count <= 0) return;

    int fontSize = 200;
    int spacing = 10;

    Vector2 totalSize = MeasureTextEx(font, input.text, fontSize, spacing);

    float currentAdvance = 0.0f;

    for (int i = 0; i < input.count; i++){
        char buf[2] = { input.text[i], '\0' };

        DrawTextEx(font, buf,
                 (Vector2){(pos.x - totalSize.x / 2.0f) + currentAdvance,
                 pos.y + input.currentY[i] - totalSize.y / 2.0f},
                 fontSize, 0,  color);
                 
        Vector2 charSize = MeasureTextEx(font, buf, fontSize, 0);
        currentAdvance += charSize.x + spacing;
    }
}
