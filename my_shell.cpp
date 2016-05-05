#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){
	string command;
	pid_t pid;
	int status;
	while(1){
		cout << "my_shell $: ";
		getline(cin, command);
		vector<string> splt_cmnd;
		int TYPE = 0;
		pid = fork();//this creates a new child
		int combd;
		if(pid){
			waitpid(pid, &status, WUNTRACED);
			getchar();
		}
		else{
			stringstream ss(command);
			string token;
			//combd = 0;
			while(getline(ss, token, ' ')){
				combd=0;
				if(token.length()){
					splt_cmnd.push_back(token);
					if(token[0]=='<'){
						TYPE = 1;
					}
					else if(token[0] == '>'){
						TYPE = 2;
						if(token[1]){
							if(token[1] == '>'){
								TYPE = 3;
							}
						}
					}
					if(token[0]=='<' && token[1] && token[1]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(0, 1);
						splt_cmnd.push_back(t.substr(1));
					}
					else if(token[0]=='>' && TYPE==2 && token[1] && token[1]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(0, 1);
						splt_cmnd.push_back(t.substr(1));
					}
					if(token[0]=='>' && token[1]=='>' && token[2] && token[2]!=' '){
						int len = splt_cmnd.size();
						string t = splt_cmnd[len-1];
						splt_cmnd[len-1] = t.substr(0, 2);
						splt_cmnd.push_back(t.substr(2));
					}	
				}
			}
		}
		/*for(int i=0;i<splt_cmnd.size();i++){
			cout << splt_cmnd[i] << endl;
		}*/

		/***
			TYPE-1 : taking a file as input
			TYPE-2 : writing the contents to the file by overriding
			TYPE-3 : writing the contents by appending
		***/
	}
	return 0;
}