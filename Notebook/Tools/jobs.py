import os

notebookRootDirectory = os.path.abspath("..")

#Add to these lists where to call scons
targetLocations = []
targetNames = []

#Core build
targetNames += ["OSBuild"] #Must be built first
targetLocations += [os.path.join(notebookRootDirectory,"Common", "os")]

targetNames += ['CommonClassesBuild']
targetLocations += [os.path.join(notebookRootDirectory, "Common", "common_classes")]

targetNames += ["SCSVBuild"]
targetLocations += [os.path.join(notebookRootDirectory, "Common", "SCSV")]

targetNames += ["NotebookCoreBuild"] 
targetLocations += [os.path.join(notebookRootDirectory,"Notebook-Core")]

targetNames += ["NotebookCLI"]
targetLocations += [os.path.join(notebookRootDirectory, "Notebook-CLI")]

targetNames += ["SkyCoreBuild"]
targetLocations += [os.path.join(notebookRootDirectory, "SkyWrite-Core")]