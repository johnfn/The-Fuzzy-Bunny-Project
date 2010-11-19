import sys
import re


def getObjectTable(filename):
    newFile = []
    lines = [l for l in file(filename)] 
    found = True
    for line in lines:
        pattern = re.compile("heap_start:")

        if pattern.search(line):
            found = False
        
        if found:
            newFile.append(line)
    return newFile


def printArr(x):
    print x,

map(printArr, getObjectTable(sys.argv[1]))

