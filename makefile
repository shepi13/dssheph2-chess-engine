textGame: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	gcc -o textGame -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

test: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	gcc -o test -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

windows: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	x86_64-w64-mingw32-gcc -o myChess.exe -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

glChess: main.c graphics.c graphics.h myGLIncludes.h
	gcc -o glChess -Ofast -std=c99 main.c graphics.c -lGL -lGLU -lglut

clean:
	rm test
	rm *.o
