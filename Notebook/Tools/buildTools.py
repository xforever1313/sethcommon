#Description:
#This files builds all tools with the help of code::blocks

import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
sys.path.append(headerPath)
import toolHeader

toolHeader.buildTools(os.path.join(absPath, "src"))

input("Press ENTER to continue")