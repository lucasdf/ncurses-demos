#include <curses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>

typedef struct _PANEL_DATA {
  int x, y, w, h;
  char label[80];
  int label_color;
  PANEL *next;
} PANEL_DATA;

#define NLINES 10
#define NCOLS 40

enum Command { CommandIdle, CommandMove, CommandResize };

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);
void set_user_ptrs(PANEL **panels, int n);

int process_command(enum Command command, int newx, int newy, int newh,
                    int neww, PANEL *stack_top, PANEL_DATA *top) {
  WINDOW *temp_win;
  WINDOW *old_win;

  switch (command) {
  case CommandResize:
    old_win = panel_window(stack_top);
    temp_win = newwin(newh, neww, newy, newx);
    replace_panel(stack_top, temp_win);
    win_show(temp_win, top->label, top->label_color);
    delwin(old_win);
    break;
  case CommandMove:
    move_panel(stack_top, newy, newx);
    break;
  default:
    break;
  }
  return 0;
}

void print_current_command(enum Command command) {
  attron(COLOR_PAIR(4));
  mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
  mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
  attroff(COLOR_PAIR(4));
  if (command == CommandResize) {
    attron(COLOR_PAIR(4));
    mvprintw(LINES - 4, 0,
             "Entered Resizing :Use Arrow Keys to resize and press <ENTER> "
             "to end resizing");
    attroff(COLOR_PAIR(4));
  } else if (command == CommandMove) {
    attron(COLOR_PAIR(4));
    mvprintw(LINES - 4, 0,
             "Entered Moving: Use Arrow Keys to Move and press <ENTER> to "
             "end moving");
    attroff(COLOR_PAIR(4));
  } else {
    move(LINES - 4, 0);
    clrtoeol();
  }
}

int main() {
  WINDOW *my_wins[3];
  PANEL *my_panels[3];
  PANEL_DATA *top;
  PANEL *stack_top;
  int ch;
  int newx, newy, neww, newh;
  enum Command command = CommandIdle;

  /* Initialize curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  /* Initialize all the colors */
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);

  init_wins(my_wins, 3);

  /* Attach a panel to each window */   /* Order is bottom up */
  my_panels[0] = new_panel(my_wins[0]); /* Push 0, order: stdscr-0 */
  my_panels[1] = new_panel(my_wins[1]); /* Push 1, order: stdscr-0-1 */
  my_panels[2] = new_panel(my_wins[2]); /* Push 2, order: stdscr-0-1-2 */

  set_user_ptrs(my_panels, 3);
  /* Update the stacking order. 2nd panel will be on top */
  update_panels();

  /* Show it on the screen */
  attron(COLOR_PAIR(4));
  mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
  mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
  attroff(COLOR_PAIR(4));
  doupdate();

  stack_top = my_panels[2];
  top = (PANEL_DATA *)panel_userptr(stack_top);
  newx = top->x;
  newy = top->y;
  neww = top->w;
  newh = top->h;
  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case 9: /* Tab */
      top = (PANEL_DATA *)panel_userptr(stack_top);
      top_panel(top->next);
      stack_top = top->next;
      top = (PANEL_DATA *)panel_userptr(stack_top);
      newx = top->x;
      newy = top->y;
      neww = top->w;
      newh = top->h;
      break;
    case 'r': /* Re-Size*/
      command = CommandResize;
      break;
    case 'm': /* Move */
      command = CommandMove;
      break;
    case KEY_LEFT:
      switch (command) {
      case CommandResize:
        --newx;
        ++neww;
        break;
      case CommandMove:
        --newx;
        break;
      default:
        break;
      }
      process_command(command, newx, newy, newh, neww, stack_top, top);
      break;
    case KEY_RIGHT:
      if (command == CommandResize) {
        ++newx;
        --neww;
      } else if (command == CommandMove)
        ++newx;
      process_command(command, newx, newy, newh, neww, stack_top, top);
      break;
    case KEY_UP:
      if (command == CommandResize) {
        --newy;
        ++newh;
      } else if (command == CommandMove)
        --newy;
      process_command(command, newx, newy, newh, neww, stack_top, top);
      break;
    case KEY_DOWN:
      if (command == CommandResize) {
        ++newy;
        --newh;
      } else if (command == CommandMove)
        ++newy;
      process_command(command, newx, newy, newh, neww, stack_top, top);
      break;
    case 10: /* Enter */
      command = CommandIdle;
      break;
    }
    print_current_command(command);
    refresh();
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

/* Set the PANEL_DATA structures for individual panels */
void set_user_ptrs(PANEL **panels, int n) {
  PANEL_DATA *ptrs;
  WINDOW *win;
  int x, y, w, h, i;
  char temp[80];

  ptrs = (PANEL_DATA *)calloc(n, sizeof(PANEL_DATA));

  for (i = 0; i < n; ++i) {
    win = panel_window(panels[i]);
    getbegyx(win, y, x);
    getmaxyx(win, h, w);
    ptrs[i].x = x;
    ptrs[i].y = y;
    ptrs[i].w = w;
    ptrs[i].h = h;
    sprintf(temp, "Window Number %d", i + 1);
    strcpy(ptrs[i].label, temp);
    ptrs[i].label_color = i + 1;
    if (i + 1 == n)
      ptrs[i].next = panels[0];
    else
      ptrs[i].next = panels[i + 1];
    set_panel_userptr(panels[i], &ptrs[i]);
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
  refresh();
}
