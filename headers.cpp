#include "headers.h"

using namespace std;

int *ptr = get_row_col("end");
int win_col = ptr[1];
int win_row = ptr[0];
int cur_row = 0;
int cur_col = 0;
int list_row = 0;
int list_col = 0;

string pwd = get_cwd();
string home = get_cwd();
string twd = get_cwd();

string get_cwd() {
    char buf[BUFSIZ];
    if(!getcwd(buf, BUFSIZ)) {
        perror("An error occured");
    }
    return buf;
}

void move_cursor(int row, int col, bool scroll_flag, string action) {
    if(row>=0 && row<=win_row-2 && col<=win_col && col>=0 && !scroll_flag) {
        cout<<"\033["<<row<<";"<<col<<"H";
        cur_row = row;
        cur_col = col;
        if(!action.compare("D"))
            list_row = cur_row-6;
    } else if(scroll_flag) {
        cout<<"\033["<<row<<";"<<col<<"H";
        cur_row = row;
        cur_col = col;
    }
    return;
}

int* get_row_col(string str) {

    int *ptr = (int *)malloc(sizeof(int)*2);

    if(!str.compare("end")) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        ptr[0] = w.ws_row;
        ptr[1] = w.ws_col;
    }

    return ptr;
}

void top_bottom_bar(int row, int col) {
    cout<<CLEAR;
    string str = "...Welcome to the file explorer...";
    int len = str.length();
    int start_col = col/2 - len/2;
    cout<<"\033["<<row-1<<";1H";
    cout<<GREEN<<"cwd: "<<WHITE<<pwd;
    cout<<"\033[1;"<<start_col<<"H";
    cout<<GREEN<<str;
    cout<<"\033[2;1H";
    for(int i=0;i<col;i++)
        cout<<".";
    cout<<"\033[3;0H";
    cur_row = 3;
}

string shrink_str(string str) {
    string dir_name;
    int length = str.length();
    if(length<=16) {
        return str;
    }
    else {
        dir_name.append(str, 0, 6);
        dir_name.append("...");
        dir_name.append(str, length-7,6);
    }

    return dir_name;
}

char check_keypress() {
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ECHO|ICANON);
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_settings);


    char ch = 0,ch_test[3] = {0,0,0};
    ch_test[0] = getchar();
    if(ch_test[0] == 27) {
        ch_test[1] = getchar();
        if (ch_test[1] == 91) {
            ch_test[2] = getchar();
            if(ch_test[2] == 'A')
                ch = -10;
            else if(ch_test[2] == 'B')
                ch = -11;
            else if(ch_test[2] == 'C')
                ch = -12;
            else if(ch_test[2] == 'D')
                ch = -13;
        } else
            ch = ch_test [1];
    } else
        ch = ch_test [0];

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    return ch;
}

void modify_wd(string dest_dir) {
    int twd_len = twd.length();
    if(!dest_dir.compare("..")) {
        while(true) {
            if(twd[--twd_len] != '/')
                twd.pop_back();
            else {
                twd.pop_back();
                break;
            }
        }
    } else {
        twd.append("/");
        twd.append(dest_dir);
    }
}

void place_cursor(int row, int col) {
    if(row>=0 && row<=win_row-2 && col<=win_col && col>=0) {
        cout<<"\033["<<row<<";"<<col<<"H";
        cur_row = row;
        cur_col = col;
    }
}

void display(vector <struct dirent *> &dir_list, int start_index, int end_index, bool scroll_status, string flag) {
    string type;
    float file_size;
    char file_per[11], ch;
    int elements = dir_list.size();
    struct dirent * dir_element = dir_list[start_index];

    if(dir_element) {
        cout<<"List of files and directories in: "<<WHITE<<pwd;
        move_cursor(5, 0, scroll_status, flag);
        move_cursor(cur_row, 6, scroll_status, flag);
        cout<<LCYAN<<"NAME";
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<"FILE TYPE";
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<"INODE NUMBER";
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<"FILE PERMISSIONS";
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<"FILE SIZE";
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<"LAST MODIFIED";
        move_cursor(cur_row+1, 0, scroll_status, flag);
    }


    while(start_index <= end_index) {
        struct stat fileStat;
        dir_element = dir_list[start_index];
        if(dir_element->d_type == DT_REG) {
            cout<<WHITE;
            type = "regular file";
        } else if(dir_element->d_type == DT_DIR) {
            cout<<YELLOW;
            type = "directory";
        } else if(dir_element->d_type == DT_SOCK) {
            cout<<WHITE;
            type = "socket";
        } else if(dir_element->d_type == DT_LNK) {
            cout<<GREEN;
            type = "symlink";
        } else {
            cout<<WHITE;
            type = "unknown";
        }

        char absolute_path[2048];
        int k, l=0;
        for(k=0;pwd[k]!=0;k++)
            absolute_path[k] = pwd[k];
        absolute_path[k++] = '/';
        while(dir_element->d_name[l]!=0) {
            absolute_path[k] = dir_element->d_name[l];
            k++;
            l++;
        }
        absolute_path[k] = 0;

        string dir_name = shrink_str(dir_element->d_name);

        stat(absolute_path,&fileStat);
        file_size = fileStat.st_size;
        string readable_size;
        if(file_size >= 1024*1024*1024) {
            file_size = file_size/(1024*1024*1024);
            readable_size.append(to_string(file_size));
            readable_size.append("GB");
        } else if(file_size >= 1024*1024) {
            file_size = file_size/(1024*1024);
            readable_size.append(to_string(file_size));
            readable_size.append("MB");
        } else if(file_size >= 1024) {
            file_size = file_size/1024;
            readable_size.append(to_string(file_size));
            readable_size.append("KB");
        } else {
            readable_size.append(to_string(file_size));
            readable_size.append("B");
        }

        if (fileStat.st_mode & S_IRUSR)
            file_per[0] = 'r';
        else
            file_per[0] = '-';
        if (fileStat.st_mode & S_IWUSR)
            file_per[1] = 'w';
        else
            file_per[1] = '-';
        if (fileStat.st_mode & S_IXUSR)
            file_per[2] = 'x';
        else
            file_per[2] = '-';
        if (fileStat.st_mode & S_IRGRP)
            file_per[3] = 'r';
        else
            file_per[3] = '-';
        if (fileStat.st_mode & S_IWGRP)
            file_per[4] = 'w';
        else
            file_per[4] = '-';
        if (fileStat.st_mode & S_IXGRP)
            file_per[5] = 'x';
        else
            file_per[5] = '-';
        if (fileStat.st_mode & S_IROTH)
            file_per[6] = 'r';
        else
            file_per[6] = '-';
        if (fileStat.st_mode & S_IWOTH)
            file_per[7] = 'w';
        else
            file_per[7] = '-';
        if (fileStat.st_mode & S_IXOTH)
            file_per[8] = 'x';
        else
            file_per[8] = '-';
        file_per[9] = 0;

        cout<<">>";
        move_cursor(cur_row, 6, scroll_status, flag);
        cout<<dir_name;
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<type;
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<dir_element->d_ino;
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<file_per;
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<readable_size;
        move_cursor(cur_row, cur_col+20, scroll_status, flag);
        cout<<fileStat.st_mtime;

        if(start_index != end_index)
            move_cursor(cur_row+1, 0, scroll_status, flag);
        else if(start_index == end_index && !flag.compare("U"))
            place_cursor(6, 0);
        else if(!flag.compare("D")) {
            move_cursor(cur_row, 0, scroll_status, flag);
            if(elements < win_row - 7)
                list_row = cur_row-6;
        }
        start_index++;
    }
}


vector <struct dirent *> ls_cmd(char *dir_path) {
    DIR *dir_ptr;
    struct dirent *dir_element;

    dir_ptr = opendir(dir_path);

    vector <struct dirent *> directories;

    if(!dir_ptr) {
        return directories;
    }

    dir_element = readdir(dir_ptr);

    while(dir_element) {
        directories.push_back(dir_element);
        dir_element = readdir(dir_ptr);
    }

    closedir(dir_ptr);
    return directories;
}
