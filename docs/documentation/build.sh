#!/bin/bash

echo "Building docs..."

pandoc --pdf-engine xelatex -s -o Guitar-Transceiver-Documentation.pdf \
	01_Cover.md \
	02_Abstract.md \
	03_Introduction.md \
	04_Problem.md \
	05_Test_Environment.md \
	06_Project_Design.md \
	07_Result.md \
	08_Conclusion.md
