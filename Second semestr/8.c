#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <math.h>

#define ITERS 100000

typedef struct Func{
	int startPos;
	int step;
	double res;
}func;

void* calcPi(void* args){
	func* arg = (func*)args;
	int step = arg->step;
	double res = 0;
	for (int i = arg->startPos; i < ITERS; i += step){
		res += 1.0 / (i * 4.0 + 1.0);
        res -= 1.0 / (i * 4.0 + 3.0);
	}
	arg->res = res;
	pthread_exit((void*)arg);
}

int main(int argc, char** argv){
	if (argc < 2){
		fprintf(stderr, "Too few args\n");
		return 1;
	}
	const int numThrs = atoi(argv[1]);
	pthread_t thrs[numThrs];
	int st;
	func* args[numThrs];
	double pi = 0;
	for (int i = 0; i < numThrs; i++){
		args[i] = (func*)malloc(sizeof(func));
		args[i]->step = numThrs;
		args[i]->startPos = i;
		if (st = pthread_create(&thrs[i], NULL, calcPi, (void*)args[i])){
			fprintf(stderr, "Not create thread %d: %d\n", i, st);
			return st;
		}
	}
	for (int i = 0; i < numThrs; i++){
		if (st = pthread_join(thrs[i], (void*)args[i])){
			fprintf(stderr, "Not join thread %d: %d\n", i, st);
			return st;
		}
		pi += args[i]->res;
		free(args[i]);
	}
	pi *= 4;
	printf("pi = %lf\nerror = %lf\n", pi, fabs(M_PI - pi));
	return 0;
}