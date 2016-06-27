CC= gcc
ILFLAGS= $(shell pkg-config --cflags ILU)
FLAGS= -Ofast -std=c99 $(ILFLAGS)

TEXTOBJS= board.o eval.o moves.o twoPlayer.o magicNumbers.o magicGeneration.o moveGeneration.o
TEXTEXE= textGame

GLLIBS= -lGL -lGLU -lglut
GLOBJS= pieceIcon.o graphics.o main.o
GLEXE= glChess

ILLIBS= $(shell pkg-config --libs ILU)

#To compile for windows, set CC=$(WINCC)
#Still need to remove getline references before this will work
#for textGame
WINCC= x86_64-w64-mingw32-gcc

all : textGame glChess

textGame : $(TEXTOBJS)
	$(CC) -o $(TEXTEXE) $(TEXTOBJS)

glChess : $(GLOBJS)
	$(CC) -o $(GLEXE) $(GLOBJS) $(GLLIBS) $(ILLIBS)

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

main.o : main.c
	$(CC) $(FLAGS) -c main.c

pieceIcon.o : pieceIcon.c pieceIcon.h
	$(CC) $(FLAGS) -c pieceIcon.c

graphics.o : graphics.c graphics.h
	$(CC) $(FLAGS) -c graphics.c

clean:
	rm -f *.o
	rm -f textGame
	rm -f glChess
