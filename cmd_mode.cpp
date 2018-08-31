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
	int p=0, leave=0;
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

void onPressEnterC(vector <struct dirent *> &dir_list, bool &scroll_bit, int &list_size) {
	for(int i=16;i<win_col;i++)
		cout<<"\033["<<win_row<<";"<<i<<"H"<<" ";
	for(int i=50;i<win_col;i++)
		cout<<"\033["<<win_row-1<<";"<<i<<"H"<<" ";
	cout<<"\033["<<win_row<<";16H";
	string input_str, cmd_input;
	input_str.assign(cmd);
	cmd.clear();
	int i=0;
	while(input_str[i]!=' ') {
		cmd_input.push_back(input_str[i]);
		i++;
	}
	
	vector <string>::iterator itr = find(list_cmds.begin(), list_cmds.end(), cmd_input);
	
	if(itr != list_cmds.end()) {
		vector <string> arguments = processCmd(i, input_str);
		if(!(*itr).compare("copy")) {

		} else if(!(*itr).compare("create_dir")) {
			int arg_len = arguments.size();
			string error = "unable to create directory: ";
			int err_bit = 0;
			for(int i=0;i<len;i++) {
				const int dir_err = mkdir(arguments[i], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				if (-1 == dir_err) {
					err_bit = 1;
				    error.append(arguments[i]);
				}
			}
			if(err_bit) {
				show_error(error);
				cout<<"\033["<<win_row<<";16H";
			}
		} else if(!(*itr).compare("create_file")) {

		} else if(!(*itr).compare("delete_file")) {

		} else if(!(*itr).compare("goto")) {
			if(arguments.size()==1) {
				if(!arguments[0].compare("/")) {
					onPressHomeN(dir_list, scroll_bit, list_size);
				} else {
					char *move_to_dir = (char *)malloc(sizeof(char)*500);
					string path;
					int i; 
					path.assign(arguments[0]);
					for(i=0;home[i]!=0;i++)
						move_to_dir[i]=home[i];
					if(path[0]!='/')
						move_to_dir[i++]='/';
					int j=0;
					while(path[j]!=0) {
						move_to_dir[i++]=path[j];
						j++;
					}
					move_to_dir[i]=0;
					
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

		} else if(!(*itr).compare("search")) {

		} else if(!(*itr).compare("rename")) {

		} else if(!(*itr).compare("move")) {

		} else if(!(*itr).compare("delete_dir")) {

		}
	} else {
		string error = "\"" + cmd_input + "\" as a command does not exist";
		show_error(error);
        cout<<"\033["<<win_row<<";16H";
	}
}

void show_error(string error) {
	cout<<"\033["<<win_row-1<<";35H"<<LRED<<"ERROR: "<<error;
}

void storeCmd(char ch) {
	if(cmd_cur<win_col)
		cout<<WHITE<<ch;
	cmd_cur++;
	int i=0;
	cmd.push_back(ch);
}