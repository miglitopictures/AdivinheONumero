
# Instruções de Colaboração

Este guia servirá para orientar o fluxo de trabalho e garantir que todos estejamos na mesma página.

## Como trabalhar no projeto?

Primeiro, clone este repositório para sua máquina:
```bash
git clone https://github.com/miglitopictures/Numeral
```

### Gerenciamento de Branches

#### Não tenho uma branch de trabalho. Como crio uma?
Crie sua branch de trabalho e mude para ela simultaneamente com:
```bash
git checkout -b nome-da-sua-branch
```
Publique a branch no GitHub:
```bash
git push -u origin nome-da-sua-branch
```

#### Meu colega já fez uma branch, como faço para trabalhar nela?
Primeiro, atualize sua lista de referências remotas e depois mude para a branch:
```bash
git fetch origin
git checkout nome-da-branch-do-colega
```

---

### Sincronização e Fluxo

#### Tudo pronto, e agora?
Sempre que começar a trabalhar, atualize sua branch local com o repositório remoto no GitHub para evitar conflitos:
*(Pode ignorar se acabou de criar a branch local agora).*

```bash
git pull origin nome-da-sua-branch
```

#### Fazendo Commits e Subindo Código
Quando terminar uma funcionalidade ou corrigir um bug:

1. **Adicione as mudanças:** `git add .`
2. **Gere o snapshot:** `git commit -m "Explique o que você fez de forma breve"`
3. **Envie para o servidor:** `git push origin nome-da-sua-branch`

---

### Como eu testo o jogo?

Identifique seu sistema operacional e siga as instruções abaixo para compilar o código usando o compilador GCC.

#### Compilando no Linux
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

#### Compilando no Windows
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

#### Compilando no Mac
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
---

### Raylib
Usamos o raylib, uma livraria minima em C para a criação de jogos. Ele não tem dependencias externas e é bem pequeno.

#### Como instalo o Raylib?
**Não precisa!** O Raylib já está incluído no repositório (estático ou via headers na pasta `include/lib`). Isso simplifica o processo de build para todos.

#### O VSCode não reconheceu o Raylib?
Se o IntelliSense mostrar "erros" (linhas vermelhas) mesmo o código compilando, siga estes passos:

1. Pressione `Ctrl + Shift + P` (ou `Cmd + Shift + P` no Mac).
2. Digite e selecione: **C/C++: Edit Configurations (UI)**.
3. Procure o campo **Include path**.
4. Adicione o caminho da pasta onde estão os headers da Raylib no projeto (ex: `${workspaceFolder}/include/**`).
5. No campo **Compiler path**, verifique se o caminho para o seu `gcc` ou `clang` está correto.

---

### O que cada arquivo faz? :o
Antes de começar a explorar o repositório, recomendamos se familiarizar com o propósito de cada um dos arquivos que existem nele, consulte o **[sumário de arquivos](ARQUIVOS.md)**.