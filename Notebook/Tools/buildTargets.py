#Description:
#This python file calls an unclean build of all targets in buildTargets.txt with the help of buildTargets.exe
import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
sys.path.append(headerPath)
import toolHeader

executablePath = os.path.join(absPath, "src/bin")
logPath = os.path.join(absPath, "logs")
toolHeader.build(executablePath, logPath)

input("Press ENTER to continue")