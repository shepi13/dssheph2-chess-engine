game: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c
	gcc -o game -O3 -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c

test: board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	gcc -o test -O3 -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

clean:
	rm test
