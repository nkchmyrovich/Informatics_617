#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define N 10000

/*
Так надо же было пробовать число нитей 1, 2, 3, 4 ... ну или 5, 6, 7, 8, если
есть hyperthreding и число ядер 4.
Для 50 ... 900 непонятно что ожидать.
*/

/*
thread_num = 50 

real	0m7.022s
user	0m27.232s
sys	0m0.000s

thread_num = 100

real	0m7.103s
user	0m27.320s
sys	0m0.012s

thread_num = 200

real	0m6.871s
user	0m26.244s
sys	0m0.032s

thread_num = 300

real	0m7.405s
user	0m24.768s
sys	0m0.016s

thread_num = 400

real	0m8.108s
user	0m23.384s
sys	0m0.024s


thread_num = 500

real	0m6.991s
user	0m26.416s
sys	0m0.044s


thread_num = 600

real	0m10.230s
user	0m19.912s
sys	0m0.056s

thread_num = 700

real	0m8.803s
user	0m21.404s
sys	0m0.044s

thread_num = 800

real	0m8.013s
user	0m22.496s
sys	0m0.052s

thread_num = 900

real	0m8.013s
user	0m22.496s
sys	0m0.052s      
*/

typedef struct {
        float* AMatrix;
        float* BMatrix;
        float* CMatrix;
        int str_start;
	int str_end;
} ThreadArg;

void* MulMatrixThread(void* args) {
	ThreadArg* threadArg = (ThreadArg*)args;
	int str_start = threadArg->str_start;
        int str_end = threadArg->str_end;
	printf("str_start: %d\n", str_start);
	printf("str_end: %d\n", str_end);
        for (size_t i = str_start; i < str_end; i++) {
                for (size_t j = 0; j < N; j++) {
                        for (size_t k = 0; k < N; k++) {
                                threadArg->CMatrix[i*N+j] +=
                                        threadArg->AMatrix[i*N+k] * threadArg->BMatrix[k*N+j];
                        }
                }
        }
	printf("end\n");
}

int main (int argv, char** argc) {
        size_t thread_num = atoi(argc[1]);
	ThreadArg* args = (ThreadArg*)calloc(thread_num, sizeof(ThreadArg));
	div_t thread_num_div = div(N, thread_num);
	int first_str_num = thread_num_div.quot + thread_num_div.rem;
	args[0].str_start = 0;
	args[0].str_end =  first_str_num;
        for (size_t i = 0; i < thread_num-1; i++) {
		args[i+1].str_start = first_str_num + i * thread_num_div.quot;
		args[i+1].str_end = first_str_num + (i + 1) * thread_num_div.quot;
	}
	printf("last str_end: %d\n", args[thread_num-1].str_end);
	pthread_t* thids = (pthread_t*)calloc(N, sizeof(pthread_t));
        float* AMatrix = (float*)calloc(N * N, sizeof(float));
        float* BMatrix = (float*)calloc(N * N, sizeof(float));
        float* CMatrix = (float*)calloc(N * N, sizeof(float)); 
        for (size_t i = 0; i < N; i++) {
                for(size_t j = 0; j < N; j++) {
                        BMatrix[N*i+j] = (float)rand();
                        AMatrix[N*i+j] = (float)rand();
                        CMatrix[N*i+j] = 0;
                }
        }
	for (size_t i = 0; i < thread_num; i++) {
                args[i].AMatrix = AMatrix;
                args[i].BMatrix = BMatrix;
                args[i].CMatrix = CMatrix;
        }
        for (size_t i = 0; i < thread_num; i++) 
                pthread_create(&thids[i], NULL, MulMatrixThread, (void*)&args[i]);
        printf("hello");
	for (size_t i = 0; i < thread_num; i++)
                pthread_join(thids[i], NULL);
	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			printf("%g ", CMatrix[N*i+j]);
		}
		printf("\n");
	}
        return 0;
}
