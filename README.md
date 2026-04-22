# Adivinhe O Número

Um jogo de adivinhar números (de 0 a 100) desenvolvido em C.

## O que o programa DEVE fazer?
- O programa deve randomizar um valor dentro de um intervalo definido.
- O programa deve receber inputs (tentativas) do jogador através da interface.
- O programa deve fornecer feedback claro se a tentativa foi acima ou abaixo do número secreto.
- O programa deve oferecer uma interface visual amigável e funcional.

## Backlog do Projeto ([Trello](https://trello.com/b/R0YXpUzE/organizacao-projeto-2))
![Board Trello Projeto 2](./assets/github/BOARD-TRELLO_PROJETO-2.png)

## Histórias de Usuário

### **UH0: Pontuação Dinâmica
![High](https://img.shields.io/badge/Prioridade-Alta-red)
**Descrição:** Como jogador, quero que minha pontuação (score) considere diversos fatores da partida e não apenas o acerto, para que cada rodada seja mais significativa e única.

**Conversa:** O sistema deve considerar diversos fatores como quantidadade de tentativas, tempo, proximidade de palpites, estatégias de jogo, sistema numérico selecionado, sequencias de acerto (etc…). Criando, ao final da partida, um score significativo, promovendo rejogabilidade e engajamento.

**Critérios de Confirmação:**

- Mostrar o score ao final da partida.
- Mostrar visualmente sua pontuação durante o jogo.
  
### **UH1: Seleção de Dificuldade**
![High](https://img.shields.io/badge/Prioridade-Alta-red)
**Descrição:** Como um jogador, eu quero ter opções de níveis de dificuldade para que eu me sinta desafiado e motivado a continuar jogando.

**Conversa:** Permitir que o usuário escolha entre diferentes níveis de intensidade antes de iniciar a partida.

**Critérios de Confirmação:**

- O jogo deve apresentar variações claras de dificuldade (10,100,1000,10000).
- A pontuação deve ser proporcional ao nível de dificuldade escolhido.

### **UH2: High Score**
![High](https://img.shields.io/badge/Prioridade-Alta-red)
**Descrição:** Como jogador, eu quero que minha melhor pontuação seja salva para que eu possa tentar superá-la em futuras partidas.

**Conversa:** O sistema deve comparar o resultado atual com o histórico salvo localmente e atualizar caso o novo recorde seja maior.

**Critérios de Confirmação:**

- O recorde (Best Score) deve permanecer salvo mesmo após fechar e abrir o jogo.
- Exibir uma mensagem especial de "Novo Recorde!" quando a pontuação anterior for superada.

### **UH3: Modo Arcade**
![High](https://img.shields.io/badge/Prioridade-Alta-red)
**Descrição:** Como um jogador, eu quero um modo arcade para experienciar uma progressão técnica ao longo do tempo.

**Conversa:** Um modo de campanha ou progressão linear onde as fases se tornam gradualmente mais complexas conforme o jogador avança.

**Critérios de Confirmação:**

- Desbloquear novos níveis após concluir o anterior.
- Mostrar o progresso do jogador ou lista de níveis.

### **UH4: Curiosidades ao Acertar**
![High](https://img.shields.io/badge/Prioridade-Alta-red)
**Descrição:** Como jogador, eu quero aprender curiosidades sobre o número ao acertar, tornando a experiência mais educativa e divertida.

**Conversa:** O sistema deve mostrar curiosidades temáticas (história, matemática, pernambuco, filmes).

**Critério de Confirmação:**

- Exibir curiosidade única para cada número no final da rodada.

### **UH5: Multiplayer Local**
![Medium](https://img.shields.io/badge/Prioridade-Média-yellow)
**Descrição:** Como jogador, quero poder jogar contra meus colegas, para ter uma experiencia em grupo mais divertida.

**Conversa:** O sistema deve ter um modo multiplayer competitivo ou cooperativo.

**Critérios de Confirmação:**

- ??????

### **UH6: Sistemas Numéricos**
![Medium](https://img.shields.io/badge/Prioridade-Média-yellow)
**Descrição:** Como jogador pro e competitivo, quero poder jogar utilizando outros sistemas numéricos menos comuns (binário, hexadecima, romano) em troca de bonus de pontuação, para ter uma experiência mais desafiador.

**Conversa:** O sistema deve permitir a seleção de diferentes sistemas numéricos (binário, hexadecimal, romanos), mantento todas as suas demais funcionalidades, em troca de bonus na pontuação.

**Critérios de Confirmação:**

- Todas as demais funcionalidades devem continuar funcionando, independentemente do sistema numérico selecionado.

### **UH7: Frenesi (Sequencia de Acertos)**
![Medium](https://img.shields.io/badge/Prioridade-Média-yellow)
**Descrição:** Como jogador, quero ser beneficiado por sequências contínuas de acerto.

**Conversa:** O sistema deve reconhecer vitórias repetidas e oferecer bonus pelo “streak“.

**Critérios de Confirmação:**

- O streak deve funcionar em todas as modalidades singleplayer do jogo.

### **UH8: Operações Matemáticas**
![Low](https://img.shields.io/badge/Prioridade-Baixa-blue)
**Descrição:** Como jogador, quero poder manipular meu palpite com operadores matemáticos como (+,-,* e /), para ter uma experiência mais educativa e desafiadora.

**Conversa:** O sistema deve permitir a entrada de operações matemáticas para manipular o valor do palpite.

**Critérios de Confirmação:**

- Todas as demais funcionalidades devem continuar funcionando.

### **UH9: 1D Linha Numérica**
![Low](https://img.shields.io/badge/Prioridade-Baixa-blue)
**Descrição:** Como jogador, quero poder interagir com o jogo por meio da linha numérica, para ter uma experiência mais visual.

**Conversa:** O sistema mostra os palpites e, ao acertar, o numero secreto por meio de uma interface de linha numerica.

**Critérios de Confirmação:**

- Mostrar a linha numérica interativa em tela, marcando os palpites já feitos.

### **UH10: 2D Plano Cartesiano**
![Low](https://img.shields.io/badge/Prioridade-Baixíssima-blue)
### Description

**Descrição:** Como jogador, quero poder interagir com o jogo por meio do plano cartersiano, para ter uma experiência mais visual e desafiadora.

**Conversa:** O sistema deve sortear dois numeros secretos, mostrando os palpites e, ao acertar, a coordenada secreta por meio do plano cartesiano.

**Critérios de Confirmação:**

- ?????

## Como rodar no projeto?

Primeiro, clone este repositório para sua máquina:
```bash
git clone https://github.com/miglitopictures/AdivinheONumero
```

Agora, identifique seu sistema operacional e siga as instruções abaixo para compilar o código.

<details>
  <summary><b>Compilando no Linux</b></summary>
  
1. Dê permissão de execução para o script:
   ```bash
   chmod +x build_linux.sh
   ```
2. Execute o script de build para compilar o programa:
   ```bash
   ./build_linux.sh
   ```
3. Rode o executável:
   ```bash
   ./game
   ```
</details>

<details>
  <summary><b>Compilando no Windows</b></summary>
  
Se você estiver usando o **MinGW/GCC** no Windows:
1. Abra o PowerShell ou CMD na pasta do projeto.
2. Execute o script de build para compilar o programa:
   ```cmd
   .\build_windows.bat
   ```
3. Rode o executável:
   ```bash
   .\game.exe
   ```
*Nota: Certifique-se de que o caminho do seu compilador (bin) esteja nas Variáveis de Ambiente (PATH).*
</details>

<details>
  <summary><b>Compilando no Mac</b></summary>
  
O processo é similar ao Linux, utilizando o terminal:
1. Garanta a permissão:
   ```bash
   chmod +x build_mac.sh
   ```
2. Execute o script de build para compilar o programa:
   ```bash
   ./build_mac.sh
   ```
3. Rode o executável:
   ```bash
   ./game
   ```
</details>

## Como trabalhar no projeto?
Para isso, preparamos algumas intruções que podem ser localizadas em **[INSTRUCOES.md](INSTRUCOES.md)**.

### O que cada arquivo faz? :o
Se tiver duvidas sobre a organização do projeto ou sobre a funcionalidade de arquivos, consulte **[ARQUIVOS.md](ARQUIVOS.md)**.

*Qualquer dúvida, contatar miglito ou lucas bonfim.

---

## Equipe
- [Lucas Bonfim](https://github.com/l-bonfim) (Frontend)
- [Lucas Carvalho](https://github.com/J4keless) (Backend)
- [Lucas Valença](https://github.com/LucasGuilhermeValenca) (Frontend)
- [Miguel Duarte](https://github.com/miglitopictures) (Frontend)
- [Pablo Tamborini](https://github.com/PTN81) (Backend)
- [Rodrigo Montenegro](https://github.com/rodrigomscmontenegro) (Backend)

## Sobre
Esse é o projeto unificador do segundo período do curso de ADS do **CESAR School**. Desenvolvido para aplicar conceitos de lógica de programação, estruturas de dados e interfaces gráficas em C.
