#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using namespace boost::filesystem;

int main(int argc, char* argv[]){

    vector<std::string> projectVector;
    ifstream inFile;
    int retValue = 0;
    if (argc == 1){
        inFile.open("projects.txt");
        if (inFile.fail()){
            cerr<<"Could not open projects.txt"<<endl;
            return -2;
        }

        inFile.peek();
        while (!inFile.eof()){
            std::string projectString;
            inFile >> projectString;
            projectVector.push_back(projectString);
            inFile.peek();
        }
        inFile.close();
    }
    else{
        for (int i = 1; i < argc; ++i){
            projectVector.push_back(string(argv[1]));
        }
    }

    for (unsigned int i = 0; i < projectVector.size(); ++i){
        //Get path to project
        path projectPath(projectVector[i]);
        path textFilePath = projectPath;
        textFilePath /= (path("coverage.txt"));

        path objectPath(projectVector[i]);
        objectPath /= (path("obj"));

        path coveragePath(projectVector[i]);
        coveragePath /=(path("codeCoverage"));

        //Create a code coverage director
        if (exists(coveragePath)){
            remove_all(coveragePath);
        }
        bool created = false;
        try{
           created = create_directory(coveragePath);
        }
        catch(boost::filesystem::filesystem_error e){
            cerr<<e.what()<<endl;
        }

        if (created){
            vector<string> cppVector;

            std::string textFilePathString = textFilePath.string();
            for (unsigned int i = 0; i < textFilePathString.size(); ++i){
                if (textFilePathString[i] == '\\')
                    textFilePathString[i] = '/';
            }

            inFile.open(textFilePathString.c_str());
            if (!inFile.fail()){
                inFile.peek();
                while (!inFile.eof()){
                    std::string fileString;
                    inFile >> fileString;
                    cppVector.push_back(fileString);
                    inFile.peek();
                }
                inFile.close();

                //Run gcov.  This assumes that the directory shifts to the project's code coverage folder
                stringstream ss;
                ss << "start /D\"" << coveragePath.string() << "\" /B" << " gcov " << " -s \"../\"" << " -o " << "\"../obj\"" << " ";
                for (unsigned int i = 0; i < cppVector.size(); ++i){
                    ss << cppVector[i] << " ";
                }
                path summaryPath  = coveragePath;
                summaryPath /= "coverageSummary.txt";

                ss << "> " << summaryPath.string();
                system(ss.str().c_str());
                system("PING -n 1 -w 3000 1.1.1.1 > nul"); //wait for 3 seconds

                //Remove unneeded files
                directory_iterator file(coveragePath);
                directory_iterator dirEnd;
                for (; file!=dirEnd; ++file){
                    bool found = false;

                    if ((*file).path().filename().string() == "coverageSummary.txt"){
                        found = true;
                    }

                    for (unsigned int i = 0; i < cppVector.size() && !found; ++i){
                        std::string currentFile = cppVector[i];
                        currentFile += ".gcov";
                        if (currentFile == (*file).path().filename().string()){
                            found = true;
                            break;
                        }
                    }
                    if (!found){
                        remove((*file).path());
                    }
                }

                //Lastly, output the summaries
                std::string summaryPathString = summaryPath.string();
                inFile.open(summaryPathString.c_str());
                std::string summaryString = "";
                while (!inFile.eof()){
                    std::string input;
                    inFile >> input;
                    if (input == "File"){
                        std::string fileName = "";
                        do{
                            fileName += inFile.get();
                        }while(inFile.peek()!='\n');

                        fileName.erase(0, 1);
                        fileName.erase(fileName.size()-1, 1);
                        path filePath(fileName);
                        bool found = false;
                        for (unsigned int i = 0; i < cppVector.size() && !found; ++i){
                            if (cppVector[i] == filePath.filename().string()){
                                found = true;
                                break;
                            }
                        }

                        std::string linesExecuted = "";
                        do{
                            linesExecuted += inFile.get();
                        }while(inFile.peek()!='\n');

                        if (found){
                            summaryString += filePath.filename().string();
                            summaryString += linesExecuted;
                            summaryString += "\n\n";
                        }

                    }
                    else if (input == "Lines"){
                        summaryString += "\nTotal Lines";
                        while (!inFile.eof()){
                            summaryString += inFile.get();
                        }
                    }
                }
                inFile.close();
                cout<<summaryString<<endl;

                ofstream outFile;
                outFile.open(summaryPathString.c_str());
                outFile << summaryString << endl;
                outFile.close();

            }
            else{
                cerr<<"Could not locate " << projectPath.string() << endl;
            }
        }
        else{
            cerr << "Could not create directory " << coveragePath.string() << endl;
        }
    }

    return retValue;
}
