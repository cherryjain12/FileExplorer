#include <iostream>
#include <vector>
#include <iterator>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <ctime>
#include <string.h>
#include <algorithm>
using namespace std;

#ifndef INC_HEADERS_H
#define INC_HEADERS_H
#define CLEAR "\033[2J"
#define LINE_CLEAR "\033[2K"
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

extern int win_row, win_col, cur_row, cur_col, list_row, list_col, windows_capacity, cmd_cur;

extern string pwd, home, twd, MODE;

extern string cmd;

extern vector <string> dir_his;

extern vector<string>::iterator his_itr;

extern vector<string> list_cmds;

int* get_row_col(string);

vector <struct dirent *>ls_cmd(char *);

string get_cwd();

void display(vector <struct dirent *>&, int, int, bool, string);

void move_cursor(int, int, bool, string);

char check_keypress();

void top_bottom_bar(int, int);

void modify_wd(string dest_dir);

void place_cursor(int, int);

void onPressLeftN(vector <struct dirent *> &, bool &, int &);

void onPressRightN(vector <struct dirent *> &, bool &, int &);

void onPressBackN(vector <struct dirent *> &, bool &, int &);

void onPressHomeN(vector <struct dirent *> &, bool &, int &);

void onPressEnterN(vector <struct dirent *> &, bool &, int &);

void onPressDownN(vector <struct dirent *> &, bool, int &);

void onPressUpN(vector <struct dirent *> &, bool);

void switch_mode(vector <struct dirent *> &, int &, bool &);

vector <string> processCmd(int, string);

void show_error(string);

void onPressBackC();

void onPressEnterC(vector <struct dirent *> &, bool &, int &);

void storeCmd(char);

void init();


#endif  /*INC_MODULE1_H*/
