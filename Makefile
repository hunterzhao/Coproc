run : main.o Scheduler.o Coproc.o
	g++ -o run main.o Scheduler.o Coproc.o

main.o : main.cc Scheduler.h Coproc.h
	g++ -c -std=c++11 -g main.cc

Scheduler.o: Scheduler.h Coproc.h Scheduler.cc
	g++ -c -std=c++11 -g Scheduler.cc

Coproc.o: Coproc.h Scheduler.h Coproc.cc
	g++ -c -std=c++11 -g Coproc.cc

clean:
	rm run Scheduler.o Coproc.o main.o