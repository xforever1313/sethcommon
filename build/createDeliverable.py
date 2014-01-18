#CreateDeliverable.py
#Creates the SkyvoCommon deliverable.

import glob
import os
import subprocess
import sys
import tarfile

baseDir = "../"
sys.path.append(baseDir) #Get globals
from Globals import *
from SkyvoCommonGlobals import *
from jobs import *

revisionFile = 'revision.txt'

def createRevisionFile():
    revFile = open(revisionFile, 'w')
    revFile.write(getRevisionNumber())
    revFile.close()

versionNumber = getReleaseVersionNumber(baseDir)
tarFileName = "SkyvoCommon-" + versionNumber + ".tar.gz"
createRevisionFile()

if os.path.exists(tarFileName):
    os.remove(tarFileName)

releaseFiles = []
for directory, dirnames, filenames in os.walk(getCommonPath(baseDir)):
    if (os.path.basename(directory) != ("debug_new" or "rapidxml" or "rapidjson")):
        if (os.path.basename(directory) == includeDir):
            releaseFiles += [directory]
        elif ((os.path.basename(directory) == srcDir) and (directory != testDir)):
            releaseFiles += [directory]
        if ("SConstruct" in filenames):
            releaseFiles += [os.path.join(directory, "SConstruct")]
        if ("Doxyfile" in filenames):
            releaseFiles += [os.path.join(directory, "Doxyfile")]

debugNewFiles = glob.glob(os.path.join(getDebugNewPath(baseDir), "*.h")) + \
                glob.glob(os.path.join(getDebugNewPath(baseDir), "*.cpp")) + \
                [os.path.join(getDebugNewPath(baseDir), "SConstruct")] + \
                [os.path.join(getDebugNewPath(baseDir), "README")]

dateVersionFiles = glob.glob(os.path.join(getDateVersionPath(baseDir), '*.h')) + \
                   glob.glob(os.path.join(getDateVersionPath(baseDir), '*.cpp'))
                
releaseFiles += debugNewFiles
releaseFiles += dateVersionFiles
releaseFiles += [getRapidXMLPath(baseDir)]
releaseFiles += [getRapidJSONPath(baseDir)]

releaseFiles += [os.path.join(baseDir, "Globals.py")]
releaseFiles += [os.path.join(baseDir, "SConstructGlobals.py")]
releaseFiles += [os.path.join(baseDir, "SkyvoCommonGlobals.py")]

releaseFiles += [getTestMain(baseDir)]

buildFiles = ["jobs.py", "buildRelease.py", "build.py"]

tar = tarfile.open(tarFileName, 'w:gz')
for file in releaseFiles:
    tar.add(file, arcname=file.strip(baseDir))
for file in buildFiles:
    tar.add(file, arcname=os.path.join("Common", file))
tar.add("version.txt", arcname="Common/version.txt")
tar.add(revisionFile, arcname=os.path.join('Common', revisionFile))
tar.add(os.path.join(baseDir, "tools"), arcname=("Common/tools"))
tar.close()
