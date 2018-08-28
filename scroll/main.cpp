#include <iterator>
#include <vector>
#include "headers.h"
using namespace std;

vector <string> dir_his;
vector<string>::iterator itr;

int main() {
    top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;home[i]!=0;i++)
        arr[i] = home[i];
    arr[i]=0;

    vector <struct dirent *> dir_list = ls_cmd(arr);

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
                list_row--;
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
                move_cursor(cur_row+1, 0, false, "D");
            } else if(scroll_bit && list_row<list_size-1){
                list_row++;
                top_bottom_bar(win_row, win_col);
                start_index = list_row - windows_capacity + 1;
                end_index = list_row;
                display(dir_list, start_index, end_index, scroll_bit, "D");
            }
        }
        else if(key_press == -12);
            //onPressRight(dir_list);
        else if(key_press == -13);
            //onPressLeft(dir_list);
        else if(key_press == 27) {
            cout<<CLEAR;
            break;
        } else if(key_press == 10);
            //onPressEnter(dir_list);
        else if(key_press == 127);
            //onPressBack(dir_list);
        else if(key_press == 104) {
            //onPressHome(dir_list);
        }
    }

    return 0;
}
