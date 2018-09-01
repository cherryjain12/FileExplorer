#include "headers.h"

string cmd;

int cmd_cur = 0;

vector<string> list_cmds;

void init(){
	list_cmds.push_back("copy");
	list_cmds.push_back("create_file");
	list_cmds.push_back("move");
	list_cmds.push_back("rename");
	list_cmds.push_back("create_dir");
	list_cmds.push_back("delete_file");
	list_cmds.push_back("goto");
	list_cmds.push_back("search");
	list_cmds.push_back("snapshot");
	list_cmds.push_back("delete_dir");
}

void onPressBackC() {
	int length = cmd.size();
	if(length > 0) {
		cmd.pop_back();
		cmd_cur--;
		cout<<"\033["<<win_row<<";"<<cmd_cur<<"H"<<" "<<"\033["<<win_row<<";"<<cmd_cur<<"H";
	}
}

vector <string> processCmd(int i, string input_str) {
	vector <string> arguments;
	string arg;
	int leave=0;
	for(int j=i+1;input_str[j]!=0;j++) {
		if(input_str[j]=='"' && input_str[j-1]==' ')
			leave = 1;
		else if(leave==1) {
			if(input_str[j]!='"')
				arg.push_back(input_str[j]);
			else {
				arguments.push_back(arg);
				arg.clear();
				leave = 0;
			}
		} else if(leave==0) {
			if(input_str[j]!=' ') {
				arg.push_back(input_str[j]);
			} else {
				arguments.push_back(arg);
				arg.clear();
			}
		}
	}
	arguments.push_back(arg);
	return arguments;
}

char *generate_abs_path(string path) {
	char *move_to_dir = (char *)malloc(sizeof(char)*500);
	int i=0, j=0; 
	if(path[0]=='/') {
		for(i=0;home[i]!=0;i++)
			move_to_dir[i]=home[i];
		while(path[j]!=0) {
			move_to_dir[i++]=path[j];
			j++;
		}
	} else {
		for(i=0;pwd[i]!=0;i++)
			move_to_dir[i]=pwd[i];
		move_to_dir[i++] = '/';
		while(path[j]!=0) {
			move_to_dir[i++]=path[j];
			j++;
		}
	}
	move_to_dir[i]=0;
	return move_to_dir;
}

void refresh(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
	top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;pwd[i]!=0;i++)
        arr[i] = pwd[i];
    arr[i]=0;

    dir_list.clear();

 	dir_list = ls_cmd(arr);
    
    scroll_bit = false;
    list_size = dir_list.size();

    if(list_size>windows_capacity) {
        display(dir_list, 0, windows_capacity-1, scroll_bit, "D");
        scroll_bit = true;
    } else {
        display(dir_list, 0, list_size-1, scroll_bit, "D");
    }
}

void onPressEnterC(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
	for(int i=16;i<win_col;i++)
		cout<<"\033["<<win_row<<";"<<i<<"H"<<" ";
	for(int i=50;i<win_col;i++)
		cout<<"\033["<<win_row-1<<";"<<i<<"H"<<" ";
	cout<<"\033["<<win_row<<";16H";
	string input_str;
	char input_cmd[20];
	input_str.assign(cmd);
	cmd.clear();
	int i=0;
	while(input_str[i]!=0 && input_str[i]!=' ') {
		input_cmd[i] = input_str[i];
		i++;
	}
	input_cmd[i]=0;
	string cmd_input;
	cmd_input.append(input_cmd);

	vector <string>::iterator itr = find(list_cmds.begin(), list_cmds.end(), cmd_input);
	
	if(itr != list_cmds.end()) {
		vector <string> arguments = processCmd(i, input_str);
		if(!(*itr).compare("copy")) {

		} else if(!(*itr).compare("create_dir")) {
			int arg_len = arguments.size();
			string error;
			if(arg_len == 2) {
				string folder_name = arguments[0];
				string path = arguments[1];
				char *dest;
				if(!path.compare("."))
					dest = generate_abs_path("");
				else
					dest = generate_abs_path(path);
				int p=0;
				while(dest[p]!=0) 
					p++;
				if(path.compare("."))
					dest[p++]='/';
				for(int q=0;folder_name[q]!=0;p++,q++) {
					dest[p] = folder_name[q];
				}
				dest[p]=0;
				
				const int dir_err = mkdir(dest, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				if (-1 == dir_err) {
					error.append("Unable to locate the directory: ");
					error.append(path);
					error.append(" or folder already exist");
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {
					refresh(dir_list, scroll_bit, list_size);
				}
				
			}else {
				error.append("Invalid create_dir command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}

		} else if(!(*itr).compare("create_file")) {
			int arg_len = arguments.size();
			string error;
			if(arg_len == 2) {
				string file_name = arguments[0];
				string path = arguments[1];
				char *dest;
				if(!path.compare("."))
					dest = generate_abs_path("");
				else
					dest = generate_abs_path(path);
				int p=0;
				while(dest[p]!=0) 
					p++;
				if(path.compare("."))
					dest[p++]='/';
				for(int q=0;file_name[q]!=0;p++,q++) {
					dest[p] = file_name[q];
				}
				dest[p]=0;
				
				int fd = open(dest, O_CREAT | O_WRONLY, 0644);

				if (fd == -1) {
					error.append("Unable to create the file: ");
					error.append(file_name);
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {
					refresh(dir_list, scroll_bit, list_size);
				}
				
			}else {
				error.append("Invalid create_file command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}
		} else if(!(*itr).compare("delete_file")) {
			int arg_len = arguments.size();
			string error;
			if(arg_len==1) {
				string file_path = arguments[0];
				char *dest = generate_abs_path(file_path);
				if (unlink(dest) < 0) {
					error.append("Unable to delete the file: ");
					error.append(file_path);
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {
					refresh(dir_list, scroll_bit, list_size);
				}
			} else {
				error.append("Invalid delete_file command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}
		} else if(!(*itr).compare("goto")) {
			if(arguments.size()==1) {
				string path;
				path.assign(arguments[0]);
				if(!path.compare("/")) {
					onPressHomeN(dir_list, scroll_bit, list_size);
				} else {	
					char *move_to_dir = generate_abs_path(path);
				
					int p=0;
					while(move_to_dir[p]!=0)
						p++;
					int count=0;
					if(!path.compare("..")) {
				        while(true) {
				            if(move_to_dir[--p] != '/')
				                move_to_dir[p]=' ';
				            else if(count<1){
				            	count++;
				                move_to_dir[p]=' ';
				            } else if(count == 1) {
				            	move_to_dir[p] = 0;
				            	break;
				            } 
				        }
				    } 
					
					twd.assign(move_to_dir);

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
			} else {
				string error = "invalid goto command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("snapshot")) {
			if(arguments.size()==2) {
				string folder = arguments[0];
				string dfile = arguments[1];
				char *dump_folder = generate_abs_path(folder);
				char *dump_file = generate_abs_path(dfile);
				generate_snapshot(dump_folder, dump_file);
			} else {
				string error = "invalid snapshot command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("search")) {
			if(arguments.size()==1) {
				string dir = pwd;
				string val = arguments[0];
				vector <string> results = search(dir, val);
				int search_size = results.size();
				if(search_size == 0) {
					string error = "No results found";
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
				} else {
						







				}
			} else {
				string error = "invalid search command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("rename")) {
			if(arguments.size()==2) {
				string pre, succ, error;
				pre.assign(arguments[0]);
				succ.assign(arguments[1]);
				char *present = generate_abs_path(pre);
				char future[100];
				int i,j=0;
				while(present[j]!=0)
					j++;
				while(present[j]!='/')
					j--;
				for(i=0;i<=j;i++)
					future[i] = present[i];
				for(int k=0;succ[k]!=0;k++,i++)
					future[i] = succ[k];
				future[i]=0;
				
				if(!rename(present, future)) {
					refresh(dir_list, scroll_bit, list_size);
				} else {
					error.append("Unable to rename the file: ");
					error.append(present);
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				}
			} else {
				string error = "invalid rename command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("move")) {

		} else if(!(*itr).compare("delete_dir")) {
			int arg_len = arguments.size();
			string error;
			if(arg_len==1) {
				string file_path = arguments[0];
				char *dest = generate_abs_path(file_path);
		
				int flag = delete_directory(dest);
				

				if (flag) {
					error.append("Unable to delete the directory: ");
					error.append(file_path);
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {

					refresh(dir_list, scroll_bit, list_size);
				}
			} else {
				error.append("invalid delete_dir command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}
		}
	} else {
		string error = "\"" + cmd_input + "\" as a command does not exist";
		show_error(error);
        cout<<"\033["<<win_row<<";16H";
	}
}

void show_error(string error) {
	cout<<"\033["<<win_row-1<<";35H"<<LRED<<"MESSAGE: "<<error;
}

void storeCmd(char ch) {
	if(cmd_cur<win_col)
		cout<<WHITE<<ch;
	cmd_cur++;
	cmd.push_back(ch);
}