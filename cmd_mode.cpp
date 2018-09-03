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
		if(path.compare(".")) {
			move_to_dir[i++] = '/';
			while(path[j]!=0) {
				move_to_dir[i++]=path[j];
				j++;
			}
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


void load_search_result(vector <struct dirent *> &dir_list, bool &scroll_status, int &list_size, string directory) {
	top_bottom_bar(win_row, win_col);
    int i;
    char arr[100];
    for(i=0;directory[i]!=0;i++)
        arr[i] = directory[i];
    arr[i]=0;

    struct stat path_stat;
    stat(arr, &path_stat);
    if(S_ISREG(path_stat.st_mode)) {
    	char file[1000];
        strcpy(file, "xdg-open ");
        strcat(file, arr);
        strcat(file, " > log.txt");
        system(file);	
    } else {
    	dir_list.clear();
	    dir_list = ls_cmd(arr);
	    dir_his.push_back(arr);
	    his_itr = dir_his.end();
	    his_itr = prev(his_itr, 1);
	    pwd.assign(arr);
	    twd.assign(arr);
	    bool scroll_bit = false;
	    int list_size = dir_list.size();
	    if(list_size>windows_capacity) {
	        display(dir_list, 0, windows_capacity-1, scroll_status, "D");
	        scroll_bit = true;
	    } else {
	        display(dir_list, 0, list_size-1, scroll_status, "D");
	    }
    }
    refresh(dir_list, scroll_status, list_size);
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
			string error;
			int arg_len = arguments.size();
			if(arg_len>=2) {
				string dest_path = arguments[arg_len-1];
				char *dest_ = generate_abs_path(dest_path);
				if(!dest_path.compare("..")) {
					error.append("invalid destination directory mentioned");
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return; 
				} 
				int eflag = 0;
				for(int i=0;i<arg_len-1;i++) {
					string file_path_ = arguments[i];
					char *file_ = generate_abs_path(file_path_);
					struct stat path_stat;
    				stat(file_, &path_stat);
    				
    				if(S_ISREG(path_stat.st_mode)) {
    					int s=0;
    					while(dest_[s]!=0)
    						s++;
    					dest_[s]='/';
    					
    					int file_path_length = file_path_.length();
    					int t=file_path_length-1, u=0;
    					while(file_path_[t]!='/' && t>=0)
    						t--;
    					while(t!=file_path_length)
    						dest_[++s] = file_path_[++t];
    					dest_[s]=0;
    					
    					ifstream iifile(dest_);
    					if(!iifile) {
    						ifstream source(file_, ios::binary);
    						ofstream dest(dest_, ios::binary);
    						dest << source.rdbuf();
    						source.close();
    						dest.close();
    						refresh(dir_list, scroll_bit, list_size);
    					} else {
    						eflag=1;
    					}
    				} else if(S_ISDIR(path_stat.st_mode)) {
    					int cflag = copy_dir(file_, dest_);
    					if(cflag) {
    						error.append("Unable to copy the files or folders.");
							show_error(error);
							cout<<"\033["<<win_row<<";16H";
    					}
    				}
    				free(dest_);
    				dest_ = generate_abs_path(dest_path);
				}
				if(eflag == 1) {
					error.append("Unable to copy the file");
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {
					//refresh(dir_list, scroll_bit, list_size);
				}
			} else {
				error.append("invalid copy command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}
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
				free(dest);
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
				free(dest);
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
				free(dest);
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
                    free(move_to_dir);
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
				refresh(dir_list, scroll_bit, list_size);
				free(dump_file);
				free(dump_folder);
			} else {
				string error = "invalid snapshot command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("search")) {
			string error;
			if(arguments.size()==1) {
				string dir = pwd;
				string val = arguments[0];
				vector <string> results = search(dir, val);
				if(results.empty()) {
					error = "No results found";
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
				} else {
				    int search_size = results.size();
				    cout<<CLEAR;
				    string str = "...Welcome to the file explorer...";
				    int len = str.length();
				    int start_col = win_col/2 - len/2;
				    cout<<"\033["<<win_row-1<<";1H";
				    cout<<GREEN<<" EXIT FROM SEARCH SCREEN "<<WHITE<<"\"<-\"";
				    cout<<"\033["<<win_row<<";1H"<<GREEN<<" Type the S.NO. to open the file or enter the directory: ";

				    cout<<"\033[1;"<<start_col<<"H";
				    cout<<GREEN<<str;
				    cout<<"\033[2;1H";
				    for(int i=0;i<win_col;i++)
				        cout<<".";
				    cout<<"\033[3;0H";
				    cout<<"List of files and directories retrieved after searching "<<WHITE<<pwd;
			        cout<<"\033[5;6H";
			        cout<<LCYAN<<"S. NO.";
			        cout<<"\033[5;26H";
			        cout<<"FILE/ DIRECTORY NAME";
			        cout<<"\033[5;60H";
			        cout<<"FILE/ FOLDER ABSOLUTE PATH";
			        cout<<endl;

			        int row = 6;
			        for(int i=1;i<=search_size;i++) {
			        	string absolute_path = results[i-1];
			        	string file_dir_name;
			        	int p=absolute_path.length() - 1;
			        	while(absolute_path[p]!='/' || p==absolute_path.length()-1) {
			        		file_dir_name.push_back(absolute_path[p--]);
			        	}
			        	char fd_name[100];
			        	int l, m;
			        	for(l=0,m=file_dir_name.length()-1;m>=0;m--,l++) {
			        		fd_name[l] = file_dir_name[m];
			        	}
			        	if(row <= win_row-2) {
			        		fd_name[l]=0;
				        	cout<<YELLOW<<"\033["<<row<<";6H"<<i;
				        	cout<<WHITE<<"\033["<<row<<";26H"<<fd_name;
				        	cout<<WHITE<<"\033["<<row<<";60H"<<absolute_path;
				        	row++;
			        	}
			        }

			        cout<<GREEN<<"\033["<<win_row<<";58H"<<WHITE;
    				int option = 0;
    				int typed_cur = 58;
    				string typed;
				    while(true) {
				    	char key_press = check_keypress();
				    	if(key_press == -13) {
		                    refresh(dir_list, scroll_bit, list_size);
		                } else if(key_press == 10) {
		                	int typed_length = typed.size();
		                	for(int i=0;i<typed_length;i++) {
		                		int digit = typed[i]-48;
		                		option = (option * 10) + digit;
		                	}
		                	if(option>0 && option<=search_size) {
		                    	load_search_result(dir_list, scroll_bit, list_size, results[option-1]);                		
			                    return;
		                	} else {
		                		error.append("The selected option was wrong");
		                		typed.clear();
		                		option = 0;
								show_error(error);
		                		for(int i=58;i<=win_col;i++)
		                			cout<<WHITE<<"\033["<<win_row<<";"<<i<<"H"<<" ";
		                		typed_cur=58;
		                		cout<<"\033["<<win_row<<";"<<typed_cur<<"H";
		                	}
		                } else if(key_press == 127) {
		                	int typed_length = typed.size();
							if(typed_length > 0) {
								typed.pop_back();
								typed_cur--;
								cout<<"\033["<<win_row<<";"<<typed_cur<<"H"<<" "<<"\033["<<win_row<<";"<<typed_cur<<"H";
							}
		                } else if(isdigit(key_press)!=0){
		                	cout<<key_press;
		                	typed_cur++;
		                	typed.push_back(key_press);
		                }
				    }
				
				}
			} else {
				error = "invalid search command";
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
					vector <string>::iterator del_itr = find(dir_his.begin(), dir_his.end(), present);
					if(del_itr!=dir_his.end()) {
						dir_his.erase(del_itr);
					}
					refresh(dir_list, scroll_bit, list_size);
				} else {
					error.append("Unable to rename the file/folder: ");
					error.append(present);
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				}
				free(present);
			} else {
				string error = "invalid rename command";
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("move")) {
			int arg_len = arguments.size();
			string error;
			if(arg_len>=2) {
				string dest_path = arguments[arg_len-1];
				char *dest = generate_abs_path(dest_path);
				if(!dest_path.compare("..")) {
					error.append("invalid destination directory mentioned");
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return; 
				} 
				int eflag = 0;
				for(int i=0;i<arg_len-1;i++) {
					string file_path_ = arguments[i];
					char *file_ = generate_abs_path(file_path_);
					struct stat path_stat;
    				stat(file_, &path_stat);
    				
    				int s=0;
    				while(dest[s]!=0)
    					s++;
    				dest[s]='/';
    				
    				int file_path_length = file_path_.length();
    				int t=file_path_length-1, u=0;
    				while(file_path_[t]!='/' && t>=0)
    					t--;
    				while(t!=file_path_length)
    					dest[++s] = file_path_[++t];
    				dest[s]=0;
    				
    				if(!rename(file_, dest)) {
						refresh(dir_list, scroll_bit, list_size);
					} else {
						eflag = 1;
					}

    				free(dest);
    				dest = generate_abs_path(dest_path);
				}
				if(eflag == 1) {
					error.append("Unable to move the file or directory.");
					show_error(error);
					cout<<"\033["<<win_row<<";16H";
					return;
				} else {
					refresh(dir_list, scroll_bit, list_size);
				}
			} else {
				error.append("invalid move command");
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
				return;
			}
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
					free(dest);
					return;
				} else {
					refresh(dir_list, scroll_bit, list_size);
					free(dest);
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