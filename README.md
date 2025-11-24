# Batalha Naval  
Projeto desenvolvido para a disciplina **PIF – Sistemas de Informação** (CESAR School).  
O objetivo é implementar o clássico jogo Batalha Naval utilizando exclusivamente os recursos básicos da linguagem C, com foco em **structs**, **ponteiros**, **alocação dinâmica de memória** e **modularização**.

---

## Integrantes do Projeto
- **Maria Clara Targino**  
- **Nádia Bordoni**  
- **Pedro Gabriel**

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
<img width="1037" height="546" alt="Captura de Tela 2025-11-23 às 21 25 33" src="https://github.com/user-attachments/assets/d578ae15-1a39-4135-a2a2-695e15b3e580" />

Testes dia 24/11:

<img width="762" height="526" alt="Captura de Tela 2025-11-23 às 23 07 14" src="https://github.com/user-attachments/assets/1f8ba7de-a6d9-4ad8-a133-64537a52f93f" />
<img width="607" height="597" alt="Captura de Tela 2025-11-23 às 23 07 22" src="https://github.com/user-attachments/assets/03a4b392-83e6-478f-b65d-44e070939894" />


---
## Roadmap do projeto

### **Fase 1 — Módulos Fundamentais**
- [x] `board`: criação, destruição, impressão e acesso a células  
- [x] `fleet`: criação da frota padrão e liberação  
- [x] Teste inicial com `main_test.c`

### **Fase 2 — Posicionamento dos Navios** 
- [ ] Posicionamento automático (aleatório)  
- [ ] Posicionamento manual via `io.c`

### **Fase 3 — Sistema de Tiros**
- [ ] Detectar navio afundado  
- [ ] Detectar fim de jogo

### **Fase 4 — Interface do Jogo**
- [ ] 

### **Fase 5 — Extensões e Melhorias**
- [ ] Tamanho de tabuleiro configurável  
- [ ] Frota customizável 
