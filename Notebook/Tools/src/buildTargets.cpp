#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Usage:
 * buildTargets.exe - unclean build of targets in buildTargets.txt
 * buildTargets.exe clean - clean build of targets in buildTargets.txt
 * buildTargets.exe projectFile - unclean build of targets in projectFile
 * buildTargets.exe clean projectFile - clean build of targets in projectFile
 */

int main(int argc, const char* argv[])
{
    std::string fileString = "../../buildTargets.txt";
    std::string buildString = "--build";

    if ((argc >= 2) && (std::string(argv[1]) == "clean")){
        buildString = "--rebuild";
        if (argc ==3){
            fileString = argv[2];
        }
    }
    else if ((argc == 2) &&  (std::string(argv[1]) != "clean")){
        fileString = argv[1];
    }

	int numFailed = 0;
	int mainRet;
	cout<<"Building Projects"<<std::endl;

	ifstream inFile;

	inFile.open(fileString.c_str());

    if (inFile.fail()){
        cout<<"Could not open file project target file"<<endl;
        return -2;
    }

	inFile.peek();

	string path = "../../";
	string fileName;
	string target;

	while (!inFile.eof() && !inFile.fail()){
		string temp;
		inFile >> temp;
		path += temp;
		inFile >> fileName;
		inFile >> target;

		stringstream ss;

		ss << "codeblocks "<< buildString <<" --target=\"" << target << "\" " << path << "\\" << fileName;

		cout<<ss.str() <<endl;

		int ret = system(ss.str().c_str());

		if (ret != 0){
			cout<<"BUILD FAILED"<<endl;
			++numFailed;
		}

		inFile.peek();
		path = "../../";
	}

	if (inFile.fail() && !inFile.eof()){
		cout<<"File could not be opened"<<endl;
		return -1;
	}
	else{
		inFile.close();
	}
	if (numFailed != 0){
		cout << "WARNING! "<<numFailed<<" Files do not build"<<endl;
		mainRet = numFailed;
	}
	else{
		cout<<"All targets built!"<<endl;
		mainRet=0;
	}

	return mainRet;
}
