Pré-requisitos:

- Ter o compilador GCC (GNU Compiler Collection) instalado no sistema.

Como Compilar:

- Para compilar o código, utilize o comando make all na pasta onde estão os programas ex1.c, ex2.c, ex3.c e ex4.c.


Como Executar
    Executando o ./ex1:
        - O programa solicitará a entrada do número de níveis (N) para gerar a árvore de processos. Insira o número desejado de níveis e pressione Enter.
        Exemplo:
        - Entrada do programa: Digite o número de níveis (N): 3
        - Saída do programa: Número total de processos a serem criados: 3
                            Processo criado: P.1, Nível: 1, PID: 19766
                            Processo criado: P.1.1, Nível: 2, PID: 19767
                            Processo criado: P.1.2, Nível: 2, PID: 19768
        - O programa continuará em execução após a saída para que seja possível abrir um terminal
        - e utilizar o comando pstree para visualizar a árvore de processos.

    Executando o ./ex2:
        - O programa solicitará o comando a ser executado juntamente com seus parâmetros como, por exemplo, ping, ls, etc.
        Exemplo:
        - Entrada do programa: Digite seu comando: ls
        - Saída do programa: Executando o comando: ls 
                            ex1  ex1.c  ex2  ex2.c  ex3  ex3.c  ex4  ex4.c  makefile  readme.txt
                            Processo executa_comando finalizado.

    Executando o ./ex3:
        - O programa gera um vetor de números inteiros aleatórios e realiza uma busca paralela utilizando múltiplos processos filhos criados com fork().
        Exemplo:
            - Vetor aleatório gerado: 1 2 2 9 4 2 8 5 9 4
            - [Filho PID 8318] O valor [4] foi encontrado na posição [4]
            - [Filho PID 8319] O valor [4] foi encontrado na posição [9]
            - [Pai] Busca concluída com sucesso.

        Caso o valor não seja encontrado por nenhum processo, o processo pai exibirá:
            - Vetor aleatório gerado: 5 0 3 1 6 6 2 2 2 3
            - [Pai] O valor [4] não foi encontrado por nenhum processo filho.

        - O valor buscado, o tamanho do vetor e o número de processos filhos são definidos por #define dentro do código (VALOR_PROCURADO, TAMANHO_VETOR e NUM_PROCESSOS_FILHOS)
    
    
    Executando o ./ex4:
       - O programa emula um shell simples, dando a opção ao usuário de realizar os comandos básicos de um terminal do linux e também executar esses comandos em segundo plano.
       Exemplo:
          -  > ls
          -  ex04.c	ex04.out  makefile
          -  > mkdir teste
          -  > ls
          -  ex04.c	ex04.out  makefile  teste
          -  > ls &
          -  Executando em segundo plano: PID: [33304]
          -  > ex04.c	ex04.out  makefile  teste

Bibliotecas Padrão:

- stdio.h
- stdlib.h
- unistd.h
- string.h
- sys/wait.h
- time.h

Bibliotecas Não Padrões:

Nenhuma biblioteca não padrão é usada neste código. Todas as bibliotecas são parte da biblioteca padrão do C.


