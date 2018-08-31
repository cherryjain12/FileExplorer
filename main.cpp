#include "headers.h"

using namespace std;

int main() {
    init();
    top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;home[i]!=0;i++)
        arr[i] = home[i];
    arr[i]=0;

    vector <struct dirent *> dir_list = ls_cmd(arr);
    dir_his.push_back(home);
    his_itr = dir_his.begin();

    bool scroll_bit = false;
    int list_size = dir_list.size();
    if(list_size>windows_capacity) {
        display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
        scroll_bit = true;
    } else {
        display(dir_list, 0, list_size-1, scroll_bit, "D");
    }

    char key_press;

    while(true) {
        key_press = check_keypress();
        if(!MODE.compare("NORMAL")) {
            if(key_press == -10) {
                onPressUpN(dir_list, scroll_bit);
            } else if(key_press == -11) {
                onPressDownN(dir_list, scroll_bit, list_size);
            } else if(key_press == -12) {
                onPressRightN(dir_list, scroll_bit, list_size);
            } else if(key_press == -13) {
                onPressLeftN(dir_list, scroll_bit, list_size);
            } else if(key_press == 10) {
                onPressEnterN(dir_list, scroll_bit, list_size);
            } else if(key_press == 127) {
                onPressBackN(dir_list, scroll_bit, list_size);    
            } else if(key_press == 104) {
                onPressHomeN(dir_list, scroll_bit, list_size);
            } else if(key_press == 58) {
                switch_mode(dir_list, list_size, scroll_bit);
            }
        } else if(!MODE.compare("CMD")) {
            if(key_press == 27) {
                switch_mode(dir_list, list_size, scroll_bit);
            } else if(key_press == 10) {
                onPressEnterC(dir_list, scroll_bit, list_size);
            } else if(key_press == 127) {
                onPressBackC();
            } else if(key_press!=-11 && key_press!=-12 && key_press!=-13 && key_press!=-10){
                storeCmd(key_press);
            } 
        }
    }

    return 0;
}
