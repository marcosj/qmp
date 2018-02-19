# qmp
Quine-McCluskey + Petrick logic minimizer.<br/>
From <https://sites.google.com/site/simpogical/download>

## Compile
	g++ -o qmp qmp.cpp

## Run
	./qmp
	./qmp < file

## Input (until EOF)
	#variables <list of minterms> [d <list of don't cares>] + Enter + Ctrl-D (Ctrl-Z win)

### Ex. 1
	3 0 1 2 7 5 6
	Ctrl-D linux
	{ 3 vars, F = sum m(0,1,2,5,6,7) }

### Ex. 2
	4 9 12 13 15 d 1 4 5 7 8 11 14
	Ctrl-Z win
	{ 4 vars, F = sum m(9,12,13,15) + sum d(1,4,5,7,8,11,14) }
