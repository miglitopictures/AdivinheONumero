# Pablo Software's Decimais

Um jogo casual de adivinhar números (de 0 a 100) desenvolvido em C, o primeiro da Pablo Software.

## O que o programa DEVE fazer?
- O programa deve randomizar um valor dentro de um intervalo definido.
- O programa deve receber inputs (tentativas) do jogador através da interface.
- O programa deve fornecer feedback claro se a tentativa foi acima ou abaixo do número secreto.
- O programa deve oferecer uma interface visual amigável e funcional.

## Backlog do Projeto ([Trello](https://trello.com/b/R0YXpUzE/organizacao-projeto-2))
![Board Trello Projeto 2](./assets/github/BOARD-TRELLO_PROJETO-2.png)

## [Histórias de Usuário](docs/HISTORIAS.md)
Concebemos 11 histórias de usuário (funcionalidades), detalhadas em HISTORIAS.md. A partir delas, organizamos nosso projeto e backlog.

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
Para isso, preparamos algumas intruções que podem ser localizadas em **[INSTRUCOES.md](docs/INSTRUCOES.md)**.

### O que cada arquivo faz? :o
Se tiver duvidas sobre a organização do projeto ou sobre a funcionalidade de arquivos, consulte **[ARQUIVOS.md](docs/ARQUIVOS.md)**.

*Qualquer dúvida, contatar miglito ou lucas bonfim.

---

## Equipe
- [Lucas Bonfim](https://github.com/l-bonfim) (Frontend)
- [Lucas Carvalho](https://github.com/J4keless) (Backend)
- [Lucas Valença](https://github.com/LucasGuilhermeValenca) (Backend)
- [Miguel Duarte](https://github.com/miglitopictures) (Frontend)
- [Pablo Tamborini](https://github.com/PTN81) (Backend)
- [Rodrigo Montenegro](https://github.com/rodrigomscmontenegro) (Backend)

## Sobre
Esse é o projeto unificador do segundo período do curso de ADS do **CESAR School**. Desenvolvido para aplicar conceitos de lógica de programação, estruturas de dados e interfaces gráficas em C.
