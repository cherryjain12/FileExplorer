#include "headers.h"

using namespace std;

int *ptr = get_row_col("end");
int win_col = ptr[1];
int win_row = ptr[0];
int cur_row = ptr[0];
int cur_col = ptr[1];

string pwd = get_cwd();
string home = get_cwd();

void nl() {
    cout<<endl;
    if(cur_row >= 0 && cur_row<=win_row)
        cur_row+=1;
    cur_col = 0;
    return;
}

string get_cwd() {
    char buf[BUFSIZ];
    if(!getcwd(buf, BUFSIZ)) {
        perror("An error occured");
    }
    return buf;
}

void move_cursor(int row, int col) {

    if(row>=0 && row<=win_row && col<=win_col && col>=0) {
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

char *shrink_str(string str) {
    char *dir_name = (char *)malloc(sizeof(char)*17);
    int length = str.length();
    if(length<=16) {
        int i;
        for(i=0;i<length;i++)
            dir_name[i] = str[i];
        dir_name[i] = 0;
        return dir_name;
    }
    else {
        int i,j;
        for(i=0,j=0;i<17;i++) {
            if(i<7)
                dir_name[i] = str[j];
            else if(i>7 && i<=10)
                dir_name[i] = '.';
            else {
                if(j==11)
                    j=length-6;
                dir_name[i] = str[j];
            }
            j++;
        }
        dir_name[i] = 0;
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
    int pwd_len = pwd.length();
    if(!dest_dir.compare("..")) {
        while(true) {
            if(pwd[--pwd_len] != '/')
                pwd.pop_back();
            else {
                pwd.pop_back();
                break;
            }
        }
    } else {
        pwd.append("/");
        pwd.append(dest_dir);
    }
}

vector <struct dirent *> ls_cmd(char *dir_path) {
    DIR *dir_ptr;
    struct dirent *dir_element;
    struct stat fileStat;

    dir_ptr = opendir(dir_path);

    vector <struct dirent *> directories;

    if(!dir_ptr) {
        perror(dir_path);
        return directories;
    }

    dir_element = readdir(dir_ptr);

    if(dir_element) {
        cout<<"List of files and directories in: "<<WHITE<<pwd;nl();nl();
        move_cursor(cur_row, 6);
        cout<<LCYAN<<"NAME";
        move_cursor(cur_row, cur_col+20);
        cout<<"FILE TYPE";
        move_cursor(cur_row, cur_col+20);
        cout<<"INODE NUMBER";
        move_cursor(cur_row, cur_col+20);
        cout<<"FILE PERMISSIONS";
        move_cursor(cur_row, cur_col+20);
        cout<<"FILE SIZE";
        move_cursor(cur_row, cur_col+20);
        cout<<"LAST MODIFIED";nl();
    }
    while(dir_element) {
        string type;
        unsigned int file_size;
        char file_per[11], ch;
        char *dir_name = shrink_str(dir_element->d_name);

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

        stat(absolute_path,&fileStat);
        file_size = fileStat.st_size;

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

        directories.push_back(dir_element);
        cout<<">>";
        move_cursor(cur_row, 6);
        cout<<dir_name;
        move_cursor(cur_row, cur_col+20);
        cout<<type;
        move_cursor(cur_row, cur_col+20);
        cout<<dir_element->d_ino;
        move_cursor(cur_row, cur_col+20);
        cout<<file_per;
        move_cursor(cur_row, cur_col+20);
        cout<<file_size;
        move_cursor(cur_row, cur_col+20);
        time_t modified_time = fileStat.st_mtime;
        tm *ltm = localtime(&modified_time);
        int yr = 1900+ltm->tm_year;
        int month = 1+ltm->tm_mon;
        int day = ltm->tm_mday;
        int hr = ltm->tm_hour;
        int minute = ltm->tm_min;
        int sec = ltm->tm_sec;
        string file_time = to_string(day) + "/" + to_string(month) + "/" + to_string(yr) + " " + to_string(hr) + ":" + to_string(minute) + ":" + to_string(sec);
        cout<<file_time;nl();

        dir_element = readdir(dir_ptr);
    }

    closedir(dir_ptr);
    return directories;
}
