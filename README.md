# UEL_Compilers

Este repositório contém todos os projetos desenvolvidos nas disciplinas de **Compiladores I** e **Compiladores II** do curso de **Ciência da Computação** na Universidade Estadual de Londrina (UEL). O foco principal é a implementação das etapas de um compilador, desde a análise léxica até a geração de código.

## Estrutura do Repositório

O desenvolvimento segue a sequência clássica de um compilador:

1. **Análise Léxica** → **Análise Sintática** → **Análise Semântica** → **Geração de Código**

### Projetos

#### 1. Analisador Léxico
Projeto desenvolvido durante o quinto semestre da graduação. Implementa a análise léxica utilizando a ferramenta **Flex** para identificar tokens da linguagem alvo.

#### 2. Analisador Sintático
Expande o analisador léxico com uma análise sintática utilizando **Flex** e **Bison**, permitindo a detecção de erros gramaticais na entrada.

#### 3. Analisador Semântico
Complementa o analisador sintático com regras semânticas, garantindo a coerência dos programas analisados. Desenvolvido com **Flex** e **Bison**.

#### 4. Gerador de Código
Módulo final do compilador, que transforma a representação intermediária em código executável ou em outra linguagem intermediária.

#### 5. Portugol em C
Projeto independente que implementa um interpretador para uma versão simplificada do **Portugol**, utilizando **C** para a análise léxica e sintática.

## Tecnologias Utilizadas
- **C**
- **Flex** (para análise léxica)
- **Bison** (para análise sintática)

