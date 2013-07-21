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

(versionString, versionNumber) = getVersion(baseDir)
tarFileName = "SkyvoCommon-" + versionNumber + ".tar.gz"

if os.path.exists(tarFileName):
    os.remove(tarFileName)

releaseFiles = []
for directory, dirnames, filenames in os.walk(getCommonPath(baseDir)):
    if (os.path.basename(directory) != ("debug_new" or "math" or "rapidxml")):
        if ((os.path.basename(directory) == includeDir) and (testDir not in directory)):
            releaseFiles += [directory]
        elif ((os.path.basename(directory) == srcDir) and (testDir not in directory)):
            releaseFiles += [directory]
        if ("SConstruct" in filenames):
            releaseFiles += [os.path.join(directory, "SConstruct")]

debugNewFiles = glob.glob(os.path.join(getDebugNewPath(baseDir), "*.h")) + \
glob.glob(os.path.join(getDebugNewPath(baseDir), "*.cpp")) + \
[os.path.join(getDebugNewPath(baseDir), "SConstruct")] + \
[os.path.join(getDebugNewPath(baseDir), "README")]

releaseFiles += debugNewFiles
releaseFiles += [getMathPath(baseDir)]
releaseFiles += [getRapidXMLPath(baseDir)]

releaseFiles += [os.path.join(baseDir, "Globals.py")]
releaseFiles += [os.path.join(baseDir, "SConstructGlobals.py")]
releaseFiles += [os.path.join(baseDir, "SkyvoCommonGlobals.py")]

buildFiles = ["jobs.py", "buildRelease.py"]

tar = tarfile.open(tarFileName, 'w:gz')
for file in releaseFiles:
    tar.add(file, arcname=file.strip(baseDir))
for file in buildFiles:
    tar.add(file, arcname=os.path.join("Common", file))
tar.add("Version.txt", arcname="Common/Version.txt")
tar.close()
