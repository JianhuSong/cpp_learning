CC=g++
VPATH=func:func1:common

test : func.o main.o func1.o
	@echo Compiling test program......
	${CC} -o test main.o func.o func1.o
clean :
	-rm test.exe main.o func.o func1.o
