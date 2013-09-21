import os    

def getCommonClassesPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "common_classes")

def getCommonPath(baseDir):
    return os.path.join(baseDir, "Common")

def getDateVersionPath(baseDir):
    return os.path.join(getCommonPath(baseDir), 'date_version')
    
def getDebugNewPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "debug_new")
    
def getMathPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "math")

def getSCSVPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "SCSV")
    
def getSkyvoOSPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "os")

def getRapidJSONPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "rapidjson")
    
def getRapidXMLPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "rapidxml")
