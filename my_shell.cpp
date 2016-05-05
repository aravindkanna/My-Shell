#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]){
	string command;
	pid_t pid;
	int status;
	while(1){
		cout << "my_shell $: ";
		getline(cin, command);
		vector<string> splt_cmnd;
		
		pid = fork();//this creates a new child
		if(pid){
			waitpid(pid, &status, WUNTRACED);
			getchar();
		}
		else{
			stringstream ss(command);
			string token;
			while(getline(ss, token, ' ')){
				if(token.length()){
					splt_cmnd.push_back(token);
				}
			}
		}
	}
	return 0;
}