CC= gcc
GLLIBS= -lGL -lGLU -lglut
FLAGS= -Ofast -std=c99
TEXTOBJS= board.o eval.o moves.o twoPlayer.o magicNumbers.o magicGeneration.o moveGeneration.o
TEXTEXE= textGame
GLOBJS = graphics.o main.o
GLEXE = glChess

all : textGame glChess

textGame : $(TEXTOBJS)
	$(CC) -o $(TEXTEXE) $(TEXTOBJS)

board.o : board.c board.h
	$(CC) $(FLAGS) -c board.c

eval.o : eval.c eval.h
	$(CC) $(FLAGS) -c eval.c 

moves.o : moves.c moves.h
	$(CC) $(FLAGS) -c moves.c

magicNumbers.o : magicNumbers.c magicNumbers.h
	$(CC) $(FLAGS) -c magicNumbers.c

magicGeneration.o : magicGeneration.c magicGeneration.h
	$(CC) $(FLAGS) -c magicGeneration.c

moveGeneration.o : moveGeneration.c moveGeneration.h
	$(CC) $(FLAGS) -c moveGeneration.c

twoPlayer.o : twoPlayer.c
	$(CC) $(FLAGS) -c twoPlayer.c

windows : board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c
	x86_64-w64-mingw32-gcc -o myChess.exe -Ofast -std=c99 board.c eval.c moves.c twoPlayer.c magicNumbers.c magicGeneration.c moveGeneration.c

glChess : $(GLOBJS)
	$(CC) -o $(GLEXE) $(GLOBJS) $(GLLIBS)

main.o : main.c
	$(CC) $(FLAGS) -c main.c

graphics.o : graphics.c graphics.h
	$(CC) $(FLAGS) -c graphics.c

clean:
	rm -f *.o
	rm -f textGame
	rm -f glChess
