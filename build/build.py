import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import getRedirectString, timeProgram

thisDir = os.getcwd()
currentTime = time.strftime("%m_%d_%Y_%H_%M_%S")

logDir = os.path.join(thisDir, "logs", sys.argv[1] + "-" + currentTime)
os.makedirs(logDir)

args = ""

for arg in sys.argv[1:]:
    args += arg
    args += ' '

if (sys.platform == 'win32'):
    sconsCommand = 'scons.bat'
else:
    sconsCommand = 'scons'

i = 0
while (i < len(targetLocations)):
    logFile = os.path.join(logDir, targetNames[i] + ".log")

    print("Running scons " + args + "for " + targetNames[i])

    process = subprocess.Popen([sconsCommand] + sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=targetLocations[i])
    log = process.communicate()[0]
    f = open(logFile, 'w')
    f.write(log)
    f.close()
    status = process.wait()
    if (status != 0):
        raise Exception('A compile error occured!')
    
    i += 1

