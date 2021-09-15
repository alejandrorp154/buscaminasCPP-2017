ejec: buscaminas.o
	g++ -Wall -o ejec buscaminas.cpp 

buscaminas.o: buscaminas.cpp
	g++ -Wall -c buscaminas.cpp

clean:
	rm -f ejec
	rm -f *.o
