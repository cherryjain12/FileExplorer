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
/*
void create_dir() {
	
	
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
	
}

*/
string replace_name(string full, string replace, string replace_with) {
	string x;
	int replace_len = replace.length();
	full.replace(0, replace_len, replace_with);
	x.assign(full);
	return x;
}

int copy_dir(char *dump_folder, char *dest) {
	string error;

	string folder_root, d_folder;
	int k=0, l;
	while(dump_folder[k]!=0) {
		d_folder.push_back(dump_folder[k]);
		k++;
	}
	for(l=k-1;l>=0;l--) {
		if(dump_folder[l]=='/')
			break;
	}

	for(int m=l+1;m<k;m++)
		folder_root.push_back(dump_folder[m]);

	queue <string> que;
	
	que.push(dump_folder);
	
	vector <string> fi_n_fo;
	vector <string> fi_n_fn;

	fi_n_fo.push_back(que.front());

	string base;
	base.append(dest);
	base.append("/");
	base.append(folder_root);
	fi_n_fn.push_back(base);
	//cout<<d_folder<<":"<<base<<":"<<endl;
	while(!que.empty()) {
		string folder = que.front();
		que.pop();
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
				string new_folder;
				new_folder.assign(folder);
				new_folder.append("/");
				new_folder.append(dir);
				string fresh_str = replace_name(new_folder, d_folder, base);
				//fi_n_fn.push_back(fresh_str);
				if(directories[i]->d_type == DT_REG) {
					fi_n_fo.push_back(new_folder);
					fi_n_fn.push_back(fresh_str);
				}
				if(directories[i]->d_type == DT_DIR && dir.compare(".") && dir.compare("..")) {
					fi_n_fo.push_back(new_folder);
					fi_n_fn.push_back(fresh_str);	
					que.push(new_folder);
				} 
				new_folder.clear();
				fresh_str.clear();
			}
		} else {
			return 1;
		}
	}
	int vsize = fi_n_fo.size();
	for(int i=0;i<vsize;i++) {
		string temp = fi_n_fo[i];
		char statstr[1000];
		int k;
		for(k=0;temp[k]!=0;k++)
			statstr[k]=temp[k];
		statstr[k]=0;
		struct stat path_stat;
    	stat(statstr, &path_stat);
		if(S_ISDIR(path_stat.st_mode)) {
			string mkfol = fi_n_fn[i];
			char mkfolder[1000];
			int j;
			for(j=0;mkfol[j]!=0;j++)
				mkfolder[j] = mkfol[j];
			mkfolder[j]=0;
			const int dir_err = mkdir(mkfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (-1 == dir_err) {
				return 1;
			}
		}
	}

	for(int i=0;i<vsize;i++) {
		string temp = fi_n_fo[i];
		char statstr[1000];
		int k;
		for(k=0;temp[k]!=0;k++)
			statstr[k]=temp[k];
		statstr[k]=0;
		struct stat path_stat;
    	stat(statstr, &path_stat);
		if(S_ISREG(path_stat.st_mode)) {
			ifstream iifile(fi_n_fn[i]);
			if(!iifile) {
				ifstream source(fi_n_fo[i], ios::binary);
				ofstream dest(fi_n_fn[i], ios::binary);
				dest << source.rdbuf();
				source.close();
				dest.close();
			} else {
				return 1;
			}
		}
	}

	return 0;
}
