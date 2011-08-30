## Quickly compile LaTex documents with the make command
## Make sure this file is named "Makefile" (duh!)
all: *.tex
	pdflatex *.tex
test: *.pdf
	evince *.pdf 2> /dev/null &

clean:
	rm *.pdf
	rm *.log
	rm *.aux
