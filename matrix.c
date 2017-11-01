#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define N 10000

/*
зачем нужна переменная thread_count?
*/

/*
добавьте комментарий в код с результатов замеров времени исполнения
в зависимости от числа нитей
*/
size_t thread_count = 0;

typedef struct {
	float* AMatrix;
	float* BMatrix;
	float* CMatrix;
	size_t* str_nums;
	size_t thread_num;
} ThreadArg;

void* MulMatrixThread(void* args) {
	/*
	один раз бы написали
	ThreadArg* threadArg = (ThreadArg*)args;
	чтобы дальше в каждом месте не писать приведение к типу
	*/
	int str_num = ((ThreadArg*)args)->str_nums[thread_count];
	thread_count++;
	int thread_num = ((ThreadArg*)args)->thread_num;
	size_t str_end = str_num + thread_num;
        if (thread_count == thread_num) str_end = N;
	for (size_t i = str_num; i < str_end; i++) {
		for (size_t j = 0; j < N; j++) {
			for (size_t k = 0; k < N; k++) {
				((ThreadArg*)args)->CMatrix[i*N+j] +=
					((ThreadArg*)args)->AMatrix[i*N+k] * ((ThreadArg*)args)->BMatrix[k*N+j];
			}
		}
	}
	return NULL;
}

int main (int argv, char** argc) {
	ThreadArg args = {};
	size_t thread_num = atoi(argc[1]);
	args.thread_num = thread_num;
	args.str_nums = (size_t*)calloc(N, sizeof(size_t));
	/*
	fixit: вы неявно предполагаете, что N делится на thread_num
	*/
	for (size_t i = 0; i < thread_num; i++) 
		args.str_nums[i] = (int)(N/thread_num) * i;
	pthread_t* thids = (pthread_t*)calloc(N, sizeof(pthread_t));
	args.AMatrix = (float*)calloc(N * N, sizeof(float));
	args.BMatrix = (float*)calloc(N * N, sizeof(float));
	args.CMatrix = (float*)calloc(N * N, sizeof(float));
	/*
	лучше бы случайными числами матрицы заполнили
	*/
	for (size_t i = 0; i < N; i++) {
		for(size_t j = 0; j < N; j++) {
			args.BMatrix[N * i + j] = 0;
			args.AMatrix[N * i + j] = (float)rand();
			args.CMatrix[N * i + j] = 0;
			if (i == j) args.BMatrix[N * i + j] = 1;
		}
	}
	int str_num = 0;
	for (size_t i = 0; i < thread_num; i++) {
		pthread_create(&thids[i], NULL, MulMatrixThread, (void*)&args);
	}
	for (size_t i = 0; i < N/thread_num; i++) 
		pthread_join(thids[i], NULL); 
	return 0;
}
