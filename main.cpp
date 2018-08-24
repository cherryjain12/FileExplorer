#include <iostream>
#include "headers.h"
using namespace std;

void top_bottom_bar(int row, int col) {
    string str = "...Welcome to the file explorer...";
    int len = str.length();
    int start_col = col/2 - len/2;
    move_cursor(row-1, 1);
    cout<<GREEN<<"cwd: "<<WHITE<<get_cwd();
    move_cursor(1, start_col);
    cout<<GREEN<<str;
    move_cursor(2,1);
    for(int i=0;i<col;i++)
        cout<<".";
    nl();
}

int main() {
    cout<<CLEAR;
    int *ptr = get_row_col("end");
    top_bottom_bar(ptr[0], ptr[1]);
    ls_cmd(".");
    char key_press;
    while(true) {
        key_press = check_keypress();
        if(key_press == -10)
            move_cursor(--cur_row,0);
        else if(key_press == -11)
            move_cursor(++cur_row,0);
        else if(key_press == -12)
            cout<<"Right";
        else if(key_press == -13)
            cout<<"Left";
        else if(key_press == 27)
            break;
    }

    if(key_press == 27)
        cout<<CLEAR;

    return 0;
}
