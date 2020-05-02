#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  initscr();

  printw("Hello world! \n");
  refresh();
  getch();

  def_prog_mode();
  endwin();
  system("/bin/sh");
  reset_prog_mode();

  refresh();
  printw("Back to ncurses");
  refresh();

  getch();
  endwin();
  return 0;
}
