a.out: main.cc headers.h pokeClasses.h constants.h map.h trainer.h
	g++ -std=c++17 main.cc
clean:
	rm -f a.out *.o
