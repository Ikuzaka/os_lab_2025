#define _POSIX_C_SOURCE 200809L

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "sum.h"
#include "utils.h"
#include <pthread.h>
#include <time.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
  long long result;
};

void *ThreadSum(void *arg) {
  struct SumArgs *args = arg;
  args->result = sum(args->array, args->begin, args->end);
  return NULL;
}

int main(int argc, char **argv) {
  int threads_num = 0;
  int array_size = 0;
  int seed = -1;
  int opt;
  struct option options[] = {
      {"threads_num", required_argument, NULL, 't'},
      {"seed", required_argument, NULL, 's'},
      {"array_size", required_argument, NULL, 'a'},
      {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "", options, NULL)) != -1) {
    switch (opt) {
      case 't': 
          threads_num = atoi(optarg); 
          break;
      case 's': 
          seed = atoi(optarg); 
          break;
      case 'a': 
          array_size = atoi(optarg); 
          break;
      default: return 1;
    }
  }
  if (threads_num <= 0 || array_size <= 0 || seed < 0 || optind != argc) {
    fprintf(stderr, "Usage: %s --threads_num N --seed N --array_size N\n", argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(*array) * (size_t)array_size);
  if (!array) return 1;
  GenerateArray(array, array_size, seed);
  pthread_t threads[threads_num];
  struct SumArgs args[threads_num];
  for (int i = 0; i < threads_num; ++i) {
    args[i].array = array;
    args[i].begin = (long long)i * array_size / threads_num;
    args[i].end = ((long long)i + 1) * array_size / threads_num;
  }

  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0; i < threads_num; ++i) {
    if (pthread_create(&threads[i], NULL, ThreadSum, &args[i])) return 1;
  }
  long long total_sum = 0;
  for (int i = 0; i < threads_num; ++i) {
    if (pthread_join(threads[i], NULL)) return 1;
    total_sum += args[i].result;
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  double elapsed = (finish.tv_sec - start.tv_sec)
                 + (finish.tv_nsec - start.tv_nsec) / 1e9;
  printf("Total: %lld\nTime: %.6f sec\n", total_sum, elapsed);
  free(array);
  return 0;
}
