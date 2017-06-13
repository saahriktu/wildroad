/*
 *  Wildroad - ncurses arcade wild road game
 *  Copyright (C) 2010-2011 Kurashov Artem Konstantinovich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

WINDOW *gw;

#define ROADLENGTH 31000

static int
get_random (int g_rand_max)
{
  return (int) ((g_rand_max + 1.0) * rand () / (RAND_MAX + 1.0));
}

void
putwinmsg (char *msgptr)
{
  WINDOW *mw;
  mw = newwin (3, strlen (msgptr) + 2, LINES / 2 - 1,
	       COLS / 2 - strlen (msgptr) / 2 - 1);
  box (mw, 0, 0);
  mvwaddstr (mw, 1, 1, msgptr);
  wrefresh (mw);
  getch ();
  delwin (mw);
}

int main (int argc, char **argv)
{
  int di, dj, cx, cy, rx, rxd, rw, lrx, ukey0, speed;
  char crt;
  int roadborder = 0x7c;
  (void) initscr ();
  (void) clear ();
  (void) noecho ();
  (void) curs_set (0);
  (void) cbreak ();
  srand (time (0));
  gw = newpad (ROADLENGTH, COLS);
  (void) keypad (stdscr, TRUE);
  (void) nodelay (stdscr, true);
  wrefresh (gw);
  rx = COLS / 2 - 10;
  rw = 20;
  speed = 25000;
  for (dj = ROADLENGTH; dj >= 0; dj--)
    {
      for (di = rx; di < rx + rw + 1; di += rw)
	mvwaddch (gw, dj, di, roadborder);
      mvwaddch (gw, dj, rx + rw / 2, '.');
      if (get_random (5) == 3)
	mvwaddch (gw, dj, get_random (COLS - 1), 'O');
      if (dj == ROADLENGTH)
	cx = rx + rw / 2;
      rxd = get_random (2) - 1;
      rx += rxd;
      switch (rxd)
	{
	case -1:
	  roadborder = 0x5c;
	  break;
	case 0:
	  roadborder = 0x7c;
	  break;
	case 1:
	  roadborder = 0x2f;
	}
      rw += get_random (2) - 1;
      if (rw < 10)
	rw = 10;
      if (rw > COLS / 2)
	rw = COLS / 2;
      if (rx < 0)
	rx = 0;
      if (rx + rw > COLS - 1)
	rx = lrx;
      lrx = rx;
    }
  cy = ROADLENGTH;
  for (dj = LINES - 1; dj < ROADLENGTH; dj++)
    {
      wmove (gw, cy, cx);
      crt = winch (gw);
      waddch (gw, 'I');
      prefresh (gw, ROADLENGTH - dj, 0, 0, 0, LINES - 1, COLS - 1);
      usleep (speed);
      mvwaddch (gw, cy, cx, crt);
      ukey0 = getch ();
      switch (ukey0)
	{
	case 260:
	  if (cx == 0)
	    break;
	  wmove (gw, cy, cx - 1);
	  if (winch (gw) != ' ' && winch (gw) != ':')
	    break;
	  cx--;
	  break;
	case 261:
	  if (cx == COLS - 1)
	    break;
	  wmove (gw, cy, cx + 1);
	  if (winch (gw) != ' ' && winch (gw) != ':')
	    break;
	  cx++;
	  break;
	case 259:
	  if (cy == 0)
	    break;
	  wmove (gw, cy - 1, cx);
	  if (winch (gw) != ' ' && winch (gw) != ':')
	    break;
	  cy--;
	  break;
	case 258:
	  if (cy == ROADLENGTH)
	    break;
	  wmove (gw, cy + 1, cx);
	  if (winch (gw) != ' ' && winch (gw) != ':')
	    break;
	  cy++;
	  break;
	case 13:
	  // if(cy<ROADLENGTH-LINES)break;
	  speed -= 10000;
	  if (speed < 10000)
	    speed = 5000;
	  break;
	case 32:
	  // if(cy<ROADLENGTH-LINES)break;
	  speed = 30000;
	  // if(speed>30000)speed=30000;
	}
      cy--;
      wmove (gw, cy, cx);
      if (winch (gw) != ' ' && winch (gw) != '.')
	{
	  putwinmsg ("GAME OVER");
	  endwin ();
	  return 0;
	}
    }
  putwinmsg ("FINISH !");
  endwin ();
  return 0;
}
