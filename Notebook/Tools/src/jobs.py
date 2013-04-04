import os

notebookRootDirectory = os.path.abspath(os.path.join("..",".."))

#Add to these lists where to call scons
targetLocations = []
targetNames = []

#Core build
targetNames += ["OSBuild"] #Must be built first
targetLocations += [os.path.join(notebookRootDirectory,"Common", "os")]

targetNames += ["CoreBuild"]
targetLocations += [os.path.join(notebookRootDirectory,"Core")]