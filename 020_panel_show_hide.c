#include <curses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>

typedef struct _PANEL_DATE {
  int hide;
} PANEL_DATA;

#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);

void init_colors() {
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
}

void init_panels(PANEL *my_panels[], WINDOW *my_wins[],
                 PANEL_DATA *panel_datas[]) {
  /* Attach a panel to each window */   /* Order is bottom up */
  my_panels[0] = new_panel(my_wins[0]); /* Push 0, order: stdscr-0 */
  my_panels[1] = new_panel(my_wins[1]); /* Push 1, order: stdscr-0-1 */
  my_panels[2] = new_panel(my_wins[2]); /* Push 2, order: stdscr-0-1-2 */

  /* Initialize panel datas saying that nothing is hidden */
  panel_datas[0] = malloc(sizeof(PANEL_DATA));
  panel_datas[0]->hide = FALSE;
  panel_datas[1] = malloc(sizeof(PANEL_DATA));
  panel_datas[0]->hide = FALSE;
  panel_datas[2] = malloc(sizeof(PANEL_DATA));
  panel_datas[0]->hide = FALSE;

  set_panel_userptr(my_panels[0], panel_datas[0]);
  set_panel_userptr(my_panels[1], panel_datas[1]);
  set_panel_userptr(my_panels[2], panel_datas[2]);
}

int main() {
  WINDOW *my_wins[3];
  PANEL *my_panels[3];
  PANEL_DATA *panel_datas[3];
  PANEL_DATA *temp;
  int ch;

  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_colors();

  init_wins(my_wins, 3);
  init_panels(my_panels, my_wins, panel_datas);
  update_panels();

  attron(COLOR_PAIR(4));
  mvprintw(LINES - 3, 0,
           "Show or Hide a window with 'a' (first window) 'b' (second window) "
           "'c' (third window)");
  mvprintw(LINES - 2, 0, "F1 to quit");
  attroff(COLOR_PAIR(4));
  doupdate();

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case 'a':
      temp = (PANEL_DATA *)panel_userptr(my_panels[0]);
      if (temp->hide == FALSE) {
        hide_panel(my_panels[0]);
        temp->hide = TRUE;
      } else {
        show_panel(my_panels[0]);
        temp->hide = FALSE;
      }
      break;
    case 'b':
      temp = (PANEL_DATA *)panel_userptr(my_panels[1]);
      if (temp->hide == FALSE) {
        hide_panel(my_panels[1]);
        temp->hide = TRUE;
      } else {
        show_panel(my_panels[1]);
        temp->hide = FALSE;
      }
      break;
    case 'c':
      temp = (PANEL_DATA *)panel_userptr(my_panels[2]);
      if (temp->hide == FALSE) {
        hide_panel(my_panels[2]);
        temp->hide = TRUE;
      } else {
        show_panel(my_panels[2]);
        temp->hide = FALSE;
      }
      break;
    }
    update_panels();
    doupdate();
  }
  endwin();
  return 0;
}
/* Put all the windows */
void init_wins(WINDOW **wins, int n) {
  int x, y, i;
  char label[80];

  y = 2;
  x = 10;
  for (i = 0; i < n; ++i) {
    wins[i] = newwin(NLINES, NCOLS, y, x);
    sprintf(label, "Window Number %d", i + 1);
    win_show(wins[i], label, i + 1);
    y += 3;
    x += 7;
  }
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color) {
  int startx, starty, height, width;

  getbegyx(win, starty, startx);
  getmaxyx(win, height, width);

  box(win, 0, 0);
  mvwaddch(win, 2, 0, ACS_LTEE);
  mvwhline(win, 2, 1, ACS_HLINE, width - 2);
  mvwaddch(win, 2, width - 1, ACS_RTEE);

  print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
