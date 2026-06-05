Abaixo está um relatório sobre nosso jogo analisando o cumprimento das 10 Heurísticas de Nielssen para a Disciplina de IHC (Interfaces Humano-Computador):

## 1. Visibilidade do status do sistema
*Status: OK*
Utilizamos componentes visuais dedicados a manter o jogador sempre informado sobre o estado atual da partida. A Linha Numérica exibe o histórico visual de palpites anteriores, a Barra de Contagem indica o número de tentativas restantes, a Seta de Feedback sinaliza a direção do palpite em relação ao número correto, e elementos de confirmação de input e acerto reforçam cada ação do usuário em tempo real.
## 2. Relação entre sistema e o mundo real
*Status: Parcial*
A natureza inerentemente abstrata de números apresentou um desafio para o cumprimento pleno desta heurística. Como resposta, implementamos a Linha Numérica como metáfora visual central do jogo — ela materializa o espaço abstrato dos números em um objeto familiar e espacial, aproximando a interação do mundo real. Apesar disso, reconhecemos que a representação ainda é limitada e que outros elementos do jogo permanecem abstratos para o usuário.
## 3. Liberdade e controle do usuário
*Status: Parcial*
Implementamos "saídas de emergência" em alguns momentos da experiência, como o botão de voltar presente nos menus de navegação. No entanto, não conseguimos oferecer essa liberdade durante o loop principal do jogo: uma vez iniciada uma rodada, o jogador não dispõe de opções para pausar, reiniciar ou abandonar a partida sem encerrar o processo. Essa lacuna representa uma perda de controle significativa em um momento crítico da experiência.
## 4. Padrões e consistências
*Status: Parcial*
Estabelecemos um padrão visual entre as telas do jogo, mantendo coerência no desenho dos botões, na paleta de cores e na tipografia, de modo a reduzir a carga cognitiva do jogador. Contudo, identificamos inconsistências no refinamento visual: a borda reta dos botões contrasta com o estilo arredondado e circular dos marcadores na Linha Numérica, criando uma dissonância estética que não foi resolvida a tempo. Tais inconsistências, embora pontuais, comprometem a sensação de unidade da interface.
## 5. Prevenção a erros
*Status: Não Cumprido*
Não implementamos mecanismos de prevenção a erros durante o input de palpites: ao confirmar uma tentativa, o jogador não recebe nenhuma etapa de verificação ou confirmação antes que o palpite seja registrado definitivamente. Essa foi uma decisão consciente de jogabilidade, visando aumentar o desafio e a tensão da partida. No entanto, reconhecemos que essa escolha abre espaço para erros acidentais, especialmente para jogadores menos experientes, que podem confirmar um valor por engano sem a possibilidade de desfazê-lo.
## 6. Reconhecimento em vez de memorização
*Status: Não Cumprido*
O jogo exige que o jogador memorize ou mentalmente rastreie os palpites anteriores e os feedbacks recebidos para tomar decisões mais precisas nas rodadas seguintes. Embora a Linha Numérica ofereça um registro visual dos palpites passados dentro de uma mesma partida, não fornecemos informações de contexto suficientes em outros momentos da interface — como indicações visíveis das regras, do intervalo válido de palpites ou dos feedbacks já recebidos de forma consolidada. O jogador precisa reter e relacionar essas informações por conta própria, contrariando o princípio de favorecer o reconhecimento sobre a memorização.
## 7. Flexibilidade e eficiência de uso
*Status: Ok*
Oferecemos dois modos de interação distintos: o jogador pode realizar seus palpites utilizando o mouse diretamente sobre a Linha Numérica, ou utilizar o teclado para inserir valores numericamente. Essa dualidade contempla diferentes perfis e preferências de usuário, tornando o jogo mais acessível para iniciantes, que tendem a preferir a interação visual e direta da linha, e mais eficiente para jogadores experientes, que podem preferir a precisão e agilidade do teclado.
## 8. Estética e design minimalista
*Status: OK*
Adotamos uma paleta de cores enxuta e funcional, com o cinza como base neutra da interface e destaques em azul, vermelho e verde reservados para elementos de maior importância — como feedback de proximidade e confirmação de acerto. Essa escolha reforça a proposta minimalista do jogo, evitando poluição visual e mantendo o foco do jogador na interação central: a Linha Numérica e os palpites.
## 9. Ajudar o usuário a reconhecer, diagnosticar e recuperar-se de erros
*Status: Não Cumprido*
Não implementamos mensagens de erro claras ou orientações de recuperação para situações problemáticas durante o jogo. Quando o jogador comete um palpite inválido ou encerra uma partida sem acertar, a interface não oferece explicações sobre o que ocorreu nem caminhos evidentes para retomar ou corrigir a situação. A ausência de mensagens de erro descritivas e acionáveis deixa o jogador sem referência para entender suas falhas e agir sobre elas, o que pode gerar frustração — especialmente em usuários menos familiarizados com o jogo.
## 10. Ajuda e documentação
*Status: Parcial*mente
O jogo não conta com uma tela de tutorial, manual ou seção de ajuda in-game. As regras e mecânicas de Numeral não são explicitadas em nenhum momento da experiência, o que significa que o jogador precisa descobri-las por conta própria durante o jogo. Em parte, essa ausência é compensada pela simplicidade da premissa — adivinhar um número entre 0 e 100 é uma mecânica intuitiva — e pelo feedback visual imediato da Linha Numérica, que orienta o jogador de forma implícita. Ainda assim, funcionalidades como os diferentes modos de jogo e a interação por teclado podem passar despercebidas sem alguma forma de documentação ou onboarding.