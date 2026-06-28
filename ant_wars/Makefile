CXXFLAGS=-Wall -pedantic -fsanitize=address -g
OBJ=main.o game_data.o game_handler.o place_on_map.o player_entity.o race_bonus_tree.o user_interface.o warrior.o

.PHONY: all
all: bulanma3 doc

.PHONY: compile
compile: $(OBJ)
	g++ $(CXXFLAGS) $^ -o bulanma3
	
bulanma3: $(OBJ)
	g++ $(CXXFLAGS) $^ -o bulanma3

main.o: src/main.cpp src/game_handler.cpp src/game_handler.h
	g++ $(CXXFLAGS) -c src/main.cpp -o main.o

game_data.o: src/game_data.cpp src/game_data.h src/place_on_map.h src/place_on_map.cpp
	g++ $(CXXFLAGS) -c src/game_data.cpp

game_handler.o: src/game_handler.cpp src/game_handler.h src/player_entity.cpp src/player_entity.h src/user_interface.cpp src/user_interface.h
	g++ $(CXXFLAGS) -c src/game_handler.cpp

place_on_map.o: src/place_on_map.cpp src/place_on_map.h src/race_bonus_tree.cpp src/race_bonus_tree.h
	g++ $(CXXFLAGS) -c src/place_on_map.cpp

player_entity.o: src/player_entity.cpp src/player_entity.h src/game_data.cpp src/game_data.h
	g++ $(CXXFLAGS) -c src/player_entity.cpp

race_bonus_tree.o: src/race_bonus_tree.cpp src/race_bonus_tree.h src/warrior.cpp src/warrior.h
	g++ $(CXXFLAGS) -c src/race_bonus_tree.cpp

user_interface.o: src/user_interface.cpp src/user_interface.h
	g++ $(CXXFLAGS) -c src/user_interface.cpp

warrior.o: src/warrior.cpp src/warrior.h
	g++ $(CXXFLAGS) -c src/warrior.cpp

.PHONY: clean
clean:
	rm -f *.o bulanma3; 
	find ./doc -type f -not -name 'Doxyfile' -not -name 'class_diagram.png' -not -name 'README.md' -delete

.PHONY: run
run: bulanma3
	./bulanma3

.PHONY: doc
doc:
	cd doc; doxygen; mv html/* .; rm -rf html latex search; cd ..

