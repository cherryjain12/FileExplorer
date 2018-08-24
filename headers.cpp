#include "headers.h"

using namespace std;

void nl() {
    cout<<endl;
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
    cout<<"\033["<<row<<";"<<col<<"H";
    cur_row = row;
    cur_col = col;
    return;
}

int* get_row_col(string str) {

    int *ptr = (int *)malloc(sizeof(int)*2);

    if(!str.compare("end")) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        ptr[0] = w.ws_row;
        ptr[1] = w.ws_col;
    } else if(!str.compare("cursor")) {

    }

    return ptr;
}

char *shrink_str(string str) {
    char *dir_name = (char *)malloc(sizeof(char)*17);
    int length = str.length();
    if(length<=16) {
        for(int i=0;i<length;i++)
            dir_name[i] = str[i];
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

    if(ch_test[0] == 27)
    {
        ch_test[1] = getchar();

        if (ch_test[1] == 91)
        {
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

void ls_cmd(char *dir_path) {
    DIR *dir_ptr;
    struct dirent *dir_element;

    dir_ptr = opendir(dir_path);

    if(!dir_ptr) {
        cout<<"No such file or directory exist!";
        return;
    }

    dir_element = readdir(dir_ptr);

    if(dir_element) {
        cout<<"List of files and directories in: "<<WHITE<<get_cwd()<<"/"<<dir_path;nl();nl();
        cout<<LCYAN<<"FILE NAME\t\tFILE TYPE\t\tFILE SIZE";nl();
    }
    while(dir_element) {
        string type;
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
        cout<<dir_name<<"\t\t"<<type<<"\t\t"<<dir_element->d_reclen;nl();
        dir_element = readdir(dir_ptr);
    }

    closedir(dir_ptr);

}

int *ptr = get_row_col("end");
    int win_col = ptr[1];
    int win_row = ptr[0];
    int cur_row = ptr[0];
    int cur_col = ptr[1];
