import os    

def getCppUTestPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "CppUTest")

def getTestMain(baseDir):
    return(os.path.join(getCppUTestPath(baseDir), "sethTestMain.cpp"))

def getCommonClassesPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "common_classes")

def getCommonPath(baseDir):
    return os.path.join(baseDir, "Common")

def getDateVersionPath(baseDir):
    return os.path.join(getCommonPath(baseDir), 'date_version')
    
def getDebugNewPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "debug_new")

def getGMockPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "gmock")

def getMathPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "math")

def getSCSVPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "SCSV")
    
def getOSPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "os")

def getRapidJSONPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "rapidjson")
    
def getRapidXMLPath(baseDir):
    return os.path.join(getCommonPath(baseDir), "rapidxml")
