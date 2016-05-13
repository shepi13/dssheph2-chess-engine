# dssheph2-chess-engine

An executable will be provided with this repository, configured to depth 6.
To change the depth, change maxDepth in eval.c and recompile with
gcc -std=c99 -O2 -o game eval.c board.c twoPlayer.c

Note: in it's current state enterring q quits and enterring r resets to
the starting chess position. It does not start in the starting position
due to testing purposes and will not until I feel it is finished (not
just for cs241), so just input r at the start to get to the starting
chess position.
