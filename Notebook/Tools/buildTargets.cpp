#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	int numFailed = 0;
	int mainRet;
	cout<<"Building Projects"<<std::endl;
	
	ifstream inFile;
	
	inFile.open("projects.txt");
	
	inFile.peek();
	
	string path;
	string fileName;
	string target;
	
	while (!inFile.eof() && !inFile.fail()){
		inFile >> path;
		inFile >> fileName;
		inFile >> target;
		
		stringstream ss;
		
		ss << "codeblocks.exe  --rebuild --target=\"" << target << "\" " << path << "\\" << fileName;
		
		cout<<ss.str() <<endl;
		
		int ret = system(ss.str().c_str());
		
		if (ret != 0){
			cout<<"BUILD FAILED"<<endl;
			++numFailed;
		}
		
		inFile.peek();
	}
	
	if (inFile.fail() && !inFile.eof()){
		cout<<"File could not be opened"<<endl;
		system("pause");
		return -1;
	}
	else{
		inFile.close();
	}
	if (numFailed != 0){
		cout << "WARNING! "<<numFailed<<" Files does not build"<<endl;
		mainRet = numFailed; 
	}
	else{
		cout<<"All targets built!"<<endl;
		mainRet=0;
	}	
	system("pause");
	
	return mainRet;
}
