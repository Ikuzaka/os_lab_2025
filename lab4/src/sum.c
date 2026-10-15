#include "sum.h"

long long sum(const int *array, int begin, int end)
{
  long long sum = 0;
  for (int i = begin; i < end; i++)
   sum += array[i];
  return sum;
}
