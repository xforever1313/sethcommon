import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import thisDir

arm = False

versionFile = os.path.abspath('version.txt')

def getVersionString():
    versionFile = open('version.txt', 'r')
    versionString = versionFile.readline().strip()
    versionFile.close()
    revisionFile = open('revision.txt', 'r')
    versionString += '+' + revisionFile.readline().strip()
    revisionFile.close()
    return versionString

versionString = getVersionString() 

if (len(sys.argv) == 2):
    if (sys.argv[1] == "arm"):
        arm = True
        
i = 0
while (i < len(targetLocations)):
    os.chdir(targetLocations[i])
    if (arm): 
        commandStr = 'scons create_lib doxygen arm_build=1 version="' + versionString + '" version_file=' + versionFile
    else:
        commandStr = 'scons create_lib doxygen arm_build=0 version="' + versionString + '" version_file=' + versionFile
    print("Building library for" + targetNames[i])
    print(commandStr)
    status = subprocess.call(commandStr, shell=True)

    os.chdir(thisDir)
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)
