# Guia de Arquivos do Projeto

Este documento detalha a responsabilidade de cada arquivo no repositório, ajudando a entender a separação entre a lógica do jogo e as interfaces (Raylib e Terminal).

---
## `./` (Scripts de Build)
![Static Badge](https://img.shields.io/badge/pasta_raiz-8A2BE2)

Arquivos para automação e configuração.

### **`build_linux.sh` / `build_mac.sh` / `build_win.bat`**
Scripts que compilam todos os arquivos `.c` e linkam com a Raylib correta de forma automática.
### **`INSTRUCOES.md`**
Guia rápido de como clonar, criar branches e compilar o projeto.
### **`ARQUIVOS.md`**
Você está aqui! Um arquivo que explica todos os outros arquivos.
### **`README.md`**
Visão geral do projeto e status do desenvolvimento.
### **`.gitignore`**
Lista arquivos que o Git deve ignorar (como o executável final `game`).

---

## `include/` (Headers)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Contém as definições de estruturas e as "assinaturas" das funções. É o contrato que diz o que cada parte do código pode fazer.

### **`gamestate.h`**
Define o "objeto" do jogo. Contém a estrutura `struct GameState`, que armazena todas as variáveis importantes (numeroSecreto, tentativas, historicoTentativas e estados como `STATE_MENU` ou `STATE_PLAYING`).
### **`logic.h`**
Declara as funções matemáticas e de processamento, como a inicialização do sorteio e o processamento de palpites.
### **`ui.h`**
Declara os pontos de entrada para as duas interfaces disponíveis: Terminal e Raylib.
### **`raylib.h`**
Header da biblioteca externa utilizada para a interface gráfica.

---

## `src/` (Código Fonte)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Onde a mágica acontece. Aqui o código é implementado de fato.

### **`main.c`**
O ponto de partida. Ele decide, através de argumentos de linha de comando (como `-term`), qual interface deve ser iniciada.
### **`logic.c`**
O "cérebro" do jogo. Não possui código visual. Cuida de gerar números aleatórios, verificar se o palpite está perto (Temperatura) e atualizar o status do jogo (GameState).
### **`ui_raylib.c`**
Implementa a interface gráfica usando a biblioteca Raylib. Cuida da janela, desenho de textos, caixas de input e feedback visual.
### **`ui_terminal.c`**
Implementa a interface de texto simples. Utiliza `printf` e `scanf` para permitir que o jogo rode inteiramente dentro do terminal.

---

## `lib/` (Binários)
![Static Badge](https://img.shields.io/badge/pasta-blue)

Contém as bibliotecas pré-compiladas da Raylib para que o projeto funcione em diferentes sistemas operacionais sem que você precise instalar nada externamente.
* `linux/`, `mac/`, `windows/`: Cada pasta contém o arquivo `libraylib.a` correspondente ao seu SO.

---

## `assets/`
![Static Badge](https://img.shields.io/badge/pasta-blue)

Destinada a arquivos externos como imagens, texturas ou sons. Atualmente contém os assets utilizados no GitHub (como a imagem do Trello).

---

### Resumo da Arquitetura
Para o esse projeto decidimos separar as preopações:
1.  **Lógica (`logic.c`)** não sabe que existe interface.
2.  **Interface (`ui_*.c`)** não sabe como o cálculo de temperatura é feito, ela apenas pede para a lógica processar e exibe o resultado.
3.  **Estado (`gamestate.h`)** é a ponte que carrega os dados entre todos eles.