# Johnny goes shopping

## Description
This project implements the Complete Karmarkar-Karp (K-K) algorithm as an anytime algorithm to solve the "Johnny goes shopping" problem from SPOJ (bipartition problem).

Various constraints were imposed on the code due to the requirements of SPOJ and the guidelines set forth by my professor. For instance, the entire implementation had to be contained within a single file. These constraints, while peculiar, were necessary for compliance and to earn extra credit in my Algorithm Analysis University Course.

Although I considered working on this project, enhancing the efficiency, readability, and modularizing the code by refactoring it into multiple files, I opted against it. This decision stemmed from my intention to rebuild the project from scratch when I have the time. In hindsight some of the choices made in this implementation are rather unorthodox and werid if not plain dumb .
## Features
- Utilizes the Karmarkar-Karp algorithm, which consists of four steps:
  1. Order the numbers from largest to smallest.
  2. Replace the largest and second-largest numbers by their difference.
  3. If two or more numbers remain, return to step 1.
  4. Using backtracking, compute the partition.
- The project includes original implementations of the algorithm, with no direct code references from external sources.
- Provides functionalities for generating random numbers, sorting, and tree creation.
- Implements backtracking to compute the partition and find the optimal solution.

## Implementation Details
- Written in C++, the project includes several data structures and functions to support the CK-K algorithm.
- Utilizes smart pointers for memory management.
- Includes functions for input/output, differencing, summation, sorting, binary search, tree creation, and backtracking.
- Implements strategies for node creation, pruning, and limiting the search space based on runtime constraints.
- Provides options to print the original indexes of numbers or their sum as the output.

## Usage
1. Input the number of weights and the weights themselves.
2. The algorithm orders the weights and constructs a binary tree.
3. Depending on the number of weights, it executes the K-K algorithm to find the optimal solution.
4. Outputs the original indexex of the numbers in set one.

## Acknowledgments
- The project references instructions and examples provided by the professor.
- It draws insights from the Wikipedia page on the Karmarkar-Karp algorithm for conceptual understanding.

