#!/bin/bash

echo "Количество аргументов: $#";

sum=0;
for arg in $@; do
  ((sum += arg))
done

count=$#;
average=$((sum / count));
echo "Сумма аргументов: $sum";
echo "Средне арифм: $average";
