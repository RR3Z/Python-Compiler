@echo off
type input\comments_tests.txt | ..\x64\Debug\Python-Compiler.exe > comments_result.txt
type input\string_literals_tests.txt | ..\x64\Debug\Python-Compiler.exe > string_literals_result.txt
type input\numeric_literals_tests.txt | ..\x64\Debug\Python-Compiler.exe > numeric_literals_result.txt