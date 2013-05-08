#Builds all the libraries

import os
import subprocess

#Add to here the directories that contain libs that need to be built
libDirs = [os.path.join("Common", "os"), os.path.join("Common", "SCSV") "Notebook-Core", "SkyWrite-Core"]

thisDir = os.getcwd()

errors = 0

for dir in libDirs:
    os.chdir(dir)
    cleanLib = "scons create_lib --clean"
    print (cleanLib)
    subprocess.call(cleanLib, shell=True)
    
    createLib = "scons create_lib"
    print (createLib)
    status = subprocess.call(createLib, shell=True)
    if (status != 0):
        errors = errors + 1

    os.chdir(thisDir)
        
if (errors != 0):
    print ("***Errors have occured!***")