#include <ui.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// ___score bar____________________________________________________________________________________________

void drawScoreBar(int currentScore, int max, Color bodyColor){
    int scorebarWidth = imap(currentScore, 0, max, 0 , LARGURA);
    DrawRectangle(0,0,scorebarWidth, 15, PS_BLUE);
}

// ___ruler_______________________________________________________________________________________________

Ruler createRuler(int divisions, float bodyHeight){
    Rectangle rect = {0,ALTURA - bodyHeight, LARGURA, bodyHeight};
    Ruler ruler = {rect, divisions, 10};
    return ruler;
}

void drawRuler(Ruler ruler, Color bodyColor, Color divisionColor) {
    
    DrawRectangleRec(ruler.rect,bodyColor);
    int spaces = ruler.divisions - 1;
    float startX = ruler.rect.x + ruler.margin;
    float distBetween = (ruler.rect.width - (2 * ruler.margin)) / spaces;
    
    for (int i = 0; i < ruler.divisions; i++){
        float x = startX + i * distBetween;
        DrawLineEx( (Vector2){ x, ruler.rect.y },
                    (Vector2){ x, ruler.rect.y + ruler.rect.height * 0.46 },
                    1.0f,
                    divisionColor);
    }
}

// ___circle mark_________________________________________________________________________________________

// Retorna posicao no eixo x que corresponde ao numero digitado.
float getXFromRulerPoint(Ruler ruler, int point){
    float spacing = (ruler.rect.width - ruler.margin * 2) / (ruler.divisions - 1);
    return point * spacing + ruler.margin;
}

// Retorna numero digitado que corresponde ponto no eixo X.
int getRulerPointFromX(Ruler ruler, float x) {
    float spacing = (ruler.rect.width - ruler.margin * 2) / (ruler.divisions - 1);
    int point = (int)roundf((x - ruler.margin) / spacing);
    if (point < 0) point = 0;
    if (point >= ruler.divisions) point = ruler.divisions - 1;
    return point;
}

void spawnActiveMark(int index, int rulerPoint) {
    float x = getXFromRulerPoint(basicRuler, rulerPoint);
    circlemarks[index].currentX = x;
    circlemarks[index].targetX  = x;
    circlemarks[index].raio  = 10;
    circlemarks[index].y = ALTURA - basicRuler.rect.height - circlemarks[index].raio;
    circlemarks[index].state = CM_FREE;
    activeMarkIndex = index;
}

void updateCircleMarks(Session *game) {
    static float soundCooldown = 0.0f;
    float dt = GetFrameTime();
    if (soundCooldown > 0.0f) soundCooldown -= dt;
    Vector2 mouse = GetMousePosition();

    // mouse overlapping detection
    int detectionPadding = 400;
    bool mouseOnRuler = CheckCollisionPointRec(mouse,
        (Rectangle){basicRuler.rect.x, basicRuler.rect.y-detectionPadding,
                    basicRuler.rect.width, basicRuler.rect.height+detectionPadding});

    // --- Spawn new marker ---
    // From mouse click on ruler
    if (activeMarkIndex < 0 && mouseOnRuler && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int point = getRulerPointFromX(basicRuler, mouse.x);
        // sync input text
        input.count = 0;
        char buf[4];
        snprintf(buf, sizeof(buf), "%d", point);
        for (int i = 0; buf[i] != '\0'; i++) numberInputAdd(&input, buf[i]);
        spawnActiveMark(game->guessCount, point);
    }
    // From first digit typed
    if (activeMarkIndex < 0 && !input.isClearing && input.count > 0) {
        int point = atoi(input.text);
        spawnActiveMark(game->guessCount, point);
    }

    // --- Drive active marker ---
    if (activeMarkIndex >= 0) {
        CircleMark *m = &circlemarks[activeMarkIndex];
        int prevPoint = getRulerPointFromX(basicRuler, m->currentX);

        // Mouse drag overrides keyboard
        if (mouseOnRuler && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int point = getRulerPointFromX(basicRuler, mouse.x);
            
            // experimentando com som,isso aqui pode acabar saindo
            if (point != prevPoint && soundCooldown <= 0.0f) {
                float t = (float)point / (basicRuler.divisions - 1); // 0.0 to 1.0
                SetSoundPitch(sfxChangeMark, 0.8f + t * 0.8f); // 0.8 to 1.6
                PlaySound(sfxChangeMark);
                soundCooldown = 0.08f; // 80ms
            };

            m->targetX = getXFromRulerPoint(basicRuler, point);
            // sync input text to mouse position
            input.count = 0;
            char buf[4];
            snprintf(buf, sizeof(buf), "%d", point);
            for (int i = 0; buf[i] != '\0'; i++) numberInputAdd(&input, buf[i]);
        } else {
            // Keyboard drives target
            if (input.count > 0) {
                int point = atoi(input.text);
                if (point > basicRuler.divisions - 1) point = basicRuler.divisions - 1;
                m->targetX = getXFromRulerPoint(basicRuler, point);
            }
        }

        // Lerp currentX toward targetX
        m->currentX = flerp(m->currentX, m->targetX, 0.4f);
        m->y = ALTURA - basicRuler.rect.height - m->raio;
    }
}

void lockActiveCircleMark(void) {
    CircleMark *m = &circlemarks[activeMarkIndex];
    int point = atoi(input.text);
    m->currentX = getXFromRulerPoint(basicRuler, point);
    m->raio = 7;
    m->y = ALTURA - basicRuler.rect.height + (basicRuler.rect.height * 0.46f);
    m->state = CM_LOCKED;
}

void drawCircleMarks(Session *game) {
    // Draw all locked (confirmed) markers
    for (int i = 0; i < game->guessCount; i++) {
        CircleMark m = circlemarks[i];
        DrawCircleV((Vector2){m.currentX, m.y}, m.raio, PS_BLUE);
    }

    // Draw active marker only if one exists
    if (activeMarkIndex >= 0) {
        CircleMark m = circlemarks[activeMarkIndex];
        float lineBottom = m.y + basicRuler.rect.height * 0.46f + m.raio;
        DrawCircleV((Vector2){m.currentX, m.y}, m.raio, PS_BLUE);
        DrawLineEx((Vector2){m.currentX, m.y},
                   (Vector2){m.currentX, lineBottom},
                   2.0f, PS_BLUE);
    }
}

// ___arrow______________________________________________________________________________________________

void drawArrow(FeedbackArrow arrow, int length, int weight){

    float headOffsetX = arrow.dir ? length/2 : -length/2; // troca a posicao no eixo x dependendo da direcao
    float headAngleOffset = arrow.dir ? 0 : 180; // troca a direcao 

    // Ponta da seta, retangulo base para a ">" ponta
    Rectangle arrowHeadRect = {arrow.pos.x + headOffsetX, arrow.pos.y, weight, length / 2};
    
    // Base (sempre igual)
    DrawRectangle(arrow.pos.x - length/2, arrow.pos.y - weight/2, length, weight, PS_BLUE);
    // Metade superior
    DrawRectanglePro(arrowHeadRect,
                        (Vector2){weight / 2, weight / 2},
                        45 + headAngleOffset, PS_BLUE);
    // Metade inferior
    DrawRectanglePro(arrowHeadRect,
                        (Vector2){weight / 2, weight / 2},
                        45+90 + headAngleOffset, PS_BLUE);

}
