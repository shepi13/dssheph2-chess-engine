# dssheph2-chess-engine

An executable will be provided with this repository, configured to depth 4.
To change the depth, change maxDepth in eval.c and recompile with
gcc -std=c99 -O2 -o game eval.c board.c twoPlayer.c
Any depth 6 and below should be acceptable, with some slight delay in 
calculation on depth 6. I chose 4 because I felt even depths were
slightly stronger although this is untested and I did not want to
take the performance hit of depth 6. More testing/modifications
are needed.

Note: in it's current state entering q quits and entering r resets to
the starting chess position. It does not start in the starting position
due to testing purposes and will not until I feel it is finished (not
just for cs241), so just input r at the start to get to the starting
chess position.
