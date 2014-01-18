#Builds all the libraries

from Globals import parseArguments
import os
import subprocess
import sys

print "Building libs. Log file can be found in build/logs"

args = ""

for arg in sys.argv[1:]:
    args += arg
    args += ' '

subprocess.call("python build.py create_lib " + args, shell=True, cwd="build")
