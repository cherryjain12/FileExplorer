#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <termios.h>
#include <stdlib.h>
using namespace std;

#ifndef INC_HEADERS_H
#define INC_HEADERS_H
#define CLEAR "\033[2J"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"
#define LRED "\x1b[91m"
#define YELLOW "\x1b[33m"
#define LCYAN "\x1b[96m"
#define BPURPLE "\x1b[45m"
#define BBLACK "\x1b[40m"
#define BRED "\x1b[41m"
#define BLGREY "\x1b[100m"
#define BLGREEN "\x1b[102m"
#define BLBLUE "\x1b[104m"
#define BLYELLOW "\x1b[103m"
#define BLMAGENTA "\x1b[105m"
#define BLCYAN "\x1b[106m"
#define BOLDON "x1b[1m"
#define BOLDOFF "x1b[21m"
#define UNDERLINEON "x1b[4m"
#define UNDERLINEOFF "x1b[24m"

string get_cwd();

int* get_row_col(string);

void ls_cmd(char *);

void move_cursor(int, int);

extern int cursor_row, cursor_col;

void nl();

char check_keypress();

extern int win_row, win_col, cur_row, cur_col;

#endif  /*INC_MODULE1_H*/
