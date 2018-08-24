/*
#include <iostream>
#include <unistd.h>
using namespace std;
void print_progress_bar(int percentage){
  string progress = "[" + string(percentage, '*') + string(100 - percentage, ' ') + "]";
  cout << progress << "\r\033[F\033[F\033[F" << flush;
}

int main(void)
{
        cout << endl;
          for (int i=0; i <= 100; ++i){
            print_progress_bar(i);
            sleep(1);
          }
          cout << endl;
          cout << endl;
          cout << endl;
          cout << endl;
        /*
        std::cout << "\033[2J\033[1;1H";
        for (int i = 0; i < 10; i++) {
                std::cout << "Status: " << i << "\r" << std::flush;
                sleep(1);
        }
        std::cout << "Completed.\n";
        */
//}
/*

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

void show_progress_bar(std::ostream& os, int time,
                       std::string message, char symbol = '*') {
    static const auto bar_length = 70;
    // not including the percentage figure and spaces

    if (message.length() >= bar_length) {
        os << message << '\n';
        message.clear();
    } else {
        message += " ";
    }

    const auto progress_level = 100.0 / (bar_length - message.length());

    std::cout << message;

    for (double percentage = 0; percentage <= 100; percentage += progress_level) {
        message += symbol;
        os << "\r [" << std::setw(3) << static_cast<int>(percentage) << "%] "
           << message << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    os << "\n\n";
}


int main() {
    std::cout<<"\033[2J";
    std::cout<<"\033[6;3HHello\n";
    std::cout << "\033[1;1H";
    std::cout<<"Hello there";
    //show_progress_bar(std::clog, 100, "progress", '#');
    return 0;
}
*/

       #include <dirent.h>     /* Defines DT_* constants */
       #include <fcntl.h>
       #include <stdio.h>
       #include <unistd.h>
       #include <stdlib.h>
       #include <sys/stat.h>
       #include <sys/syscall.h>

       #define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

       struct linux_dirent {
           long           d_ino;
           off_t          d_off;
           unsigned short d_reclen;
           char           d_name[];
       };

       #define BUF_SIZE 1024

       int
       main(int argc, char *argv[])
       {
           int fd, nread;
           char buf[BUF_SIZE];
           struct linux_dirent *d;
           int bpos;
           char d_type;

           fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
           if (fd == -1)
               handle_error("open");

           for ( ; ; ) {
               nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
               if (nread == -1)
                   handle_error("getdents");

               if (nread == 0)
                   break;

               printf("--------------- nread=%d ---------------\n", nread);
               printf("inode#    file type  d_reclen  d_off   d_name\n");
               for (bpos = 0; bpos < nread;) {
                   d = (struct linux_dirent *) (buf + bpos);
                   printf("%8ld  ", d->d_ino);
                   d_type = *(buf + bpos + d->d_reclen - 1);
                   printf("%-10s ", (d_type == DT_REG) ?  "regular" :
                                    (d_type == DT_DIR) ?  "directory" :
                                    (d_type == DT_FIFO) ? "FIFO" :
                                    (d_type == DT_SOCK) ? "socket" :
                                    (d_type == DT_LNK) ?  "symlink" :
                                    (d_type == DT_BLK) ?  "block dev" :
                                    (d_type == DT_CHR) ?  "char dev" : "???");
                   printf("%4d %10lld  %s\n", d->d_reclen,
                           (long long) d->d_off, d->d_name);
                   bpos += d->d_reclen;
               }
           }

           exit(EXIT_SUCCESS);
       }
