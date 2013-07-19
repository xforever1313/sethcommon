#Builds all the libraries

from Globals import parseArguments
import os
import subprocess
import sys

#Add to here the directories that contain libs that need to be built
libDirs = [os.path.join("Common", "os"), os.path.join("Common", "SCSV")]

thisDir = os.getcwd()

errors = 0

arm = parseArguments()

if (arm):
    cleanCommand = "scons create_lib arm_build=1 --clean"
    buildCommand = "scons create_lib arm_build=1"
else:
    cleanCommand = "scons create_lib arm_build=0 --clean"
    buildCommand = "scons create_lib arm_build=0"

for dir in libDirs:
    os.chdir(dir)

    print (cleanCommand)
    subprocess.call(cleanCommand, shell=True)
    
    print (buildCommand)
    status = subprocess.call(buildCommand, shell=True)
    if (status != 0):
        errors = errors + 1

    os.chdir(thisDir)
        
if (errors != 0):
    print ("***Errors have occured!***")

exit(errors)
