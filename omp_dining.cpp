#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>


#define PHIL 5
#define FORKS 5

int eat_check = 0;

omp_lock_t forks[FORKS];

void think(int i) {
  int secs = 1 + (rand() % PHIL);
  printf("Philosopher %d thinks\n", i);
  sleep(secs);
}

void eat(int i) {
  int fork_left;
  int fork_right;

  if (i == PHIL - 1) {
    fork_left = 0;
    fork_right = i;
  } else {
    fork_left = i;
    fork_right = i + 1;
  }

  eat_check++;

  printf("Philosopher %d asks for a fork (%d) \n", i, fork_left);
  omp_set_lock(&forks[fork_left]);

  printf("Philosopher %d asks for a fork (%d) \n", i, fork_right);
  omp_set_lock(&forks[fork_right]);

  int secs = rand() % PHIL;
  printf("Philosopher %d eats %d seconds \n", i, secs);

  omp_unset_lock(&forks[fork_left]);
  omp_unset_lock(&forks[fork_right]);
}

void dining(int i) {
  while (eat_check<=10) {
      think(i);
      eat(i);
  }
}

int main() {

  srand(time(NULL));
  omp_set_num_threads(PHIL);

  for (int i = 0; i < PHIL; i++) {
    omp_init_lock(&forks[i]);
  }

  #pragma omp parallel for private(i)
  for (int i = 1; i <= PHIL; i++) {
    dining(i);
  }

  return 0;
}
