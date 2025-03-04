# UEL_Compilers

Este repositório contém todos os projetos desenvolvidos nas disciplinas de **Compiladores I** e **Compiladores II** do curso de **Ciência da Computação** na Universidade Estadual de Londrina (UEL). O foco principal é a implementação das etapas de um compilador, desde a análise léxica até a geração de código.

## Etapas de um Compilador 

O desenvolvimento dos projetos 1, 2 e 3 seguem a sequência clássica de um compilador:

**Análise Léxica** → **Análise Sintática** → **Análise Semântica** → **Geração de Código**

### Projetos
---
#### 1. Analisador Léxico
Projeto desenvolvido durante o quinto semestre da graduação. Implementa a análise léxica utilizando a ferramenta **Flex** para identificar tokens da linguagem alvo.
---
#### 2. Analisador Sintático
Expande o analisador léxico com uma análise sintática utilizando **Flex** e **Bison**, permitindo a detecção de erros gramaticais na entrada.
---
#### 3. Gerador de Código
Módulo final do compilador desenvolvido durante o sexto semestre da graduação, que transforma a representação intermediária em código executável ou em outra linguagem intermediária.
---
#### 4. Portugol em C
Projeto independente que implementa um interpretador para uma versão simplificada do **Portugol**, utilizando **C** para a análise léxica e sintática.
---
#### 5. DCMAT
DCMAT é uma calculadora interativa que permite a visualização e o cálculo de funções matemáticas, incluindo gráficos, integrais e somatórios. Com suporte a configurações de precisão e controle sobre os eixos e visualização, ela oferece uma interface de texto simples para gráficos 2D, onde os usuários podem plotar funções, ajustar parâmetros e calcular resultados como integrais e somatórios. Além disso, o programa utiliza métodos como a soma de Riemann para integrar funções.
---
#### 6. RegAlloc
Projeto que aborda a alocação de registradores em um compilador. Utiliza uma técnica mais complexa, como a análise de interferência e a seleção de registradores. O RegAlloc é uma parte importante do compilador, garantindo a utilização eficiente dos registradores da máquina de destino.
---
#### 7. Linear Scan
Este projeto implementa um algoritmo de alocação de registradores baseado em linear scan, otimizado para trabalhar com grandes volumes de dados.
---

## Tecnologias Utilizadas
- **C**
- **Flex** (para análise léxica)
- **Bison** (para análise sintática)
- **Algoritmos de alocação de registradores**
