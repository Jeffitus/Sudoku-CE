#!/usr/bin/python

with open("easy.txt", "r") as infile:
  allPuzzles = infile.readlines()

  with open("easy.csv", "w") as outfile:
    # header
    for char in "Sudoku":
      outfile.write(str(ord(char)) + ",")
    # 100 puzzles in this var
    for i in range(0, 99):
      puzzle = allPuzzles[i]
      [hash, puzzleData, difficulty] = puzzle.split()
      difficulty = int(float(difficulty) * 10)
      for char in puzzleData:
        outfile.write(char)
        outfile.write(",")
      outfile.write(str(difficulty) + "\n")