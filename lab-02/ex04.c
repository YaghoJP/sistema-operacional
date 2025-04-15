#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_TAMANHO_ENTRADA 100
#define MAX_ARGUMENTOS 100

/*
 * Descrição:
 * Faça uma interface de shell simples que fornece um prompt ao usuário para executar comandos do shell
 * do sistema. Se o comando for executado em segundo plano (&), a interface deve possibilitar a execução
 * de outros comandos. Caso contrário, a interface deve esperar o retorno do comando e, em seguida,
 * exibir o prompt novamente.
 * 
 * Autores: Daniel Victor Silva Bonfim, Elder Henrique Alves Correia, Winicius Abilio de Britto e
 * Yagho Junior Petini
 * Data de criação: 13/04/2025
 * Última atualização: 15/04/2025
 * 
 * Parâmetros de entrada:
 * - Parâmetros de entrada vai ser o próprio comando no shell, como por exemplo o comando "ls" ou "mkdir pasta_teste"
 * 
 * Parâmetros de saída:
 * - exibe o resultado do comando que foi passado para executar.
 * 
 * Função principal:
 * - Espera a entrada do usuário e após isso, verifica se a entrada é pra ser executada em segundo plano ou não.
 * - Após isso, é criado um novo processo que será sebustituido pelo binário do respectivo comando passado pelo usuário.
 * - Caso o usuário insira um comando e em seguida o caracter '&', o programa irá ser executado em segundo plano e após o seu termino fornecer a saída.
 */


void extraiParametros(char *entrada, char *params[]){

    int i = 0;
    char *token = strtok(entrada, " \t\n");
    
    while (token != NULL && i < MAX_ARGUMENTOS - 1) {
        params[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    params[i] = NULL;
}

void shell(){

  char entrada[MAX_TAMANHO_ENTRADA];
  char *parametros[MAX_ARGUMENTOS];
  bool saida=false;
  bool segundoPlano=false;

  while(!saida){
    printf("> ");
    fflush(stdout);
    fgets(entrada, MAX_TAMANHO_ENTRADA, stdin);

    //Retirando o \n

    int len = strlen(entrada);
    if (len > 0 && entrada[len-1] == '\n') {
      entrada[len-1] = '\0';
      len--;
    }

    if (strcmp(entrada, "exit") == 0) {
      saida = true; 
      continue;
    }

   // Verifica se é para segundo plano (deve ser o último caractere)

   if (len > 0 && entrada[len-1] == '&') {
     segundoPlano = true;
     entrada[len-1] = '\0'; // Remove o &
     // Remove espaço antes do & se existir
     if (len > 1 && entrada[len-2] == ' ') {
       entrada[len-2] = '\0';
     }
   }
   extraiParametros(entrada,parametros);
    
   //fork
   pid_t pid = fork();

   //Se for o processo "Filho"
   if(pid==0){
     execvp(parametros[0], parametros);
     exit(0);
   }
   //se o usuário informou para executar o programa em segundo plano o programa irá mostrar uma mensagem na tela indicando isso.
   //Caso não seja, ele vai continuar a execução normal do shell.
   if(!segundoPlano) {
       waitpid(pid, NULL, 0);
    }
    else {
       printf("Executando em segundo plano: PID: [%d]\n", pid);
    }
  }  
}

int main(){
  shell();
  return(0);
}

