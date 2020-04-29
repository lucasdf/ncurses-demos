#include <ncurses.h>

int main(int argc, char *argv[]) {
  initscr();
  start_color();

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  printw("A Big string which i didn't care to type fully");
  /*
   * mvchgat
   * 1. initial y
   * 2. initial x
   * 3. -1 up to end of line
   * 4. attribute
   * 5. color index, or 0 for no color
   * 6. null :p
   */
  mvchgat(0, 0, -1, A_BLINK, COLOR_RED, NULL);
  refresh();
  getch();
  endwin();
  return 0;
}
