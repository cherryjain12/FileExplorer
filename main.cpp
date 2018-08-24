#include <iostream>
#include "headers.h"
using namespace std;

void top_bottom_bar(int row, int col) {
    string str = "...Welcome to the file explorer...";
    int len = str.length();
    int start_col = col/2 - len/2;
    move_cursor(row-1, 1);
    cout<<GREEN<<"cwd: "<<WHITE<<pwd;
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
    vector <struct dirent *> dir_list = ls_cmd(".");
    char key_press;
    while(true) {
        key_press = check_keypress();
        if(key_press == -10)
            move_cursor(cur_row-1,0);
        else if(key_press == -11)
            move_cursor(cur_row+1,0);
        else if(key_press == -12);
            //cout<<"Right";
        else if(key_press == -13);
            //cout<<"Left";
        else if(key_press == 27)
            break;
        else if(key_press == 10) {
            if(cur_row >=6 and cur_row < dir_list.size()+6) {
                char *move_to_dir = dir_list[cur_row-6]->d_name;
                if(!pwd.compare(home) && (move_to_dir[0]=='.' && move_to_dir[1]=='.' && move_to_dir[2]==0));
                else {
                    if(dir_list[cur_row-6]->d_type == DT_DIR ) {
                        string move_to = "";
                        move_to.append(move_to_dir);
                        if(move_to.compare(".")) {
                            cout<<CLEAR;
                            int i;
                            modify_wd(move_to);
                            for(i=0;pwd[i]!=0;i++)
                                move_to_dir[i]=pwd[i];
                            move_to_dir[i]=0;
                            top_bottom_bar(ptr[0], ptr[1]);
                            dir_list.clear();
                            dir_list = ls_cmd(move_to_dir);
                        }
                    }
                }
            }
        }
    }

    if(key_press == 27)
        cout<<CLEAR;

    return 0;
}
