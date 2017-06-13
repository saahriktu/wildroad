all:
	cc -o wildroad wildroad.c -lcurses
install:
	cp wildroad /usr/games/wildroad
