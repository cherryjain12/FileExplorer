#include <iterator>
#include <vector>
#include "headers.h"
using namespace std;

vector <string> dir_his;
vector<string>::iterator itr;

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

void onPressEnter(vector <struct dirent *> &dir_list) {
    if(cur_row >=6 and cur_row < dir_list.size()+6) {
        char *move_to_dir = dir_list[cur_row-6]->d_name;
        if(!pwd.compare(home) && (move_to_dir[0]=='.' && move_to_dir[1]=='.' && move_to_dir[2]==0));
        else {
            cout<<"inside else, pressed enter"<<move_to_dir;
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
                    top_bottom_bar(win_row, win_col);
                    dir_list.clear();
                    dir_list = ls_cmd(move_to_dir);
                    dir_his.push_back(pwd);
                    itr = dir_his.end();
                    itr = prev(itr, 1);
                }
            }
        }
    }
}

void onPressLeft(vector <struct dirent *> &dir_list) {
    if(itr != dir_his.begin()) {
        itr = prev(itr, 1);
        dir_list.clear();
        string str = *itr;
        int len = str.length(), i;
        char move_to_dir[len];
        cout<<CLEAR;
        pwd.replace(pwd.begin(), pwd.end(), str);
        top_bottom_bar(win_row, win_col);
        for(i=0;i<len;i++) {
            move_to_dir[i] = str[i];
        }
        move_to_dir[i] = 0;
        dir_list = ls_cmd(move_to_dir);
    }
}

void onPressRight(vector <struct dirent *> &dir_list) {
    vector<string>::iterator ftr = next(itr,1);
    if(ftr != dir_his.end()) {
        itr = next(itr, 1);
        dir_list.clear();
        string str = *itr;
        int len = str.length(), i;
        cout<<CLEAR;
        pwd.replace(pwd.begin(), pwd.end(), str);
        top_bottom_bar(win_row, win_col);
        char move_to_dir[len];
        for(i=0;i<len;i++) {
            move_to_dir[i] = str[i];
        }
        move_to_dir[i] = 0;
        dir_list = ls_cmd(move_to_dir);
    }
}

void onPressHome(vector <struct dirent *> &dir_list) {
    dir_his.push_back(home);
    itr = dir_his.end();
    itr = prev(itr, 1);
    dir_list.clear();
    cout<<CLEAR;
    int len = home.length(), i;
    char move_to_dir[len];
    for(i=0;i<len;i++) {
        move_to_dir[i] = home[i];
    }
    move_to_dir[i] = 0;
    pwd.assign(home);
    top_bottom_bar(win_row, win_col);
    dir_list = ls_cmd(move_to_dir);
}

void onPressBack(vector <struct dirent *> &dir_list) {
    string dest_dir="..";
    if(!pwd.compare(home));
    else {
        string move_to = "";
        move_to.append(dest_dir);
        cout<<CLEAR;
        int i;
        modify_wd(move_to);
        int len = pwd.length();
        char move_to_dir[len];
        for(i=0;pwd[i]!=0;i++)
            move_to_dir[i]=pwd[i];
        move_to_dir[i]=0;
        top_bottom_bar(win_row, win_col);
        dir_list.clear();
        dir_list = ls_cmd(move_to_dir);
        dir_his.push_back(pwd);
        itr = dir_his.end();
        itr = prev(itr, 1);
    }
}

int main() {
    cout<<CLEAR;
    top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;home[i]!=0;i++)
        arr[i] = home[i];
    arr[i]=0;

    vector <struct dirent *> dir_list = ls_cmd(arr);
    dir_his.push_back(home);
    itr = dir_his.begin();
    char key_press;
    while(true) {
        key_press = check_keypress();
        if(key_press == -10)
            move_cursor(cur_row-1,0);
        else if(key_press == -11)
            move_cursor(cur_row+1,0);
        else if(key_press == -12)
            onPressRight(dir_list);
        else if(key_press == -13)
            onPressLeft(dir_list);
        else if(key_press == 27) {
            cout<<CLEAR;
            break;
        } else if(key_press == 10)
            onPressEnter(dir_list);
        else if(key_press == 127)
            onPressBack(dir_list);
        else if(key_press == 104) {
            onPressHome(dir_list);
        }
    }
    return 0;
}
