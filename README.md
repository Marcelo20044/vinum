# Alcohol-oriented programming language Vinum
## Описание проекта
Язык программирования, написанный командой студентов по университетскому предмету "Платформы и среды выполнения языков программирования". Название Vinum не случайно и отображает основную особенность ЯП - большинство служебных слов так или иначе связаны с алкоголем, например: 'beef' - if, 'pour' - for и тд.

**Project stack:** 
С++

## Инструкция по запуску
1. Склонировать проект
2. Запустить интересующие .vnm файлы

### Array sorting example:
```
fun printArr(pinta arr[]) {
    pour pinta i = 0; i < len(arr); i = i + 1 {
        toast(arr[i] + " ")
    }
}

pinta arr[5] = {2, 5, 1, 3, 4} 
toast("before sort: ")
printArr(arr)

pour pinta i = 1; i < n; i = i + 1 {
    pour pinta j = i; j > 0; j = j - 1 {
        beef arr[j - 1] > arr[j] {
            pinta temp = arr[j - 1]
            arr[j - 1] = arr[j]
            arr[j] = temp
        }
    }
}

toast("\nafter sort: ")
printArr(arr)
```
## Уточнения по технической части
В формате проекта участниками команды были реализованы:
- Лексер
- Парсер
- Компилятор для формирования AST (формирует LLVM IR)
- Оптимизации
- JIT
- Gc
- Vm

## Тесты
### Task 1: Factorial Calculation (Recursive Function)
Obiective: Implement a function that calculates the factorial of a given number using recursion.
Purpose: Test recursive function calls, stack management, and handling of large numbers.
Benchmark: Measure the execution time for calculating the factorial of numbers like 10! or 20!.

### Task 2: Array Sorting (Sorting Algorithm)
Objective: Implement an algorithm to sort an array (e.g., quicksort or merge sort).
Purpose: Test array handling, loop operations, and element comparison.
Benchmark: Measure the time taken to sort a randomly generated array of, for example, 1000 or 10,000 elements.

### Task 3: Prime Number Generation (Sieve of Eratosthenes)
Objective: Implement an algorithm to generate prime numbers (e.g.,
Sieve of Eratosthenes).
Purpose: Test array manipulation, loops, and arithmetic operations.
Benchmark: Measure the time it takes to generate all prime numbers up to 100,000.
