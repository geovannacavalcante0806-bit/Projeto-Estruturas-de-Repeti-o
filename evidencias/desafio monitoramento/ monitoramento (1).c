/*
 * Sistema Inteligente de Monitoramento Industrial
 * Disciplina: Programação em C
 * Professora: Karla Sartin
 *
 * O programa le a temperatura de um sensor repetidamente,
 * valida as entradas, calcula estatisticas (media, maior,
 * menor, quantidade acima do limite) e encerra automaticamente
 * quando detecta 3 leituras CONSECUTIVAS acima do limite de
 * seguranca definido pelo usuario.
 *
 * Estruturas de repeticao utilizadas:
 *  - while    -> usada na validacao de entradas, pois a
 *                condicao (entrada invalida) precisa ser
 *                testada ANTES de repetir a correcao. Se a
 *                entrada digitada ja for valida na primeira
 *                tentativa, o bloco de correcao nunca executa.
 *  - do...while -> usada no laco principal de leitura do
 *                sensor, pois o programa PRECISA ler pelo
 *                menos uma temperatura antes de poder testar
 *                a condicao de parada (3 leituras consecutivas
 *                acima do limite). A condicao so pode ser
 *                avaliada DEPOIS que uma leitura acontece.
 */

#include <stdio.h>

/* Limpa o restante do buffer de entrada apos uma leitura invalida
 * (por exemplo, quando o usuario digita letras em vez de numeros). */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta os caracteres invalidos que sobraram no buffer */
    }
}

/* Le um valor float validado a partir do teclado.
 * Repete a solicitacao enquanto a entrada for invalida.
 *
 * Aqui usamos WHILE: a condicao "leitura invalida" e testada
 * antes de executar o bloco de correcao. Se o usuario acertar
 * de primeira, o bloco de correcao (mensagem de erro + nova
 * leitura) nunca chega a ser executado. */
float lerFloatValido(const char *mensagem) {
    float valor;
    int leituraOk;

    printf("%s", mensagem);
    leituraOk = scanf("%f", &valor);

    while (leituraOk != 1) {
        limparBufferEntrada();
        printf("Entrada invalida! Digite apenas numeros. Tente novamente: ");
        leituraOk = scanf("%f", &valor);
    }

    return valor;
}

int main(void) {
    float limite;
    float temperatura;
    float soma = 0.0f;
    float maior = 0.0f;
    float menor = 0.0f;
    int totalLeituras = 0;
    int qtdAcimaDoLimite = 0;
    int consecutivasAcimaDoLimite = 0;
    int primeiraLeitura = 1;      /* controla a inicializacao de maior/menor */
    int encerradoPorAlerta = 0;   /* 1 = encerrou por 3 leituras consecutivas */

    const float VALOR_ENCERRAR = -9999.0f; /* sentinela para encerrar manualmente */

    printf("SISTEMA INTELIGENTE DE MONITORAMENTO DE TEMPERATURA\n\n");

    /* 1) Validacao do limite de temperatura (usa WHILE internamente,
     *    dentro de lerFloatValido, para tratar entradas invalidas) */
    limite = lerFloatValido("Digite o limite de temperatura de seguranca (C): ");
    printf("\nLimite definido: %.1f C\n", limite);
    printf("Digite as temperaturas lidas pelo sensor.\n");
    printf("Para encerrar manualmente a qualquer momento, digite %.0f\n\n", VALOR_ENCERRAR);

    /* 2) Laco principal de leitura do sensor (DO...WHILE)
     *
     * Usamos do...while porque o programa PRECISA executar pelo
     * menos uma leitura antes de poder verificar a condicao de
     * parada (3 leituras consecutivas acima do limite). Nao faz
     * sentido testar essa condicao antes de existir ao menos uma
     * leitura -- por isso o teste fica APOS o bloco de comandos. */
    do {
        char prompt[100];
        sprintf(prompt, "Temperatura #%d (C): ", totalLeituras + 1);
        temperatura = lerFloatValido(prompt);

        /* Encerramento manual pelo usuario */
        if (temperatura == VALOR_ENCERRAR) {
            printf("\nMonitoramento encerrado manualmente pelo usuario.\n");
            break;
        }

        totalLeituras++;
        soma += temperatura;

        if (primeiraLeitura) {
            maior = temperatura;
            menor = temperatura;
            primeiraLeitura = 0;
        } else {
            if (temperatura > maior) {
                maior = temperatura;
            }
            if (temperatura < menor) {
                menor = temperatura;
            }
        }

        if (temperatura > limite) {
            qtdAcimaDoLimite++;
            consecutivasAcimaDoLimite++;
            printf("  -> ALERTA: temperatura acima do limite! "
                   "(%d consecutiva(s))\n", consecutivasAcimaDoLimite);
        } else {
            /* A condicao deixou de se repetir: zera o contador
             * de consecutivas, pois a sequencia foi interrompida. */
            consecutivasAcimaDoLimite = 0;
        }

        if (consecutivasAcimaDoLimite == 3) {
            encerradoPorAlerta = 1;
            printf("\n*** ALERTA CRITICO: 3 temperaturas consecutivas "
                   "acima do limite! Encerrando monitoramento. ***\n");
        }

    } while (!encerradoPorAlerta);

    /* 3) Relatorio final */
    printf("\nRELATORIO FINAL\n\n");

    if (totalLeituras == 0) {
        printf("Nenhuma temperatura foi registrada.\n");
    } else {
        float media = soma / totalLeituras;
        float percentualAcima = (qtdAcimaDoLimite * 100.0f) / totalLeituras;

        printf("Total de leituras realizadas : %d\n", totalLeituras);
        printf("Limite de seguranca definido  : %.1f C\n", limite);
        printf("Temperatura media              : %.2f C\n", media);
        printf("Maior temperatura registrada   : %.1f C\n", maior);
        printf("Menor temperatura registrada   : %.1f C\n", menor);
        printf("Leituras acima do limite       : %d\n", qtdAcimaDoLimite);
        printf("Percentual acima do limite     : %.1f%%\n", percentualAcima);

        if (encerradoPorAlerta) {
            printf("Motivo do encerramento         : 3 leituras consecutivas "
                   "acima do limite\n");
        } else {
            printf("Motivo do encerramento         : encerrado manualmente "
                   "pelo usuario\n");
        }
    }

    return 0;
}
