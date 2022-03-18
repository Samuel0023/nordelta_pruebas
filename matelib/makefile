CC=gcc

IDIR =../
SHARED_IDIR = ../../shared/include
LIBS=-lcommons -lpthread -lreadline -lcunit -lrt -lm
name=carpincho1

build-example-lib: 
	cd lib_implementation && \
	$(CC) -c -Wall -Werror -I"../" -I$(SHARED_IDIR) -lcommons -fpic *.c && \
	cd ../../shared/src && \
	pwd && \
	$(CC) -c -Wall -Werror -I$(SHARED_IDIR) -lcommons -fpic *.c && \
	cd ../../matelib/lib_implementation && \
	$(CC) -shared matelib.o ../../shared/src/*.o -o libmatelib.so $(LIBS) && \
	rm *.o && \
	cd ../../shared/src && \
	rm *.o

build-example: build-example-lib
	cd lib_usage && \
	$(CC) -g3 -Wall -L"../lib_implementation" -I"../" $(name).c -lcommons -lmatelib -lpthread -lrt -o $(name).exe

run: build-example
	cd lib_usage && \
	LD_LIBRARY_PATH="../lib_implementation/" ./$(name).exe

run-test: build-example
	cd lib_usage && \
	LD_LIBRARY_PATH="../lib_implementation/" valgrind --tool=memcheck --leak-check=yes --show-possibly-lost=no --show-reachable=no --num-callers=20 ./$(name).exe

clean:
	rm lib_implementation/libmatelib.so lib_usage/$(name).exe