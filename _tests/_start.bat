@echo off
type input\comments_tests.txt | ..\x64\Debug\Python-Compiler.exe > comments_result.txt
type input\string_literals_tests.txt | ..\x64\Debug\Python-Compiler.exe > string_literals_result.txt
type input\multiline_string_literals_tests.txt | ..\x64\Debug\Python-Compiler.exe > multiline_string_literals_result.txt
type input\numeric_literals_tests.txt | ..\x64\Debug\Python-Compiler.exe > numeric_literals_result.txt
type input\delimiters_tests.txt | ..\x64\Debug\Python-Compiler.exe > delimiters_result.txt
type input\keywords_tests.txt | ..\x64\Debug\Python-Compiler.exe > keywords_result.txt
type input\operators_test.txt | ..\x64\Debug\Python-Compiler.exe > operators_result.txt
type input\identifiers_test.txt | ..\x64\Debug\Python-Compiler.exe > identifiers_result.txt
type input\data_types_tests.txt | ..\x64\Debug\Python-Compiler.exe > data_types_result.txt
type input\error_tests.txt | ..\x64\Debug\Python-Compiler.exe > error_result.txt
type input\dent_tests.txt | ..\x64\Debug\Python-Compiler.exe > dent_result.txt
