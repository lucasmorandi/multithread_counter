#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define N_PROCESSOS 4      //define o numero maximo de processos paralelos

int main() {

  int n=0;
  int flagPrimo, div;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *num;
  num = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);

  int *cnt;
  cnt = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  int *processos_ativos;
  processos_ativos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  
  //Recebe os numeros pelo terminal
  while(scanf("%d", &num[n]) != EOF){
    n++;
  }

  pid_t filho[n];               
  *processos_ativos = 0;    
 
 for ( int i=0; i<n; i++) {

    if(*processos_ativos <= N_PROCESSOS){
      filho[i] = fork();

      if (filho[i] == 0) {     
        /* Esta parte do codigo executa no processo filho */
        flagPrimo=1;

        //percorre o vetor de numeros procurando por primos
        for(div=2; div < num[i]; div++){                
          if(num[i]%div == 0){
            //nao eh primo
            flagPrimo=0;
            break;
          }
        }

        if(flagPrimo == 1 && num[i]!=1 && num[i]!=0){   // eh primo
          *cnt = *cnt + 1;
        }
        i++; 
        *processos_ativos--;
        exit(0);
      }
    *processos_ativos++;
    }

  } 
  int pid;
  for (int i=0; i<n; i++) {
   pid = waitpid(filho[i], NULL, 0);
  }

  printf("%d\n", *cnt);
  munmap(NULL, sizeof(int)*100);
  munmap(NULL, sizeof(int));
}