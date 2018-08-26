
#include<iostream>
#include<sys/stat.h>
#include <ctime>
#include <math.h>
#include <dirent.h>

using namespace std;

void ls_cmd(char *dir_path) {
    DIR *dir_ptr;
    struct dirent *dir_element;
    struct stat fileStat;

    dir_ptr = opendir(dir_path);

    if(!dir_ptr) {
        perror(dir_path);
        return;
    }

    dir_element = readdir(dir_ptr);

    while(dir_element) {
        string type;
        unsigned int file_size;
        char file_per[11], ch;

        if(dir_element->d_type == DT_REG) {
            type = "regular file";
        } else if(dir_element->d_type == DT_DIR) {
            type = "directory";
        } else if(dir_element->d_type == DT_SOCK) {
            type = "socket";
        } else if(dir_element->d_type == DT_LNK) {
            type = "symlink";
        } else {
            type = "unknown";
        }

        string strr;
        strr.append(dir_path);
        strr.append("/");
        strr.append(dir_element->d_name);
        char ptr[1024];
        int k;
        for(k=0;strr[k]!=0;k++)
            ptr[k] = strr[k];
        ptr[k]=0;

        stat(ptr, &fileStat);
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

        cout<<">>"<<"\t"<<dir_element->d_name<<"\t"<<type<<"\t"<<dir_element->d_ino<<"\t"<<file_per<<"\t"<<file_size;
        time_t modified_time = fileStat.st_mtime;
        tm *ltm = localtime(&modified_time);
        int yr = 1900+ltm->tm_year;
        int month = 1+ltm->tm_mon;
        int day = ltm->tm_mday;
        int hr = ltm->tm_hour;
        int minute = ltm->tm_min;
        int sec = ltm->tm_sec;
        string file_time = to_string(day) + "/" + to_string(month) + "/" + to_string(yr) + " " + to_string(hr) + ":" + to_string(minute) + ":" + to_string(sec);
        cout<<"\t"<<file_time<<endl;

        dir_element = readdir(dir_ptr);
    }

    closedir(dir_ptr);
}


int main() {
    ls_cmd("/home/aman/Desktop/git/fileexplorer/.git");
    return 0;
}
