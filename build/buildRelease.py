import time
import os
import subprocess
import sys

from jobs import *

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import thisDir, getRevisionNumber

arm = False

versionFile = os.path.abspath('version.txt')

def getVersionString():
    versionFile = open('version.txt', 'r')
    versionString = versionFile.readline().strip()
    versionFile.close()
    try:
        revisionFile = open('revision.txt', 'r')
        versionString += '+' + revisionFile.readline().strip()
        revisionFile.close()
    except:
        versionString += '+' + getRevisionNumber()
    return versionString

versionString = getVersionString() 

args = ['create_lib', 'doxygen', 'version="' + versionString + '"', 'version_file=' + versionFile]
for arg in sys.argv[1:]:
    if ((arg != 'delta') and (arg != 'nightly')):
        args += [arg]

status = subprocess.call(['python', 'build.py'] + args)
if (status != 0):
    raise Exception("Seth common release build failed")
