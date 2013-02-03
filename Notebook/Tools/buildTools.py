#Description:
#This files builds all tools with the help of code::blocks

import subprocess
import os

os.chdir("src")
buildTargetCommand = "g++ buildTargets.cpp -o bin/buildTargets.exe"
print (buildTargetCommand)
x = subprocess.call(buildTargetCommand)
if (x != 0):
    print ("WARNING! Build Failed!")

ccoverageCommand = "codeblocks --rebuild --target=\"Release\" ccoverage.cbp"
print(ccoverageCommand)
y = subprocess.call(ccoverageCommand)
if (y != 0):
    print ("WARNING! Build Failed!")

if (x == 0 and y == 0):
    print ("All Tools build successfully!")
else:
    print("WARNING! Some tools did not build")

input("Press ENTER to continue")