all: rest

# Tool invocation
# Executable rest
rest: \
	bin/Dish.o \
	bin/Customer.o \
	bin/Table.o \
	bin/Restaurant.o \
	bin/Action.o \
	bin/Main.o \
	bin/rest \

bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

bin/Table.o: src/Table.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

bin/Main.o: src/Main.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

bin/rest:
	g++ -o bin/rest bin/Main.o bin/Restaurant.o bin/Dish.o bin/Table.o bin/Customer.o bin/Action.o

#Clean the build directory
clean:
	rm -f bin/*
