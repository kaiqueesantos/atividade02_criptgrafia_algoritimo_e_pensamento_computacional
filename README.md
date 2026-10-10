# Criptografia Simples com Progressões e Séries

## O que é o projeto

Programa em C que criptografa uma palavra usando duas camadas:

1. **Cifra de César:** cada letra é deslocada por um SHIFT fixo.
2. **Sequência numérica:** cada letra é deslocada de novo pelo termo da sequência na mesma posição.

O deslocamento total de cada letra é `SHIFT + sequência[i]`, e o alfabeto dá a volta depois do Z (a = 0, b = 1, ..., z = 25).

## Sequências que dá para escolher

| Tipo | Sequência |
|------|-----------|
| 1 | PA (o usuário informa o primeiro termo e a razão) |
| 2 | PG (o usuário informa o primeiro termo e a razão) |
| 3 | Fibonacci (1, 1, 2, 3, 5, 8, ...) |
| 4 | Números primos (2, 3, 5, 7, ...) |

## Como rodar

```
gcc -o criptografia criptografia.c
./criptografia
```

## Menu

- **1 - Criptografar palavra:** pede a palavra (até 15 letras, sem acento), o SHIFT e a sequência.
- **2 - Comparar sequências:** criptografa a mesma palavra com as 4 sequências para dar para comparar (nessa opção a PA usa 1 e 1, e a PG usa 1 e 2).
- **0 - Sair**

## Arquivos que o programa gera

- `resultado_criptografia.txt`: a última palavra codificada.
- `log_execucao.txt`: histórico das execuções, com a data, os dados digitados, a sequência e o resultado de cada camada letra por letra.

## Exemplo

Palavra `coracao`, SHIFT 3, Fibonacci (tipo 3):

| Letra | c | o | r | a | c | a | o |
|-------|---|---|---|---|---|---|---|
| Sequência | 1 | 1 | 2 | 3 | 5 | 8 | 13 |
| Camada 1 (+3) | f | r | u | d | f | d | r |
| Camada 2 (+sequência) | g | s | w | g | k | l | e |

Palavra criptografada: `gswgkle`

Arquivo `resultado_criptografia.txt`:

```
Palavra codificada: gswgkle | SHIFT: 3 | Tipo: 3 | Letras: 7
```

**Observação:** o enunciado da atividade mostra `fqvdjqb` como resultado desse exemplo, mas esse valor não bate com a regra descrita (SHIFT + sequência[i]). Seguindo a regra, o resultado é `gswgkle`.

## Taxonomia de Bloom

- **Lembrar:** alfabeto, valores das letras e as sequências.
- **Compreender:** como o SHIFT e a sequência mudam cada letra.
- **Aplicar:** implementação das duas camadas em C.
- **Analisar:** opção 2 do menu, que compara as sequências.
- **Avaliar:** conclusão do grupo abaixo.
- **Criar:** programa completo, em que o usuário escolhe a palavra, o SHIFT e a sequência.

## Conclusão do grupo sobre a segurança

Para comparar, usamos a opção 2 do programa com a palavra `abc` e SHIFT 3. Os resultados foram:

| Sequência | Resultado |
|-----------|-----------|
| PA (1 e 1) | egi |
| PG (1 e 2) | egj |
| Fibonacci | efh |
| Primos | fhk |

Pelo que observamos, a **PA é a mais fraca**. Como ela soma sempre o mesmo valor, as letras codificadas seguem um padrão regular (em `egi` as letras andam de 2 em 2). Dependendo da razão ela ainda se repete rápido: com razão 13, por exemplo, o deslocamento só alterna entre dois valores, já que o alfabeto tem 26 letras.

A **PG** cresce rápido, mas como o alfabeto dá a volta no 26, os deslocamentos também acabam se repetindo depois de alguns termos.

Na nossa opinião, a **Fibonacci** é a mais forte entre as quatro, porque cada termo depende dos dois anteriores e o resultado não segue um padrão fácil de perceber. Os **primos** também ficaram bons, já que não existe fórmula para eles, mas são mais fáceis de adivinhar por serem uma sequência bem conhecida.

Mesmo assim, concluímos que essa criptografia não é segura de verdade. Quem souber que ela usa Cifra de César mais uma sequência só precisa testar o SHIFT (são só 26 valores) e as 4 sequências para descobrir a palavra. Ela serve bem para aprender como a matemática pode ser usada na criptografia, mas a segurança de verdade precisa de métodos bem mais complexos.
