2snake_cli: out/2snake.o out/ncurses_display.o out/helper.o
	mkdir -p compiled
	gcc out/2snake.o out/ncurses_display.o out/helper.o -lncurses -o compiled/2snake_cli

out/2snake.o: 2snake.c display.h settings.h helper.h 
	mkdir -p out
	gcc -c 2snake.c -o out/2snake.o

out/ncurses_display.o: ncurses_display.c display.h settings.h
	mkdir -p out
	gcc -c ncurses_display.c -o out/ncurses_display.o

out/helper.o: helper.c
	mkdir -p out
	gcc -c helper.c -o out/helper.o
