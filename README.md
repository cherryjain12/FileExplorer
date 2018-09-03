Following are the important points regarding the project.

1: The application must only be opened in full screen else the layout of the application will get disturbed.

2: All the mandatory functionalities like create_dir, create_file, search, goto, delete_file, delete_dir, snapshot,copy, move and rename are well implemented except the recycle bin.

3: As per the normal mode scroll feature is working fine and user can navigate using the four arrow keys, backspace, enter and home key.

4: Files are opening on pressing enter using the traditional "xdg-open" method.

5: User can enter into command mode on pressing enter and back to normal mode on pressing ESC key.

6: Below is the syntax and assumptions for various commands implemented:
    
    (a): Path having space in between must be enclosed with inverted commas. ""
            For Eg: goto "Path with space"
    
    (b): Any any extra space in between the command is considered as a syntax error.
    
    (c): Tough recursive delete is implemented but user can delete only one directory/file at a time whether it be empty or filled.
    
    (d): Similar is the case with creating a directory or a file, (i.e. only one file/directory will be created per command).
    
    (e): Unlike creating or deleting a file or a directory on one go, user can move/copy multiple files/directories at a time.
    
    (f): User can provide both absolute path wrt the HOME(directory where the application is started) or relative path wrt the present working directory of the user. 
    
    (g): If the user is providing the absolute path wrt HOME then the path must start with a back slash '/'.
    
    (f): If the user is providing the relative path wrt current working directory then the path must not start with back slash '/'.
    
    (g):    Syntax for
    
            GOTO: goto <directory_name>
                Eg: goto "destination directory"            goto destination_directory
            
            CREATE FILE: create_file <file_name> <destination>
                Eg: create_file "sample.txt" destination_directory           create_file file_name .
                
            CREATE DIRECTORY: create_dir <directory_name> <destination>
                Eg: create_dir "directory name" destination_directory           create_dir directory_name destination
            
            DELETE_FILE: delete_file <file_path>
                Eg: delete_file "file path"            delete_file file_path
            
            DELETE_DIR: delete_dir <dir_path>
                Eg: delete_dir "dir path"           delete_dir dir_path
                
            COPY: copy <filename/(s)> <destination directory>               copy <directory name/(s)> <destination directory>
                Eg: copy file1.txt file2.txt /home/git/destination              copy /home/source /home/git/destination
            
            MOVE: move <filename/(s)> <destination directory>               move <directory name/(s)> <destination directory>
                Eg: move file1.txt file2.txt /home/git/destination              move /home/source /home/git/destination
                
            SNAPSHOT: snapshot <directory path> <filename>
                Eg: snapshot .git sample.txt
                
            SEARCH: search <keyword>
                Eg: search sample
                
            RENAME: rename <old dir/file name> <new dir/file name>
                Eg: rename xyz.txt abc.txt                          rename dir1 dir2
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
