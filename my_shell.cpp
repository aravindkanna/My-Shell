#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char* argv[]){
	string command;
	pid_t pid;
	int status;
	string input_file = "";
	string output_file = "";
	while(1){
		cout << "my_shell $: ";
		getline(cin, command);
		vector<string> splt_cmnd;
		vector<bool> TYPE(3);
		pid = fork();//this creates a new child
		int combd;
		if(pid){
			waitpid(pid, &status, WUNTRACED);
		}
		else{
			stringstream ss(command);
			string token;
			while(getline(ss, token, ' ')){
				combd=0;
				if(token.length()){
					if(splt_cmnd.size()){
						string bck = splt_cmnd.back();
						if(!strcmp(bck.c_str(), "<")){
							input_file = token;
							splt_cmnd.pop_back();
						}
						if(!strcmp(bck.c_str(), ">") || !strcmp(bck.c_str(), ">>")){
							output_file = token;
							splt_cmnd.pop_back();
						}
					}
					splt_cmnd.push_back(token);
					if(token[0]=='<'){
						TYPE[0] = true;
					}
					else if(token[0] == '>'){
						TYPE[1] = true;
						if(token[1]){
							if(token[1] == '>'){
								TYPE[1] = false;
								TYPE[2] = true;
							}
						}
					}
					if(token[0]=='<' && token[1] && token[1]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(1);
						input_file = splt_cmnd.back();
					}
					else if(token[0]=='>' && TYPE[1] && token[1] && token[1]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(0, 1);
						splt_cmnd.push_back(t.substr(1));
						output_file = splt_cmnd.back();
					}
					if(token[0]=='>' && token[1]=='>' && token[2] && token[2]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(2);
						output_file = splt_cmnd.back();
					}	

				}
			}

			int len=splt_cmnd.size();
			if(TYPE[0]){
				int ip = open(input_file.c_str(), O_RDONLY);
				dup2(ip, 0);
				close(ip);


			}
			if(TYPE[1]){
				remove(output_file.c_str());
				int op = open(output_file.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
				dup2(op, 1);
				close(op);
			}
			if(TYPE[2]){
				int op = open(output_file.c_str(), O_APPEND | O_WRONLY | O_CREAT, S_IRWXU);
				dup2(op, 1);
				close(op);
			}

			char *a[len];
			for(int i=0;i<len;i++){
				a[i] = strdup(splt_cmnd[i].c_str());
			}
			a[len] = NULL;
			execvp(a[0], a);
		}
	}
	return 0;
}