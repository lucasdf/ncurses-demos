#include <curses.h>
#include <panel.h>

int main() {
  WINDOW *my_wins[3];
  PANEL *my_panels[3];
  int lines = 10, cols = 40, y = 2, x = 4, i;

  initscr();
  cbreak();
  noecho();

  my_wins[0] = newwin(lines, cols, y, x);
  my_wins[1] = newwin(lines, cols, y + 1, x + 5);
  my_wins[2] = newwin(lines, cols, y + 2, x + 10);

  // add borders around windows
  for (i = 0; i < 3; ++i) {
    box(my_wins[i], 0, 0);
    mvwprintw(my_wins[i], 1, 1, "%d", i);
  }

  my_panels[0] = new_panel(my_wins[0]);
  my_panels[1] = new_panel(my_wins[1]);
  my_panels[2] = new_panel(my_wins[2]);

  update_panels();

  doupdate();

  getch();
  endwin();
}
