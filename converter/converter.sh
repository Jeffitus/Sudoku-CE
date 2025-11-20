#!/bin/bash

./converter.py
convbin -j csv -k 8xv -i easy.csv -o SudoEasy.8xv -n SudoEasy -r
convbin -j csv -k 8xv -i easy.csv -o SudoEas2.8xv -n SudoEas2 -r
convbin -j csv -k 8xv -i easy.csv -o SudoEas3.8xv -n SudoEas3 -r