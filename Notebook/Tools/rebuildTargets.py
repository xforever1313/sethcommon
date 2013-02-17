#Description:
#This python file calls an clean build of all targets in buildTargets.txt with the help of buildTargets.exe
import sys
import os

absPath = os.getcwd()
headerPath = os.path.join(absPath, "src")
sys.path.append(headerPath)
import toolHeader

buildTargetsPath = os.path.join(absPath, "buildTargets.txt")
logPath = os.path.join(absPath, "logs")

status = toolHeader.rebuild(buildTargetsPath, logPath)
if(status == 0):
    print("All targets built!")
else:
    print(str(status) + " Targets failed to build!")

input("Press ENTER to continue")