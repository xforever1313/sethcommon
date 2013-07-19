import os    

def getCommonClassesPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "common_classes")

def getCommonPath(baseDir):
    return os.path.join(baseDir, "Common")

def getNotebookCorePath(env):
    return os.path.join(env['NOTEBOOK_ROOT'], "Notebook-Core")
    
def getSCSVPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "SCSV")
    
def getSkyvoOSPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "os")
