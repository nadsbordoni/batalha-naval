# Batalha Naval
Projeto desenvolvido para a disciplina **PIF – Sistemas de Informação** (CESAR School).

## Integrantes
- Maria Clara Targino  
- Nádia Bordoni  
- Pedro Gabriel Paes

## Resumo
Implementação do jogo Batalha Naval em C, com foco em:
- structs e ponteiros
- alocação dinâmica
- modularização (módulos: board, fleet, game, io, random)

## Funcionalidades
- Tamanho de tabuleiro configurável (NxN, quadrado)
- Posicionamento de navios:
  - Automático (aleatório)
  - Manual (via CLI)
- Jogo por turnos com exibição de:
  - Tabuleiro próprio (com navios)
  - Visão parcial do tabuleiro inimigo (apenas hits/misses)
- Exibição de vencedor e estatísticas ao final (tiros, acertos)

## Como compilar
Na raiz do projeto:
```bash
make run
```
---
## Testes de funcionamento
Testes parciais foram feitos 23/11 e no dia 24/11:
![alt text](image.png)
![alt text](image-1.png)

Testes finais feitos 01/12:
1. Menu inicial:
<img width="572" height="468" alt="Captura de Tela 2025-12-02 às 01 34 13" src="https://github.com/user-attachments/assets/9da9d189-d93f-4bce-92f7-d7b64b5f1030" />

2. Configurações:
<img width="573" height="466" alt="Captura de Tela 2025-12-02 às 01 33 55" src="https://github.com/user-attachments/assets/15e09fb4-2286-4a22-99cd-b226b669134c" />
<img width="568" height="470" alt="Captura de Tela 2025-12-02 às 01 34 04" src="https://github.com/user-attachments/assets/d8585612-3a6e-4017-b94d-81569529bf43" />

3. Jogadores e posicionamento de navios:
<img width="570" height="467" alt="Captura de Tela 2025-12-02 às 01 34 49" src="https://github.com/user-attachments/assets/fe52d84e-ab9f-4c2d-b94b-22853559a0b8" />
<img width="569" height="465" alt="Captura de Tela 2025-12-02 às 01 35 17" src="https://github.com/user-attachments/assets/29436aa3-162d-45d2-97a5-6b7791806526" />
<img width="571" height="465" alt="Captura de Tela 2025-12-02 às 01 35 27" src="https://github.com/user-attachments/assets/701cce62-043e-41a4-b6e5-28fa6c0a1256" />
<img width="336" height="418" alt="Captura de Tela 2025-12-02 às 01 39 04" src="https://github.com/user-attachments/assets/018e8f70-75c3-44f9-b9e6-e3a4767c8afc" />

a. Testes envolvendo tratamento de erros no posicionamento de navios:
<img width="510" height="172" alt="Captura de Tela 2025-12-02 às 01 36 39" src="https://github.com/user-attachments/assets/95bc4ed5-1551-4c05-9607-e8130caa9a7b" />

4. Vídeo com Jogabilidade:
https://github.com/user-attachments/assets/9ec9371b-29ac-4a0c-bac5-ec9aab11be32

