#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Usage:
 * buildTargets.exe clean/unclean default/ProjectFile logLocation
 */

int main(int argc, const char* argv[])
{
    std::string fileString = "../../buildTargets.txt";
    std::string buildString = "--build";

    if (argc != 4){
        cout<<"Usage: buildTargets.exe clean/unclean default/ProjectFile logLocation"<<endl;
        return -10;
    }
    //Check first arg
    if (argv[1] == string("clean")){
        buildString = "--rebuild";
    }
    else if (argv[1] == string("unclean")){
        buildString = "--build";
    }
    else{
        cout<<"Invalid parameter for build type (use clean or unclean"<<endl;
        return -11;
    }

    //Check second arg
    if (argv[2] != string("default")){
        fileString = string(argv[2]);
    }

    std::string logPath = string(argv[3]);

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

		ss << "codeblocks "<< buildString <<" --target=\"" << target << "\" " << path << "\\" << fileName << " > " << logPath << "/" << fileName << "Build.log";

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
