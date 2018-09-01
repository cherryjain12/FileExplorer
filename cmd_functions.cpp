#include "headers.h"

vector <struct dirent *> ls_snap(char *dir_path) {
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

void generate_snapshot(char *dump_folder, char *dump_file) {
	string error;
	int fd = open(dump_file, O_CREAT | O_WRONLY, 0644);
	if (fd == -1) {
		error.append("Unable to create the file: ");
		error.append(dump_file);
		show_error(error);
		cout<<"\033["<<win_row<<";16H";
		return;
	}

	queue <string> que;
	ofstream myfile;
	myfile.open(dump_file);

	que.push(dump_folder);
	
	if (!myfile.fail()) {
		while(!que.empty()) {
			string folder = que.front();
			que.pop();
			myfile<<">> "<<folder<<" :\n";
			char ch_f[200];
			int i;
			for(i=0;folder[i]!=0;i++)
				ch_f[i]=folder[i];
			ch_f[i]=0;
			vector <struct dirent *> directories = ls_snap(ch_f);
			int len = directories.size();
			
			if(!directories.empty()) {
				for(int i=0;i<len;i++) {
					string dir = directories[i]->d_name;
					myfile<<"\t\t"<<dir<<"\n";
					if(directories[i]->d_type == DT_DIR && dir.compare(".") && dir.compare("..")) {
						string new_folder;
						new_folder.append(folder);
						new_folder.append("/");
						new_folder.append(dir);
						que.push(new_folder);
						new_folder.clear();
					}
				}
			} else {
				myfile<<"\t\t Unable to open this directory\n";
			}
		}
		myfile.close();
	} else {
		string error = "unable to open dump file";
		show_error(error);
		cout<<"\033["<<win_row<<";16H";
	}
}

int isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();
 
    for (int i = 0; i <= N - M; i++) {
        int j;
 
        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;
        if (j == M)
            return i;
    }
    return -1;
}


vector <string> search(string directory, string val) {
	string error;
	
	queue <string> que;
	
	que.push(directory);
	
	vector <string> search_results;

	while(!que.empty()) {
		string folder = que.front();
		que.pop();
		char ch_f[200];
		int i;
		for(i=0;folder[i]!=0;i++)
			ch_f[i]=folder[i];
		ch_f[i]=0;
		vector <struct dirent *> directories = ls_snap(ch_f);
		
		if(!directories.empty()) {
			int len = directories.size();
			for(int i=0;i<len;i++) {
				string dir = directories[i]->d_name;
				string new_folder;
				new_folder.append(folder);
				new_folder.append("/");
				new_folder.append(dir);
				if(directories[i]->d_type == DT_DIR && dir.compare(".") && dir.compare(".."))
					que.push(new_folder);
				
				if(isSubstring(val,dir) != -1){
					search_results.push_back(new_folder);
				}
				new_folder.clear();
			}
		} 
	}
	return search_results;
}


int delete_directory(char *delete_dir) {
	stack <string> stk;
	stk.push(delete_dir);
	int flag = 0;
	while(!stk.empty()) {
		string folder = stk.top();
		char ch_f[200];
		int i;
		for(i=0;folder[i]!=0;i++)
			ch_f[i]=folder[i];
		ch_f[i]=0;
		vector <struct dirent *> directories = ls_snap(ch_f);
		int len;
		if(!directories.empty() && directories.size()==2) {
			stk.pop();
			const int dir_err = rmdir(ch_f);
			
			vector <string>::iterator del_itr = find(dir_his.begin(), dir_his.end(), ch_f);
			if(del_itr!=dir_his.end()) {
				dir_his.erase(del_itr);
			}
			
			if (dir_err!=0) {
				flag = 1;
			}
		}
		if(!directories.empty() && directories.size()!=2) {
			len = directories.size();
			for(int i=0;i<len;i++) {
				string dir = directories[i]->d_name;
				string new_folder;
				new_folder.append(folder);
				new_folder.append("/");
				new_folder.append(dir);
				if(directories[i]->d_type == DT_DIR && dir.compare(".") && dir.compare("..")) {
					stk.push(new_folder);
				} else if(directories[i]->d_type != DT_DIR){
					char del_folder[200];
					int s;
					for(s=0;new_folder[s]!=0;s++)
						del_folder[s] = new_folder[s];
					del_folder[s] = 0;
					if (unlink(del_folder) < 0) {
						flag = 1;
					}
				}
			}
		} else if(directories.empty())
			flag = 1;
		directories.clear();
	}
	return flag;
}

void move_dir(char *source, char *dest) {
	string error;
	error.append("Unfortunately moving the directory functionality is not implemented");
	show_error(error);
	cout<<"\033["<<win_row<<";16H";
	return;
}
