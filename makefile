game: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	gcc -o game -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

test: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	gcc -o test -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

windows: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	x86_64-w64-mingw32-gcc -o myChess.exe -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

clean:
	rm test
