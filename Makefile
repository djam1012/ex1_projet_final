all: ChaineMain ReconstitueReseau

ChaineMain: Chaine.o SVGwriter.o ChaineMain.o
	gcc Chaine.o SVGwriter.o ChaineMain.o -o ChaineMain -lm

Chaine.o: Chaine.c Chaine.h SVGwriter.h
	gcc -c Chaine.c -o Chaine.o

SVGwriter.o: SVGwriter.c SVGwriter.h Chaine.h
	gcc -c SVGwriter.c -o SVGwriter.o

ChaineMain.o: ChaineMain.c Chaine.h SVGwriter.h
	gcc -c ChaineMain.c -o ChaineMain.o

ReconstitueReseau: Chaine.o Reseau.o SVGwriter.o ReconstitueReseau.o
	gcc Chaine.o Reseau.o SVGwriter.o ReconstitueReseau.o -o ReconstitueReseau -lm

Reseau.o: Reseau.c Reseau.h SVGwriter.h
	gcc -c Reseau.c -o Reseau.o

ReconstitueReseau.o: ReconstitueReseau.c Reseau.h Chaine.h SVGwriter.h
	gcc -c ReconstitueReseau.c -o ReconstitueReseau.o

clean:
	rm -f *.o
	rm -f ChaineMain
