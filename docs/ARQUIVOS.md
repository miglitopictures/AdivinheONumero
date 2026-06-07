# Guia de Arquivos do Projeto

Este documento detalha a responsabilidade de cada arquivo no repositório, ajudando a entender a separação entre as três camadas do projeto: Interface, Lógica e Dados.

---

## `./` (Raiz do Projeto)
![Static Badge](https://img.shields.io/badge/pasta_raiz-8A2BE2)

Arquivos de automação, configuração e documentação geral.

### **`build_linux.sh` / `build_mac.sh` / `build_win.bat`**
Scripts que compilam todos os arquivos `.c` das subpastas de `src/` e linkam com a Raylib correta de forma automática.

### **`README.md`**
Visão geral do projeto, motivação de design, descrição da interface numérica, arquitetura em camadas, histórias de usuário e instruções de compilação para cada plataforma.

### **`.gitignore`**
Lista arquivos que o Git deve ignorar (como o executável final `game` e `game.exe`).

---

## `docs/` (Documentação)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Contém toda a documentação do projeto.

### **`ARQUIVOS.md`**
Você está aqui! Um arquivo que explica todos os outros arquivos do repositório.

### **`CONTRIBUTING.md`**
Guia de colaboração: como clonar o repositório, criar e gerenciar branches, fazer commits, compilar o projeto e configurar o VSCode para reconhecer a Raylib.

### **`HISTORIAS.md`**
Descreve as 11 histórias de usuário (UH0 a UH10) que guiaram o desenvolvimento, com descrição, critérios de confirmação e prioridade de cada funcionalidade.

### **`HEURUSTICAS.md`**
Relatório de avaliação do jogo com base nas 10 Heurísticas de Nielsen (IHC), com status de cumprimento e justificativa para cada uma.

---

## `include/` (Headers)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Contém as definições de estruturas e as assinaturas das funções. É o contrato que diz o que cada parte do código pode fazer.

### **`logic.h`**
Define todos os tipos do jogo (enums e structs) e declara todas as funções das camadas de Lógica e Dados.

**Enums:** `State` (estados do ciclo de vida), `Mode` (Normal, Arcade, Coop), `Difficulty`, `NumericalSystem`, `Temperature` (Cold/Warm/Hot), `Player`.

**Structs:** `Timer`, `Session` (estado completo da sessão), `DadosHighscore`, `ListaHighscores`, `DadosPartida`, `Stats`.

**Funções declaradas por arquivo de implementação:**
- `logic.c` — `iniciarJogo`, `processarTentativa`, `processarGameover`, `atualizarTimer`
- `score.c` — `processarTemperatura`, `atualizarTempoRealScore`, `calcularPalpiteScore`
- `game_data.c` — `salvarFinalDePartida`, `atualizarHighscore`, `checarHighscore`, `coletarHighscores`, `coletarEstatisticas`, `buscarCuriosidade`, `configurarCuriosidade`, `atualizarNomePlayer`
- `rng.c` — `resetarRandomSeed`, `numeroAleatorio`, `somaMedRec`, `somaDesvRec`

### **`ui.h`**
Define todos os tipos da interface (enums e structs), declara as variáveis globais e todas as funções da camada de Interface.

**Enums:** `MenuState` (Logo, Main, Modes, Stats, Ranking), `ButtonState`, `CircleMarkState`.

**Structs:** `DigitInput`, `Button`, `Option`, `OptionPicker`, `Ruler`, `CircleMark`, `FeedbackArrow`, `TextInput`.

**Variáveis globais:** sons (`sfxChangeMark`, `sfxSelectSynth`, etc.), paleta de cores (`PS_BLACK`, `PS_BLUE`, etc.), fonte (`font`), componentes da UI (`basicRuler`, `circlemarks[]`, `input`, `arrow`, `menuState`, botões, pickers).

**Funções declaradas por arquivo de implementação:**
- `zgame_loop.c` — `startRaylibMode`
- `math_utils.c` — `ilerp`, `flerp`, `imap`, `fmap`
- `number_input.c` — `numberInputAdd`, `handleKeysNumberInput`, `updateNumberInput`, `clearAnimNumberInput`, `clearInstantNumberInput`, `drawAnimatedNumberInput`
- `components.c` — funções do `Ruler`, `CircleMark`, barra de score (`drawTopBar`) e `FeedbackArrow`
- `ui_menu.c` — `Button`, `OptionPicker`, `initMenu`, `updateMenu`, `drawMenu`
- `ui_playing.c` — `updatePlaying`, `drawPlaying`
- `ui_end.c` — `updateWin`, `drawWin`, `updateGameover`, `drawGameover`, `updateHighscoreScreen`, `drawHighscoreScreen`, `drawCoopPlacar`

### **`raylib.h`**
Header da biblioteca externa Raylib utilizada para janela, rendering, áudio e input.

---

## `src/` (Código Fonte)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Onde a implementação acontece. O código está dividido em subpastas por camada. O build compila `src/main.c` e todos os `.c` dentro das subpastas (`src/*/*.c`).

### **`main.c`**
Ponto de entrada do programa. Inicializa a `Session` e chama `startRaylibMode()` para entregar o controle à camada de Interface.

---

### `src/logic/` — Camada de Lógica
![Static Badge](https://img.shields.io/badge/camada-Lógica-green)

O "cérebro" do jogo. Não possui código visual e não lê arquivos. Manipula o estado da sessão (`Session *game`) via ponteiro.

#### **`logic.c`**
Funções centrais do game loop: `iniciarJogo` (reseta a `Session` para valores padrão), `processarTentativa` (avalia o palpite, atualiza histórico e checa vitória), `processarGameover` (verifica se o score zerou) e `atualizarTimer` (decrementa o timer do modo Coop).

#### **`score.c`**
Cálculos de pontuação e feedback de proximidade: `processarTemperatura` (define Cold/Warm/Hot com base na distância do palpite ao alvo), `atualizarTempoRealScore` (drena o score progressivamente a cada frame) e `calcularPalpiteScore` (retorna a penalidade de pontos por palpite errado).

#### **`rng.c`**
Geração de aleatoriedade: `resetarRandomSeed` (alimenta o `srand` com o tempo atual) e `numeroAleatorio` (retorna um inteiro pseudo-aleatório no intervalo `[min, max]`). Também contém `somaMedRec` e `somaDesvRec`, funções recursivas para cálculo de média e desvio padrão do histórico de tentativas.

#### **`game_data.c`**
Ponto central de persistência: grava dados da rodada finalizada em `partidas.txt` (`salvarFinalDePartida`), gerencia o ranking em `highscores.txt` (`atualizarHighscore`, `checarHighscore`, `coletarHighscores`), lê o histórico de partidas e calcula estatísticas (`coletarEstatisticas`), e busca a curiosidade do número sorteado em `curiosidades.txt` (`buscarCuriosidade`, `configurarCuriosidade`, `atualizarNomePlayer`).

---

### `src/ui/` — Camada de Interface
![Static Badge](https://img.shields.io/badge/camada-Interface-blue)

Implementa toda a interface gráfica usando a Raylib. Não realiza cálculos de lógica — consome o estado em `Session *game` e chama funções das outras camadas quando necessário.

#### **`zgame_loop.c`**
Inicializa a janela Raylib, carrega recursos (fonte, sons), roda o loop principal (`BeginDrawing` / `EndDrawing`) e encerra tudo ao sair. Prefixado com `z` para ser compilado por último.

#### **`math_utils.c`**
Utilitários matemáticos de interpolação e mapeamento usados pelas animações da UI: `ilerp`, `flerp`, `imap` e `fmap`.

#### **`number_input.c`**
Implementação do `DigitInput`: captura de teclas, adição de dígitos, limpeza animada/instantânea e desenho animado dígito-a-dígito.

#### **`components.c`**
Componentes visuais reutilizáveis: criação e desenho do `Ruler` (linha numérica), spawn/update/draw das `CircleMark`s (marcadores de palpite), barra de score (`drawTopBar`) e `FeedbackArrow` (seta direcional pós-palpite).

#### **`ui_menu.c`**
Telas de menu: lógica e desenho de `Button` e `OptionPicker`, e as funções `initMenu`, `updateMenu` e `drawMenu` que cobrem os estados `LOGO`, `MAIN`, `MODES`, `STATS` e `RANKING`.

#### **`ui_playing.c`**
Tela de jogo ativo: `updatePlaying` (processa input do jogador, atualiza marcadores e score em tempo real) e `drawPlaying` (desenha régua, marcadores, input animado e seta de feedback).

#### **`ui_end.c`**
Telas de fim de rodada: vitória (`updateWin`, `drawWin` — exibe o número acertado, trivia e estatísticas), derrota (`updateGameover`, `drawGameover`), entrada de nome para o ranking (`updateHighscoreScreen`, `drawHighscoreScreen`) e placar do modo Coop (`drawCoopPlacar`).

---

## `data/`
![Static Badge](https://img.shields.io/badge/pasta-blue)

Arquivos de dados lidos e escritos pelo jogo em tempo de execução.

* **`curiosidades.txt`** — banco de curiosidades associadas a cada número, exibidas ao acertar.
* **`highscores/*.txt`** — ranking persistente dos maiores scores por modo e dificuldade.
* **`partidas.txt`** — histórico de partidas finalizadas, usado para calcular as estatísticas.

---

## `lib/` (Binários)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Contém as bibliotecas pré-compiladas da Raylib para que o projeto funcione sem instalação externa.

* `linux/`, `mac/`, `windows/`: cada pasta contém o `libraylib.a` correspondente ao SO.

---

## `assets/`
![Static Badge](https://img.shields.io/badge/pasta-blue)

Arquivos de mídia usados pelo jogo e pela documentação.

* `assets/github/` — imagens usadas no `README.md` (tela inicial, thumbnail do vídeo, captura do Trello, protótipo do Figma).

---

## Resumo da Arquitetura

O projeto separa responsabilidades em três camadas independentes que conversam via `Session *game`:

| Camada | Pasta | Responsabilidade |
|---|---|---|
| **Interface** | `src/ui/` | Renderização, animações e captura de input do jogador |
| **Lógica** | `src/logic/` | Game loop, RNG, temperatura, score, estatísticas e persistência de dados |
| **Data** | `data/*.txt` | Dados das partidas, highscores e curiosidades |

O **estado** (`Session *game`, definido em `logic.h`) é a ponte que carrega os dados entre todas elas.