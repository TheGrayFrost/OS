a.out: filesys.h libfsys.a
	g++ filesys.cpp libfsys.a
libfsys.a: fs.o in.o sb.o structures.h
	ar -cvq libfsys.a fs.o in.o sb.o
fs.o: fs.cpp structures.h
	g++ -c fs.cpp
in.o: in.cpp structures.h
	g++ -c in.cpp
sb.o: sb.cpp structures.h
	g++ -c sb.cpp

clean:
	rm a.out *.a *.o