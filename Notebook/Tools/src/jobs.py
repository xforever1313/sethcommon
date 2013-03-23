import os

notebookRootDirectory = os.path.abspath(os.path.join("..",".."))

#Add to these lists where to call scons
targetLocations = []
targetNames = []

#Core build
targetNames += ["CoreBuild"]
targetLocations += [os.path.join(notebookRootDirectory,"Core")]