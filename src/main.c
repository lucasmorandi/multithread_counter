/* Contador de primos multithread
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define N_THREADS 4     //define o numero maximo de threads paralelas
#define MAX_NUMS 100 	//define o numero maximo de numeros na entrada

pthread_mutex_t trava, trava2;

int threads_ativas=0, cnt=0;
unsigned long int num[MAX_NUMS];


//Funçao que verifica se o numero recebido por arg eh primo
void* verifica_primo(void* arg){
	pthread_mutex_lock(&trava);
	threads_ativas++;
	int *num = (int*)arg;
	pthread_mutex_unlock(&trava);

	int flagPrimo=1, divisor;

	//percorre o vetor de numeros procurando por primos
    for(divisor=2; divisor < 1+(*num/2); divisor++){                
      if(*num%divisor == 0){
        //nao eh primo
        flagPrimo=0;
        break;
      }
    }

    if(flagPrimo == 1 && *num!=1 && *num!=0){   // eh primo
    	pthread_mutex_lock(&trava2);
    	cnt = cnt + 1;    	
    	pthread_mutex_unlock(&trava2);
    }

    pthread_mutex_lock(&trava);
	threads_ativas--;
	pthread_mutex_unlock(&trava);
    //free(num);
	return NULL;
}

int main() {

  int n=0;

  //Recebe os numeros pelo terminal
  while(scanf("%lu", &num[n]) != EOF){			//armazena os numeros em num[]
    n++;
  }
  pthread_t threads[n];     				  //cria vetor de n threads             
 

 for ( int i=0; i<n;) {

   	if(threads_ativas < N_THREADS){
      //cria nova thread
    	int *narg = malloc(sizeof(unsigned long int));
    	*narg = num[i];  		

       	pthread_create(&(threads[i]), NULL, verifica_primo, (void*)narg);
       	i++;
  	}
  }

  //garantir que todas as threads terminaram
  for (int i = 0; i < n; i++) {
  	pthread_join(threads[i], NULL);
  }

  printf("%d\n", cnt);
}
