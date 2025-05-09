# ⚔️ Roguelike RPG - Algoritmos e Programação II

Um jogo de Roguelike na tematica de RPG de texto baseado em turnos desenvolvido em C++ no terminal do Windows. O jogador explora um mapa com inimigos, armadilhas, itens, e salas especiais — enfrentando combates táticos em turnos até chegar ao **chefão final (BOSS)**.

## 📚 Informações do Projeto

**Disciplina:** Algoritmos e Programação II  
**Turma:** 25-1 M1  
**Professor:** Alex Rese

### 👨‍💻 Desenvolvido por:

| Nome                   | E-mail                                   | Código de Pessoa |
|------------------------|------------------------------------------|------------------|
| Andrey Felsky          | andrey.8273782@edu.univali.br            | 8273782          |
| Bruno Dalbosco Rover   | bruno.8314144@edu.univali.br             | 8314144          |
| Wallacy Alvarenga      | wallacyalvarenga@univali.br              | 6916694          |

---

## 🎮 Como Jogar

- **Setas do teclado (ou W A S D)**: Movimentar pelo mapa
- **ENTER / ESPAÇO**: Confirmar ações ou entrar em combate
- **ESC**: Finaliza o jogo

---

## ⚙️ Estrutura Geral

### 🗺️ Exploração do Mapa
- O jogador aparece no centro do mapa.
- Visibilidade é limitada: só é possível enxergar ao redor do personagem.
- Itens, inimigos, chaves e armadilhas são espalhados aleatoriamente pelo mapa.

### ⚔️ Sistema de Combate
- Quando o jogador encontra um inimigo, inicia-se o **combate por turnos**.
- O combate é baseado em **iniciativa**. Quem tem maior valor, ataca primeiro.
- Ações disponíveis:
  - **ATACAR**: Tentativa de infligir dano no inimigo.
  - **DEFENDER**: Aumenta temporariamente sua defesa.
  - **ITENS**: Usa itens do inventário (ex: poção de cura, pergaminho mágico).
  - **FUGIR**: Tenta escapar do combate. Não funciona contra o BOSS.

### 👾 Inimigos
- Inimigos normais são gerados e posicionados aleatoriamente.
- Inimigos **andam sozinhos** pelo mapa em movimentos aleatórios (sem atravessar paredes).
- O Chefão é representado por um símbolo especial (ex: `char(234)`).

### 🧪 Itens
- Itens podem ser coletados e usados no combate.
- Tipos:
  - **Poção de Cura**: Recupera 1 ponto de vida.
  - **Pergaminho de Mísseis Mágicos**: Causa dano direto ao inimigo.

### ☠️ Armadilhas
- Podem causar danos aleatórios, ou até remover uma chave do jogador.
- Algumas armadilhas não causam nada, ou podem até ajudar.

### 🗝️ Chaves
- Usadas para abrir portas que bloqueiam acesso ao mapa do chefão.
- São obrigatórias para completar o jogo.

---

## 🧠 Lógica de Pontuação

Ao encerrar o jogo (ESC), a pontuação será calculada:

- Cada item coletado: `+1 ponto`
- Cada chave: `+1 ponto`
- Tempo jogado: penalidade (ex: `-1 ponto a cada 2 minutos`)
- Combates vencidos (kill): também podem ser somados (opcional)

---

## 🏁 Condições de Vitória e Derrota

### ✅ Vitória
- Vencer o chefão (Boss Room)
- Sobreviver a todos os combates e desafios

### ❌ Derrota
- Vida do jogador chega a 0
- Sair do jogo antes de vencer

---

## 💡 Destaques Técnicos

- Jogo estruturado com múltiplos arquivos (`GameMap.cpp`, `Combat.cpp`, etc.)
- Usa `Windows API` para manipulação de cursor no terminal
- Movimento inimigo em 8 direções com delay de movimento
- Interface de combate dinâmica com controle de ações
- Tratamento de inputs inválidos para evitar bugs e injustiças
- Inventário dinâmico com verificação de uso e consumo

---

## 📌 Requisitos

- Sistema operacional: **Windows**
- Compilador: **G++ compatível com Windows (ex: MinGW)**
- Terminal com suporte à manipulação de cursor (CMD, PowerShell)

---

## 🛠️ Compilação

Se estiver usando o terminal com G++:

```bash
g++ main.cpp -o rpg.exe
./rpg.exe
```