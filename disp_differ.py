import sys
import re


def getObjectTable(filename):
    newFile = []
    lines = [l for l in file(filename)] 
    foundDispatch = False
    for line in lines:
        pattern = re.compile("._dispTab:")
        word_pattern = re.compile("\.word")
        if not word_pattern.search(line):
            foundDispatch = False

        if pattern.search(line):
            foundDispatch = True
        
        if foundDispatch:
            newFile.append(line)
    return newFile


def printArr(x):
    print x,

map(printArr, getObjectTable(sys.argv[1]))

