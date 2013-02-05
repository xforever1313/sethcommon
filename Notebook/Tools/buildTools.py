#Description:
#This files builds all tools with the help of code::blocks

import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
logPath = os.path.join(absPath, "logs")
sys.path.append(headerPath)
import toolHeader

toolHeader.buildTools(os.path.join(absPath, "src"), logPath)

input("Press ENTER to continue")