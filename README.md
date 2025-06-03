# Sudoku Solver

A C program that solves Sudoku puzzles using constraint propagation and search algorithms.

## Features
- Reads Sudoku puzzles from `.ss` , `.sdx` or `.sdm` files
- Uses constraint propagation for efficient solving

## Requirements
- C compiler (gcc recommended)
- Make build system

## Building the Project
1. Clone the repository:
```bash
git clone https://github.com/nikicha2/Sudoku-solver.git
cd Sudoku-solver
```

2. Build the project:
```bash
make all
```

## Usage
Run the solver with a Sudoku puzzle file:
```bash
./main.exe path/to/puzzle.ss
```
And a file with the solution will be created in the root directory with a name `solution.sdx`, `solution.sdx` or `solution.sdm` depending on the input file type.

### Input File Format
The input file should can be:
- A version 4.2n `.ss` file with the following format:
```
    I426 (place 6 in row 4, column 2(indexed from 0))
    I123 (place 3 in row 1, column 2)
    ...
    I999 (place 9 in row 9, column 9)
```
- A `.sdx` file
- A `.sdm` file

## Example
```bash
# Build the project
make all

# Run with a puzzle file
./main.exe puzzles/easy.ss
```