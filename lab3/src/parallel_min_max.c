#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
              printf("Seed must be a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
              printf("Array must be a positive number\n");
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
              printf("Proccess must be a positive number\n");
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
  int (*pipes)[2];
  FILE **files;

  if (with_files) {
      files = calloc((size_t) pnum, sizeof(*files));
      if (files == NULL) {
          fprintf(stderr, "Error of tmp files!\n");
          return 1;
      }
  } else {
      pipes = malloc((size_t) pnum * sizeof(*pipes));
      if (pipes == NULL) {
          fprintf(stderr, "Error of pipes!\n");
      }
  }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (int i = 0; i < pnum; i++) {
    if (with_files) {
      files[i] = tmpfile();
    } else {
      pipe(pipes[i]);
    }

    pid_t child_pid = fork();

    if (child_pid >= 0) {
      // successful fork
      if (child_pid != 0 && !with_files) {
          close(pipes[i][1]);
      }
      active_child_processes += 1;
      if (child_pid == 0) {
        if (!with_files)
            close(pipes[i][0]);
        // child process
        unsigned int begin = (unsigned int) i * array_size / pnum;
        unsigned int end = (unsigned int) (i + 1) * array_size / pnum;
        // parallel somehow
        struct MinMax part = GetMinMax(array, begin, end);

        if (with_files) {
          fwrite(&part, sizeof(struct MinMax), 1, files[i]);
        } else {
          write(pipes[i][1], &part, sizeof(struct MinMax));
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    int status;
    pid_t pid;
    pid = wait(&status);
    if (pid == -1) {
      printf("Error of child process!");
    }
    printf("Proccess %ld returned code %d\n", (long) pid, status);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;
    struct MinMax part;

    if (with_files) {
      rewind(files[i]);
      if (fread(&part, sizeof(part), 1, files[i]) != 1) {
        fprintf(stderr, "Cannot read result from temporary file %d\n", i);
        return 1;
      }
      fclose(files[i]);
    } else {
      size_t received = 0;
      while (received < sizeof(part)) {
        ssize_t count = read(pipes[i][0], (char *)&part + received,
                             sizeof(part) - received);
        if (count <= 0) {
          fprintf(stderr, "Cannot read result from pipe %d\n", i);
          return 1;
        }
        received += (size_t)count;
      }
      close(pipes[i][0]);
    }
    min = part.min;
    max = part.max;

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  
  if (with_files) {
      free(files);
  } else {
      free(pipes);
  }

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
