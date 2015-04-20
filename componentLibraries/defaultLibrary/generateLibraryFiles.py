#!/usr/bin/python
# Script to generate all library files from teh existing directory structure and compoent hpp file
# Author: Peter Nordin
# Date:  20150319
# $Id: generateLibraryFiles.py 7631 2015-02-06 14:07:58Z petno25 $

import os
import sys
import re

def getAllEmediateSubDirsOf(dirPath,  listOfPaths):
    results = list()
    for fullPath in listOfPaths:
        if fullPath != dirPath and fullPath != '.':
            relpath = os.path.relpath(fullPath, dirPath)
            if not relpath.startswith('..'):
                #print('dirPath: '+dirPath)
                #print('fullPath: '+fullPath)
                #print('relpath: '+relpath)
                results.append(relpath)
    
    results2 = list()
    for result in results:
       dir = result.split('/')[0]
       if dir not in results2:
        results2.append(dir)
    
    #if len(results2) > 0:
        #print(r'Emediate subdirs:'+results2.join())
    return results2
    
def generateRootCCFile(dirPath,  filename,  componentHeaders, componentCCIFiles):
    filepath = os.path.join(dirPath,filename)
    file = open(filepath,  'w+')
    if not file.closed:
        print('Generating: '+filepath)
        file.write("//This file has been automatically generated!\n") 
        for header in componentHeaders:
            file.write(r'#include "'+header+'"\n')       
        file.write('\n')
        for cci in componentCCIFiles:
            file.write(r'#include "'+cci+'"\n')  
        file.write('\n')
    file.close()

#def generateCCFileForComponentHeader(dirPath, filename, componentHeader):
#    filepath = os.path.join(dirPath,filename)
#    file = open(filepath,  'w+')
#    if not file.closed:
#        print('Generating: '+filepath)
#        file.write("//This file has been automatically generated!\n")        
#        file.write(r'#include "'+componentHeader+r'"')
#        file.write("\n")
#    file.close()

def generateCCIFileForComponentRegistration(dirPath, filename, componentTypeNames):
    filepath = os.path.join(dirPath,filename)
    file = open(filepath,  'w+')
    if not file.closed:
        print('Generating: '+filepath)
        file.write("//This file has been automatically generated!\n")
        for typeName in componentTypeNames:
            file.write(r'pComponentFactory->registerCreatorFunction("'+typeName+r'",'+typeName+r'::Creator);'+"\n")
    file.close()
    return filepath

def generateHeaderFileForComponentHeaderInclusion(dirPath, filename, copmonentHeaders):
    filepath = os.path.join(dirPath,filename)
    file = open(filepath,  'w+')
    if not file.closed:
        print('Generating: '+filepath)
        file.write("//This file has been automatically generated!\n")
        for header in copmonentHeaders:
            file.write(r'#include "'+header+'"\n')
    file.close()
    return filepath

def generatePRI(dirPath, filename, includeFiles, componentHeaders, componentSrcFiles, componentCCIFiles):
    filepath = os.path.join(dirPath,filename)
    file = open(filepath,  'w+')
    if not file.closed:
        print('Generating: '+filepath)

        file.write("#This file has been automatically generated!\n")
        for includeFile in includeFiles:
            file.write(r'include($${PWD}/'+includeFile+r')'+"\n")
        file.write("\n")
        file.write("\n")

        file.write(r'HEADERS += ')
        for header in componentHeaders:
            file.write(r' \ '+'\n'+r' $${PWD}/'+header)
        file.write("\n")
        file.write("\n")

        file.write(r'SOURCES += ')
        for source in componentSrcFiles:
            file.write(r' \ '+"\n"+r' $${PWD}/'+source)
        file.write("\n")
        file.write("\n")

        file.write(r'OTHER_FILES += ')
        for other in componentCCIFiles:
            file.write(r' \ '+"\n"+r' $${PWD}/'+other)
        file.write("\n")
        file.write("\n")
    file.close()
    return filepath
    
#def generateSubdirsPRO(dirPath, filename, subdirs, componentHeaders, componentSrcFiles, componentCCIFiles):
#    filepath = os.path.join(dirPath,filename)
#    file = open(filepath,  'w+')
#    if not file.closed:
#        print('Generating: '+filepath)
#
#        file.write("#This file has been automatically generated!\n")
#        file.write("TEMPLATE = subdirs\n")
#        file.write("SUBDIRS = ")
#        for subdir in subdirs:
#            file.write(subdir+' ')
#        file.write("\n")
#
#        file.write(r'HEADERS += ')
#        for header in componentHeaders:
#            file.write(r' \ '+'\n'+r' $${PWD}/'+header)
#        file.write("\n")
#
#        file.write(r'SOURCES += ')
#        for source in componentSrcFiles:
#            file.write(r' \ '+"\n"+r'$${PWD}/'+source)
#        file.write("\n")
#
#        file.write(r'OTHERS += ')
#        for other in componentCCIFiles:
#            file.write(r' \ '+"\n"+r'$${PWD}/'+other)
#        file.write("\n")
#    file.close()
#    return filepath



def findFiles(rootDir, suffixes, excludeDirs):
    files = list()
    for dirpath, dirnames, filenames in os.walk(rootDir):
        print(dirpath)
        enterDir = True
        for d in excludeDirs:
            if d in dirpath:
                enterDir=False
        if enterDir:
            for filename in filenames:
                name, ext = os.path.splitext(filename)
#                print(r'name: '+name)
#                print(r'ext: '+ext)
#                print(r'suffixes: '+suffixes)
                if ext != '' and ext in suffixes:
                    filepath = os.path.join(dirpath,filename)
                    print(r'Found match:'+filepath)
                    files.append(filepath)
    return files

def checkTypeName(filename):
    typename = ''
    file = open(filename,  'r')
    if not file.closed:
        # Read until line starts with class
        for line in file:
            line = line.strip()
            if line.startswith('class'):
                if line.endswith('ComponentQ') or line.endswith('ComponentC') or line.endswith('ComponentSignal') or line.endswith('ComponentSystem'):
                    try:
                        found = re.search('class(.+?):', line).group(1)
                    except AttributeError:
                        found = ''
                    typename = found.strip()
                    if not typename == '':
                        break
    file.close()
    return typename

class ComponentDir:
    def __init__(self):
        self.dirPath = ''
        self.dirName = ''
        self.compHeaders = list()
        self.compSources = list()
        self.typeNames = list()
        self.cciFile = ''
        self.headerFile = ''

    def isEmpty(self):
        if self.dirPath == '':
            return True
        else:
            return False

def main(rootDir):
    suffixes = list()
    suffixes.append('.hpp')
    excludeDirs = list()
    excludeDirs.append('Dependencies')
    
    files = findFiles(rootDir, suffixes, excludeDirs)

    currentDir = ComponentDir()    
    componentDirs = list()

    for filepath in files:
        fileFullDir, fileName = os.path.split(filepath)
        dummy, fileDir = os.path.split(fileFullDir)
        print("\n")
        print('filepath:     '+filepath)
        print('fileFullDir:  '+fileFullDir)
        print('fileName:     '+fileName) 
        print('fileDir:      '+fileDir)
        if currentDir.dirPath != fileFullDir:
            # Append last dir
            if not currentDir.isEmpty():
                print('Appending componentDir entery 1')
                componentDirs.append(currentDir)                
    
            print('Creating new ComppnentDir entery')
            currentDir = ComponentDir() 
            currentDir.dirPath = fileFullDir
            currentDir.dirName = fileDir
            

        typename = checkTypeName(filepath)
        if not typename == '':
            print('Typename:     '+typename)
            fileBaseName, fileExt = os.path.splitext(fileName)
            print('fileBaseName: '+fileBaseName)
            print('fileExt:      '+fileExt)
            currentDir.typeNames.append(typename)
            currentDir.compHeaders.append(fileName)
            #currentDir.compSources.append(fileBaseName+'.cc')
            #generateCCFileForComponentHeader(fileFullDir, fileBaseName+'.cc', fileName)

    if not currentDir.isEmpty():
        print('Appending componentDir entery 2')
        componentDirs.append(currentDir) 

    print (len(componentDirs))
    
    allComDirPaths = list()
    for compDir in componentDirs:
        allComDirPaths.append(compDir.dirPath)
        
    for compDir in componentDirs:
        emediates = getAllEmediateSubDirsOf(compDir.dirPath,  allComDirPaths)
        emediateHeaders =list()
        emediateCCI = list()
        for emediate in emediates:
            emediateHeaders.append(emediate+'/'+emediate+'.h')
            emediateCCI.append(emediate+'/'+emediate+'.cci')
        
        compDir.cciFile = os.path.basename(generateCCIFileForComponentRegistration(compDir.dirPath, compDir.dirName+r'.cci', compDir.typeNames ))
        compDir.headerFile = os.path.basename(generateHeaderFileForComponentHeaderInclusion(compDir.dirPath, compDir.dirName+r'.h', compDir.compHeaders+emediateHeaders ))
         
    # Generate root level files
    emediates = getAllEmediateSubDirsOf(rootDir,  allComDirPaths)
    emediateHeaders =list()
    emediateCCI = list()
    for emediate in emediates:
        emediateHeaders.append(emediate+'/'+emediate+'.h')
        emediateCCI.append(emediate+'/'+emediate+'.cci')
    generateHeaderFileForComponentHeaderInclusion(rootDir, 'Components.cci', emediateCCI)
    generateHeaderFileForComponentHeaderInclusion(rootDir, 'Components.h', emediateHeaders)
    
    allHeaders = list()
    allCCI = list()
    for compDir in componentDirs:
        allHeaders.append(compDir.dirPath+'/'+compDir.dirName+'.h')
        allCCI.append(compDir.dirPath+'/'+compDir.dirName+'.cci')
        for compHeader in compDir.compHeaders:
            allHeaders.append(compDir.dirPath+'/'+compHeader)
    allHeaders.append('Components.h')   
    allCCI.append('Components.cci')
    
    generatePRI(rootDir, 'Components.pri', list(), allHeaders, list(),  allCCI)
    
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('Error: You must give at least one argument, the root dir')
        exit()
    else:
        rootDir = sys.argv[1]
    
    main(rootDir)
