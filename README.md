# Simulação de Eventos Discretos – The Boys

Projeto final da disciplina **Programação 1**, que consiste na implementação de uma simulação de eventos discretos inspirada no universo da série **:contentReference[oaicite:1]{index=1}**.

## 📌 Sobre o projeto

O programa simula um mundo composto por heróis, bases e missões ao longo de um período de tempo.  
Os heróis possuem habilidades, velocidade, paciência e experiência, e se deslocam entre bases enquanto aguardam oportunidades de participar de missões.

As missões exigem conjuntos específicos de habilidades e são realizadas por equipes de heróis presentes em uma base. A simulação acompanha a evolução desse sistema e gera estatísticas ao final da execução.

## ⚙️ Principais conceitos utilizados

- Simulação de eventos discretos
- Estruturas de dados (filas, conjuntos e filas de prioridade)
- Gerenciamento de eventos ao longo do tempo
- Modelagem de entidades e estados de um sistema

## 🧩 Entidades da simulação

O sistema é composto por quatro tipos principais de entidades:

- **Heróis** – possuem habilidades, velocidade, paciência e experiência
- **Bases** – locais onde heróis se encontram e formam equipes
- **Missões** – tarefas que exigem conjuntos específicos de habilidades
- **Mundo** – estrutura que reúne todas as entidades e controla o tempo da simulação

## 🔄 Eventos da simulação

A evolução do sistema ocorre através de eventos processados em ordem temporal, como:

- CHEGA
- ESPERA
- DESISTE
- AVISA
- ENTRA
- SAI
- VIAJA
- MISSAO
- MORRE
- FIM

Esses eventos são armazenados em uma **Lista de Eventos Futuros (LEF)**, implementada como uma fila de prioridades.
