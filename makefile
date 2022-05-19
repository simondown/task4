CC=mpic++
CPPFLAGS= -O3 -lm -ldl -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wno-suggest-attribute=format

all: hello

hello: main.o function.o text.o print.o vector.o solution.o norma.o accuracy.o
	$(CC) $(CPPFLAGS) main.o function.o text.o print.o vector.o solution.o norma.o accuracy.o

main.o: main.cpp
	$(CC) $(CPPFLAGS) -g -c main.cpp

function.o: function.cpp
	$(CC) $(CPPFLAGS) -c function.cpp

text.o: text.cpp
	$(CC) $(CPPFLAGS) -c text.cpp

print.o: print.cpp
	$(CC) $(CPPFLAGS) -c print.cpp

vector.o: vector.cpp
	$(CC) $(CPPFLAGS) -c vector.cpp

solution.o: solution.cpp
	$(CC) $(CPPFLAGS) -c solution.cpp

norma.o: norma.cpp
	$(CC) $(CPPFLAGS) -c norma.cpp

accuracy.o: accuracy.cpp
	$(CC) $(CPPFLAGS) -c accuracy.cpp


clean:
	rm -rf *.o hello


