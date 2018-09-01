#include "headers.h"

vector <string> dir_his;
vector<string>::iterator his_itr;

void onPressEnterN(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
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
                        his_itr = dir_his.end();
                        his_itr = prev(his_itr, 1);
                    } else {
                        copy(dir_list_copy.begin(), dir_list_copy.end(), back_inserter(dir_list));
                        dir_list_copy.clear();
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

void onPressUpN(vector <struct dirent *> &dir_list, bool scroll_bit) {
    if(cur_row > 6) {
        if(scroll_bit)
            list_row--;
        else
            list_row = cur_row-1-6;
        place_cursor(cur_row-1, 0);
    } else if(scroll_bit && list_row>0) {
        list_row--;
        top_bottom_bar(win_row, win_col);
        int start_index = list_row;
        int end_index = list_row + windows_capacity - 1;
        display(dir_list, start_index, end_index, scroll_bit, "U");
    }
}

void onPressDownN(vector <struct dirent *> &dir_list, bool scroll_bit, int &list_size) {
    if(cur_row < win_row-2) {
        if(!scroll_bit)
            move_cursor(cur_row+1, 0, false, "D");
        else {
            place_cursor(cur_row+1, 0);
            list_row++;
        }
    } else if(scroll_bit && list_row<list_size-1){
        list_row++;
        top_bottom_bar(win_row, win_col);
        int start_index = list_row - windows_capacity + 1;
        int end_index = list_row;
        display(dir_list, start_index, end_index, scroll_bit, "D");
    }
}

void onPressHomeN(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    dir_his.push_back(home);
    his_itr = dir_his.end();
    his_itr = prev(his_itr, 1);
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

void onPressBackN(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
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
            his_itr = dir_his.end();
            his_itr = prev(his_itr, 1);

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

void onPressRightN(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    vector<string>::iterator ftr = next(his_itr,1);
    if(ftr != dir_his.end()) {
        his_itr = next(his_itr, 1);
        dir_list.clear();
        string str = *his_itr;
        int len = str.length(), i;
        pwd.replace(pwd.begin(), pwd.end(), str);
        twd.replace(twd.begin(), twd.end(), str);
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

void onPressLeftN(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
    if(his_itr != dir_his.begin()) {
        his_itr = prev(his_itr, 1);
        dir_list.clear();
        string str = *his_itr;
        int len = str.length(), i;
        char move_to_dir[len];
        cout<<CLEAR;
        pwd.replace(pwd.begin(), pwd.end(), str);
        twd.replace(twd.begin(), twd.end(), str);
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

