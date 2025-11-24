# Batalha Naval  
Projeto desenvolvido para a disciplina **PIF – Sistemas de Informação** (CESAR School).  
O objetivo é implementar o clássico jogo Batalha Naval utilizando exclusivamente os recursos básicos da linguagem C, com foco em **structs**, **ponteiros**, **alocação dinâmica de memória** e **modularização**.

---

## Integrantes do Projeto
- **Maria Clara Targino**  
- **Nádia Bordoni**  
- **Pedro Gabriel**

---

## Descrição Geral do Projeto
Este projeto consiste em implementar o jogo **Batalha Naval**, seguindo as regras tradicionais:

- Cada jogador possui um tabuleiro.  
- Cada jogador posiciona navios em posições válidas.  
- Os jogadores alternam disparos até que toda a frota inimiga seja destruída.

---

## Como testar o projeto agora
Enquanto o jogo completo ainda não está implementado, utilizamos um arquivo de testes:  
`tests/main_test.c`

Ele é atualizado conforme adicionamos novas funcionalidades.

### Compilar e rodar (Mac/Linux)
No terminal, a partir da **pasta raiz do projeto**, execute:

```bash
gcc tests/main_test.c src/board.c src/fleet.c -o test
./test
```

Testes no dia 23/11:
[Colocar imagem do print do terminal]

---
## Estrutura do Projeto
src/
  board.h       # estrutura do tabuleiro
  board.c       # criar, destruir, acessar e imprimir tabuleiro

  fleet.h       # estrutura de navio e frota
  fleet.c       # criação da frota padrão e desalocação

  game.h        # (a implementar)
  game.c        # (a implementar)

  io.h          # (a implementar)
  io.c          # (a implementar)

  random.h      # (a implementar)
  random.c      # (a implementar)

tests/
  main_test.c   # arquivo para testes incrementais

## Roadmap do projeto

### ✔️ **Fase 1 — Módulos Fundamentais**
- [x] `board`: criação, destruição, impressão e acesso a células  
- [x] `fleet`: criação da frota padrão e liberação  
- [x] Teste inicial com `main_test.c`

### 🔜 **Fase 2 — Posicionamento dos Navios** 
- [ ] Posicionamento automático (aleatório)  
- [ ] Posicionamento manual via `io.c`

### 🔜 **Fase 3 — Sistema de Tiros**
- [ ] Detectar navio afundado  
- [ ] Detectar fim de jogo

### 🔜 **Fase 4 — Interface do Jogo**
- [ ] 

### 🔜 **Fase 5 — Extensões e Melhorias**
- [ ] Tamanho de tabuleiro configurável  
- [ ] Frota customizável 
