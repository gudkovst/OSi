#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILOSOPHERS 5
#define DELAY 1000
#define FOOD 50

#define FIRST_FORK(i) (i != PHILOSOPHERS - 1)? i : 0
#define SECOND_FORK(i) (i != PHILOSOPHERS - 1)? i + 1 : i

pthread_mutex_t forks[PHILOSOPHERS];
pthread_t phils[PHILOSOPHERS];
pthread_mutex_t foodlock;
int resEating[PHILOSOPHERS] = {0};

int food_on_table(){
  static int food = FOOD;
  int myfood;

  pthread_mutex_lock(&foodlock);
  myfood = food;
  if (food > 0){
    food--;
  }
  pthread_mutex_unlock(&foodlock);
  return myfood;
}

void get_fork(int phil, int fork){
  pthread_mutex_lock(&forks[fork]);
  printf("Philosopher %d: got fork %d\n", phil, fork);
}

void down_forks(int f1, int f2){
  pthread_mutex_unlock(&forks[f1]);
  pthread_mutex_unlock(&forks[f2]);
}

void* philosopher(void* num){
  int id = (int)num;
  int firstFork = FIRST_FORK(id);
  int secondFork = SECOND_FORK(id)
  int f;

  while(f = food_on_table()){
    usleep(DELAY);
    printf("Philosopher %d: get dish %d.\n", id, f);
    get_fork(id, firstFork);
    get_fork(id, secondFork);

    printf("Philosopher %d: eating.\n", id);
    resEating[id]++;
    usleep(DELAY * (FOOD - f + 1));
    down_forks(secondFork, firstFork);
  }
  return NULL;
}

int main(){
    int st;
  
    if (st = pthread_mutex_init(&foodlock, NULL)){
      fprintf(stderr, "Not init food mutex: %d\n", st);
      return st;
    }
    for (int i = 0; i < PHILOSOPHERS; i++)
      if (st = pthread_mutex_init(&forks[i], NULL)){
        fprintf(stderr, "Not init fork mutex: %d\n", st);
        return st;
      }
    for (int i = 0; i < PHILOSOPHERS; i++)
      if (st = pthread_create(&phils[i], NULL, philosopher, (void*)i)){
        fprintf(stderr, "Not create philosopher: %d\n", st);
        return st;
      }
    for (int i = 0; i < PHILOSOPHERS; i++)
      if (st = pthread_join(phils[i], NULL)){
        fprintf(stderr, "Not join philosopher: %d\n", st);
        return st;
      }
    for (int i = 0; i < PHILOSOPHERS; i++)
      if (st = pthread_mutex_destroy(&forks[i])){
        fprintf(stderr, "Not destroy fork mutex: %d\n", st);
        return st;
      }
    if (st = pthread_mutex_destroy(&foodlock)){
      fprintf(stderr, "Not destroy food mutex: %d\n", st);
      return st;
    }
    return 0;
}
