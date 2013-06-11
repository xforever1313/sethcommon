#include <sstream>

#include "FilenameChecker.h"

namespace Common{

///\warning each replacement character must be different

const std::string FilenameChecker::ESCAPE_CHARACTER = "+";

const std::string FilenameChecker::BACK_SLASH = "\\";
const std::string FilenameChecker::BACK_SLASH_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "b" + FilenameChecker::ESCAPE_CHARACTER; //Back slash

const std::string FilenameChecker::FRONT_SLASH = "/";
const std::string FilenameChecker::FRONT_SLASH_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "f" + FilenameChecker::ESCAPE_CHARACTER; //Forward slash

const std::string FilenameChecker::COLON = ":";
const std::string FilenameChecker::COLON_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "c" + FilenameChecker::ESCAPE_CHARACTER; //Colon

const std::string FilenameChecker::ASTERIC = "*";
const std::string FilenameChecker::ASTERIC_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "s" + FilenameChecker::ESCAPE_CHARACTER; //aSteric

const std::string FilenameChecker::AMP = "&";
const std::string FilenameChecker::AMP_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "m" + FilenameChecker::ESCAPE_CHARACTER; //aMp

const std::string FilenameChecker::QUESTION = "?";
const std::string FilenameChecker::QUESTION_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "e" + FilenameChecker::ESCAPE_CHARACTER; //quEstion

const std::string FilenameChecker::QUOTE = "\"";
const std::string FilenameChecker::QUOTE_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "o" + FilenameChecker::ESCAPE_CHARACTER; //quOte

const std::string FilenameChecker::APOS = "\'";
const std::string FilenameChecker::APOS_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "p" + FilenameChecker::ESCAPE_CHARACTER; //aPos

const std::string FilenameChecker::LT = "<";
const std::string FilenameChecker::LT_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "l" + FilenameChecker::ESCAPE_CHARACTER; //Less than

const std::string FilenameChecker::GT = ">";
const std::string FilenameChecker::GT_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "g" + FilenameChecker::ESCAPE_CHARACTER; //Greater than

const std::string FilenameChecker::PIPE = "|";
const std::string FilenameChecker::PIPE_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "i" + FilenameChecker::ESCAPE_CHARACTER; //pIpe

const std::string FilenameChecker::CARROT = "^";
const std::string FilenameChecker::CARROT_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "r" + FilenameChecker::ESCAPE_CHARACTER; //caRrot

const std::string FilenameChecker::PLUS = "+";
const std::string FilenameChecker::PLUS_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "u" + FilenameChecker::ESCAPE_CHARACTER; //plUs

const std::string FilenameChecker::SPACE = " ";
const std::string FilenameChecker::SPACE_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "_" + FilenameChecker::ESCAPE_CHARACTER;

const std::string FilenameChecker::PERIOD = ".";
const std::string FilenameChecker::PERIOD_REPLACE = FilenameChecker::ESCAPE_CHARACTER + "d" + FilenameChecker::ESCAPE_CHARACTER; //perioD

std::string FilenameChecker::checkFilename(std::string filename){
    if (filename.size() >= FilenameChecker::MAX_FILE_SIZE){
        std::stringstream ss;
        ss << "Filename " << filename << " is bigger than " << FilenameChecker::MAX_FILE_SIZE << " characters";
        throw FilenameCheckerException(ss.str());
    }

    for (unsigned int i = 0; i < filename.size(); ++i){
        std::string replacementString = "";
        if (filename[i] == FilenameChecker::SPACE[0]){
            replacementString = FilenameChecker::SPACE_REPLACE;
        }
        else if (filename[i] == FilenameChecker::BACK_SLASH[0]){
            replacementString = FilenameChecker::BACK_SLASH_REPLACE;
        }
        else if (filename[i] == FilenameChecker::FRONT_SLASH[0]){
            replacementString = FilenameChecker::FRONT_SLASH_REPLACE;
        }
        else if (filename[i] == FilenameChecker::COLON[0]){
            replacementString = FilenameChecker::COLON_REPLACE;
        }
        else if (filename[i] == FilenameChecker::ASTERIC[0]){
            replacementString = FilenameChecker::ASTERIC_REPLACE;
        }
        else if (filename[i] == FilenameChecker::AMP[0]){
            replacementString = FilenameChecker::AMP_REPLACE;
        }
        else if (filename[i] == FilenameChecker::QUESTION[0]){
            replacementString = FilenameChecker::QUESTION_REPLACE;
        }
        else if (filename[i] == FilenameChecker::QUOTE[0]){
            replacementString = FilenameChecker::QUOTE_REPLACE;
        }
        else if (filename[i] == FilenameChecker::APOS[0]){
            replacementString = FilenameChecker::APOS_REPLACE;
        }
        else if (filename[i] == FilenameChecker::LT[0]){
            replacementString = FilenameChecker::LT_REPLACE;
        }
        else if (filename[i] == FilenameChecker::GT[0]){
            replacementString = FilenameChecker::GT_REPLACE;
        }
        else if (filename[i] == FilenameChecker::PIPE[0]){
            replacementString = FilenameChecker::PIPE_REPLACE;
        }
        else if (filename[i] == FilenameChecker::CARROT[0]){
            replacementString = FilenameChecker::CARROT_REPLACE;
        }
        else if (filename[i] == FilenameChecker::PLUS[0]){
            replacementString = FilenameChecker::PLUS_REPLACE;
        }
        else if (filename[i] == FilenameChecker::PERIOD[0]){
            replacementString = FilenameChecker::PERIOD_REPLACE;
        }

        if (replacementString != ""){
            filename.erase(i, 1);
            filename.insert(i, replacementString);
            i += replacementString.size() - 1;
        }
    }

    return filename;
}


}
