# Sistema de Monitoramento de Temperatura

## 1. Identificação

- **Nome do aluno:** Geovanna Vitória
- **Disciplina:** Algoritmos e Pensamento Computacional
- **Professora:** Profa. Karla Sartin
- **Título do projeto:** Sistema de Monitoramento de Temperatura

## 2. Objetivo

O programa simula um sistema de monitoramento de temperatura de uma máquina
industrial. Ele lê continuamente as temperaturas informadas por um sensor,
valida as entradas do usuário, calcula estatísticas (média, maior, menor e
percentual de leituras acima do limite) e **encerra automaticamente** o
monitoramento caso identifique **3 temperaturas consecutivas** acima do
limite de segurança definido, sinalizando uma possível situação de risco.

## 3. Funcionamento do programa

- **Definição do limite:** o próprio usuário informa, no início da execução,
  o limite de temperatura de segurança (em °C).
- **Leituras do sensor:** a cada iteração, o programa solicita uma nova
  temperatura, simulando a leitura feita por um sensor.
- **Tratamento de entradas inválidas:** se o usuário digitar algo que não
  seja um número (letras, símbolos etc.), o `scanf` falha, o buffer de
  entrada é limpo e o programa solicita a entrada novamente, sem travar ou
  encerrar por erro.
- **Identificação de temperaturas acima do limite:** cada leitura é
  comparada com o limite definido. Se for maior, o contador geral de
  leituras acima do limite é incrementado e uma mensagem de alerta é
  exibida.
- **Contagem de consecutivas:** existe um contador específico para
  temperaturas **consecutivas** acima do limite. Ele é incrementado a cada
  leitura acima do limite e **reiniciado (zerado)** assim que aparece uma
  leitura dentro do limite, garantindo que só contem sequências
  ininterruptas.
- **Condição de encerramento automático:** quando o contador de
  consecutivas atinge o valor 3, o programa exibe um alerta crítico e
  encerra o laço de leitura automaticamente.
- **Encerramento manual:** a qualquer momento, o usuário também pode digitar
  o valor sentinela `-9999` para encerrar o monitoramento manualmente.
- **Relatório final:** ao final (seja por encerramento automático ou
  manual), o programa exibe total de leituras, temperatura média, maior e
  menor temperatura, quantidade e percentual de leituras acima do limite, e
  o motivo do encerramento.

## 4. Estruturas de repetição utilizadas

- **`while`** — utilizado dentro da função `lerFloatValido()`, responsável
  pela validação das entradas do usuário (tanto do limite quanto de cada
  temperatura). A condição (`leituraOk != 1`, ou seja, "a leitura foi
  inválida?") é testada **antes** de executar o bloco de correção. Isso
  faz sentido porque, se o usuário já digitar um valor válido de primeira,
  o bloco de correção (mensagem de erro + nova tentativa de leitura) **não
  precisa ser executado nenhuma vez**.

- **`do...while`** — utilizado no laço principal que lê as temperaturas do
  sensor. Aqui a lógica é diferente: o programa **precisa obrigatoriamente
  realizar pelo menos uma leitura** antes de poder verificar a condição de
  parada (3 leituras consecutivas acima do limite). Não seria possível
  testar essa condição antes de existir ao menos uma leitura, pois o
  contador de consecutivas só é atualizado depois que uma temperatura é
  processada.

### Justificativa da escolha

A combinação das duas estruturas foi escolhida porque elas resolvem
problemas diferentes dentro do mesmo programa:

- Na validação de entrada, o `while` evita executar código de correção
  desnecessário quando a entrada já é válida (teste **antes**).
- No laço de leitura do sensor, o `do...while` garante que **pelo menos uma
  leitura sempre aconteça**, já que o próprio propósito do programa é
  monitorar a temperatura — não faria sentido "testar antes de ler" (teste
  **depois**).

## 5. Como executar

Compile o programa com o `gcc`:

```bash
gcc monitoramento.c -o monitoramento
./monitoramento
```

Durante a execução, informe o limite de temperatura e, em seguida, as
temperaturas lidas pelo sensor, uma por vez. Para encerrar manualmente a
qualquer momento, digite `-9999`.

## 6. Testes realizados

Os arquivos de evidência (capturas/textos das execuções) estão na pasta
`evidencias/`.

### Teste 1 — Validação de entradas inválidas (`evidencias/teste01.txt`)

Foram digitados os valores `abc` (para o limite) e `xyz` (para uma
temperatura). Em ambos os casos, o programa identificou a entrada inválida,
exibiu a mensagem de erro e solicitou o valor novamente, sem travar.
**Resultado:** validação funcionando corretamente.

### Teste 2 — Temperaturas acima do limite, porém não consecutivas (`evidencias/teste02.txt`)

Com limite de 80°C, foram informadas as temperaturas 80, 40, 85, 30 e 82.
As leituras 85 e 82 ficaram acima do limite, mas intercaladas por leituras
abaixo do limite (40 e 30), portanto o contador de consecutivas foi zerado
entre elas e o monitoramento **não** foi encerrado automaticamente.
**Resultado:** 2 leituras acima do limite (50%), contador de consecutivas
reiniciado corretamente, encerramento manual.

### Teste 3 — Três temperaturas consecutivas acima do limite (`evidencias/teste03.txt`)

Com limite de 80°C, foram informadas as temperaturas 80, 30, 85, 90 e 95.
As três últimas leituras (85, 90, 95) ficaram consecutivamente acima do
limite, disparando o **encerramento automático** após a terceira leitura
consecutiva.
**Resultado:** alerta crítico exibido, monitoramento encerrado
automaticamente, relatório final indicando corretamente o motivo do
encerramento.

---

## Questão final de reflexão

**Por que você escolheu `while`, `do...while` ou uma combinação das duas
estruturas? Em qual parte do algoritmo a diferença entre testar a condição
antes ou depois da execução foi importante para sua solução?**

Optei por usar uma **combinação das duas estruturas**, pois cada uma
resolve um problema diferente dentro do programa.

Usei o `while` na validação das entradas (limite e temperaturas), porque
nesse caso a condição precisa ser testada **antes** de decidir se o bloco
de correção será executado. Se o usuário digitar um número válido logo na
primeira tentativa, não há necessidade de exibir nenhuma mensagem de erro
nem de pedir a entrada novamente — o `while` permite exatamente isso, pulando
o bloco de correção quando ele não é necessário.

Já no laço principal, que lê as temperaturas do sensor, usei o
`do...while`, porque a diferença entre testar antes ou depois foi
determinante: o programa só consegue saber se deve parar (3 leituras
consecutivas acima do limite) **depois** de ter processado pelo menos uma
leitura. Se eu tivesse usado um `while` tradicional nesse ponto, seria
necessário ler uma temperatura só para poder testar a condição pela
primeira vez, o que tornaria a lógica confusa e redundante. Com o
`do...while`, o fluxo fica natural: o sensor sempre faz pelo menos uma
leitura, processa o resultado, atualiza o contador de consecutivas e só
então verifica se deve continuar ou encerrar — exatamente como acontece na
situação-problema do menu apresentada em aula, em que o bloco precisa
executar pelo menos uma vez antes de a condição fazer sentido.
