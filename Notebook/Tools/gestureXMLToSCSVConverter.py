#Converts a Gesture from and XML to SCSV
#Instructions: copy paste this file into a directory with gesture xml files in them
#It will output all files to SCSVOutput 
import xml.parsers.expat
import glob
import os

scsvSeparator = ","

gestureList = []

currentGesture = None

class Point:
    def __init__(self, x, y, p, t):
        self.x = x
        self.y = y
        self.p = p
        self.t = t
    
    def getSCVSString(self):
        return self.x + scsvSeparator + self.y + scsvSeparator + self.p + "\n"
     
class Stroke:
    def __init__(self, id):
        self.id = id
        self.pointList = []
    
    def addPoint(self, x, y, p, t):
        self.pointList += [Point(x, y, p, t)]
    
    def getStartTime(self):
        return self.pointList[0].t
        
    def getEndTime(self):
        return self.pointList[len(self.pointList)-1].t
    
    def getSCSVString(self):
        ret = self.id + "\n"
        for point in self.pointList:
            ret += point.getSCVSString()
        return ret

class Gesture:
    def __init__(self, name, subject, speed):
        self.gestureName = name
        self.gestureSubject = subject
        self.gestureSpeed = speed
        self.strokeList = []
        self.currentStroke = None
        
    def getTime(self):
        start = self.strokeList[0].getStartTime()
        finish = self.strokeList[len(self.strokeList)-1].getEndTime()
        return int(finish) - int(start)

    def addStroke(self, id):
        if (self.currentStroke != None):
            self.strokeList += [self.currentStroke]

        self.currentStroke = Stroke(id)
        
    def addPoint(self, x, y, p, t):
        self.currentStroke.addPoint(x,y,p,t)
        
    def getSCSVString(self):
        ret = self.gestureName + scsvSeparator + self.gestureSubject + scsvSeparator + self.gestureSpeed + scsvSeparator + str(self.getTime()) + "\n"
        for stroke in self.strokeList:
            ret += stroke.getSCSVString()
        return ret
        

# 3 xml handler functions
def start_element(name, attrs):
    global currentGesture 
    if (name == "Gesture"):
        currentGesture= Gesture(attrs['Name'], attrs['Subject'], attrs['Speed'])
    elif (name == "Stroke"):
        currentGesture.addStroke(attrs["index"])
    elif (name == "Point"):
        currentGesture.addPoint(attrs["X"], attrs["Y"], attrs["Pressure"], attrs["T"])
def end_element(name):
    global currentGesture 
    global gestureList
    if (name == "Gesture"):
        #add a false stroke so that the latest stroke is added
        currentGesture.addStroke("-1")
        gestureList += [currentGesture]
        currentGesture= None

if ( __name__ == "__main__" ):
    fileLocation = "xmlFilesToConvert"
    if (not os.path.isdir(fileLocation)):
        print("Input directory(" + fileLocation + ") does not exist")
        exit(1)

    xmlFiles = glob.glob(os.path.join(fileLocation,"*.xml"))

    for file in xmlFiles:
        xmlString = ""
        inFile = open(file, "r")
        line = inFile.readline()
        while (line != ""):
            xmlString = xmlString + line
            line = inFile.readline()
        inFile.close()
        parser = xml.parsers.expat.ParserCreate()
        parser.StartElementHandler = start_element
        parser.EndElementHandler = end_element
        parser.Parse(xmlString)
    
    outputDir = "convertedScsvGestures"
    if (not os.path.exists(outputDir)):
        os.mkdir(outputDir)
    
    #Output the shiny new scsv files
    for gesture in gestureList:
        outFile = open(os.path.join(outputDir, gesture.gestureName + ".scsv"), "w")
        outFile.write(gesture.getSCSVString())