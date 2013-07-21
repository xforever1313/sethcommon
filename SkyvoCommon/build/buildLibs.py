import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import thisDir

arm = False

if (len(sys.argv) == 2):
    if (sys.argv[1] == "arm"):
        arm = True
        
i = 0
while (i < len(targetLocations)):
    os.chdir(targetLocations[i])
    if (arm): 
        commandStr = "scons release_lib arm_build=1"
    else:
        commandStr = "scons release_lib -j4 arm_build=0"
    print("Building library for" + targetNames[i])
    status = subprocess.call(commandStr, shell=True)

    os.chdir(thisDir)
    if (status != 0):
        break
    i += 1
    
if (status != 0):
    print("A compile error occured!")
    exit(status)