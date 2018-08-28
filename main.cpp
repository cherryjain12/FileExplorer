#include <iterator>
#include <vector>
#include "headers.h"
using namespace std;

vector <string> dir_his;
vector<string>::iterator itr;

void onPressEnter(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    if(list_row<list_size) {
        char *move_to_dir = dir_list[list_row]->d_name;
        if(!pwd.compare(home) && (move_to_dir[0]=='.' && move_to_dir[1]=='.' && move_to_dir[2]==0));
        else {
            if(dir_list[list_row]->d_type == DT_DIR) {
                string move_to = "";
                move_to.append(move_to_dir);
                if(move_to.compare(".")) {
                    int i;
                    modify_wd(move_to);
                    for(i=0;twd[i]!=0;i++)
                        move_to_dir[i]=twd[i];
                    move_to_dir[i]=0;

                    vector <struct dirent *> dir_list_copy(dir_list.begin(), dir_list.end());

                    dir_list.clear();
                    dir_list = ls_cmd(move_to_dir);
                    if(!dir_list.empty()) {
                        pwd.assign(twd);
                        top_bottom_bar(win_row, win_col);
                        list_size = dir_list.size();
                        int windows_capacity = win_row - 7;
                        if(list_size>windows_capacity) {
                            display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
                            scroll_bit = true;
                        } else {
                            display(dir_list, 0, list_size-1, scroll_bit, "D");
                            scroll_bit = false;
                        }
                        dir_his.push_back(pwd);
                        itr = dir_his.end();
                        itr = prev(itr, 1);
                    } else {
                        copy(dir_list_copy.begin(), dir_list_copy.end(), back_inserter(dir_list));
                        twd.assign(pwd);
                    }
                }
            } else if(dir_list[list_row]->d_type == DT_REG) {
                char file[1000];
                strcpy(file, "xdg-open ");
                char ptr_pwd[1000];
                int p;
                for(p=0;pwd[p]!=0;p++)
                    ptr_pwd[p]=pwd[p];
                ptr_pwd[p]=0;
                strcat(file, ptr_pwd);
                strcat(file, "/");
                strcat(file, move_to_dir);
                strcat(file, " > log.txt");
                system(file);
            }
        }
    }
}

void onPressHome(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    dir_his.push_back(home);
    itr = dir_his.end();
    itr = prev(itr, 1);
    dir_list.clear();

    int len = home.length(), i;
    char move_to_dir[len];
    for(i=0;i<len;i++) {
        move_to_dir[i] = home[i];
    }
    move_to_dir[i] = 0;
    pwd.assign(home);
    top_bottom_bar(win_row, win_col);
    dir_list = ls_cmd(move_to_dir);

    list_size = dir_list.size();
    int windows_capacity = win_row - 7;
    if(list_size>windows_capacity) {
        display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
        scroll_bit = true;
    } else {
        display(dir_list, 0, list_size-1, scroll_bit, "D");
        scroll_bit = false;
    }

}

void onPressBack(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    string dest_dir="..";
    if(!pwd.compare(home));
    else {
        string move_to = "";
        move_to.append(dest_dir);
        cout<<CLEAR;
        int i;
        modify_wd(move_to);
        int len = twd.length();
        char move_to_dir[len];
        for(i=0;twd[i]!=0;i++)
            move_to_dir[i]=twd[i];
        move_to_dir[i]=0;

        vector <struct dirent *> dir_list_copy(dir_list.begin(), dir_list.end());
        dir_list.clear();
        dir_list = ls_cmd(move_to_dir);

        if(!dir_list.empty()) {
            pwd.assign(twd);
            top_bottom_bar(win_row, win_col);
            dir_his.push_back(pwd);
            itr = dir_his.end();
            itr = prev(itr, 1);

            list_size = dir_list.size();
            int windows_capacity = win_row - 7;
            if(list_size>windows_capacity) {
                display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
                scroll_bit = true;
            } else {
                display(dir_list, 0, list_size-1, scroll_bit, "D");
                scroll_bit = false;
            }
        } else {
            copy(dir_list_copy.begin(), dir_list_copy.end(), back_inserter(dir_list));
            twd.assign(pwd);
        }
    }
}

void onPressRight(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    vector<string>::iterator ftr = next(itr,1);
    if(ftr != dir_his.end()) {
        itr = next(itr, 1);
        dir_list.clear();
        string str = *itr;
        int len = str.length(), i;
        pwd.replace(pwd.begin(), pwd.end(), str);
        top_bottom_bar(win_row, win_col);
        char move_to_dir[len];
        for(i=0;i<len;i++) {
            move_to_dir[i] = str[i];
        }
        move_to_dir[i] = 0;
        dir_list = ls_cmd(move_to_dir);
        list_size = dir_list.size();
        int windows_capacity = win_row - 7;
        if(list_size>windows_capacity) {
            display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
            scroll_bit = true;
        } else {
            display(dir_list, 0, list_size-1, scroll_bit, "D");
            scroll_bit = false;
        }
    }
}

void onPressLeft(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
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

        list_size = dir_list.size();
        int windows_capacity = win_row - 7;
        if(list_size>windows_capacity) {
            display(dir_list, 0, windows_capacity-1, false, "D");
            scroll_bit = true;
        } else {
            display(dir_list, 0, list_size-1, scroll_bit, "D");
            scroll_bit = false;
        }
    }
}

int main() {
    top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;home[i]!=0;i++)
        arr[i] = home[i];
    arr[i]=0;

    vector <struct dirent *> dir_list = ls_cmd(arr);
    dir_his.push_back(home);
    itr = dir_his.begin();

    bool scroll_bit = false;
    int list_size = dir_list.size();
    int windows_capacity = win_row - 7;
    if(list_size>windows_capacity) {
        display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
        scroll_bit = true;
    } else {
        display(dir_list, 0, list_size-1, scroll_bit, "D");
    }

    int start_index, end_index;

    char key_press;

    while(true) {
        key_press = check_keypress();
        if(key_press == -10) {
            if(cur_row > 6) {
                if(scroll_bit)
                    list_row--;
                else
                    list_row = cur_row-1-6;
                place_cursor(cur_row-1, 0);
            }
            else if(scroll_bit && list_row>0) {
                list_row--;
                top_bottom_bar(win_row, win_col);
                start_index = list_row;
                end_index = list_row + windows_capacity - 1;
                display(dir_list, start_index, end_index, scroll_bit, "U");
            }
        } else if(key_press == -11) {
            if(cur_row < win_row-2) {
                if(!scroll_bit)
                    move_cursor(cur_row+1, 0, false, "D");
                else {
                    place_cursor(cur_row+1, 0);
                    list_row++;
                }
                cout<<list_row<<":"<<scroll_bit;
            } else if(scroll_bit && list_row<list_size-1){
                list_row++;
                top_bottom_bar(win_row, win_col);
                start_index = list_row - windows_capacity + 1;
                end_index = list_row;
                display(dir_list, start_index, end_index, scroll_bit, "D");
            }
        }
        else if(key_press == -12)
            onPressRight(dir_list, scroll_bit, list_size);
        else if(key_press == -13)
            onPressLeft(dir_list, scroll_bit, list_size);
        else if(key_press == 27) {
            cout<<CLEAR;
            break;
        } else if(key_press == 10) {
            onPressEnter(dir_list, scroll_bit, list_size);
        }
        else if(key_press == 127) {
            onPressBack(dir_list, scroll_bit, list_size);
        }
        else if(key_press == 104) {
            onPressHome(dir_list, scroll_bit, list_size);
        }
    }

    return 0;
}
