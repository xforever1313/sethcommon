#Description:
#This python file calls an clean build of all targets in buildTargets.txt with the help of buildTargets.exe
import subprocess
import os

os.chdir("src/bin")
subprocess.call("buildTargets.exe clean")

input("Press ENTER to continue")