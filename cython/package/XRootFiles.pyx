# filename: XRootFiles.pyx

import matplotlib.pyplot as plt
import numpy as np
import os
import ROOT


## class to access the root files
class XRootFiles:

    ## constructor
    def __init__(self, filename):
        self._thisfile = ROOT.TFile(filename)

    
    ## setup subdirectory name
    def SetSubDirectory(self, name):
        self._subdir = name


    ## find the maximum temperature
    def FindMaxTemp(self):
        if len(self._subdir)==0:
            print "Error: please set the subdirectory first."
            raise
        Tmax = 4.5
        for directory in self._subdir:
            tree = self._thisfile.Get(directory+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if tree.T > Tmax:
                    Tmax = tree.T
        print "find maximum temperature: %.4f over the magnet %s~%s" %(Tmax, self._subdir[0], self._subdir[-1])
        return Tmax


    ## find the minimum temperature
    def FindMinTemp(self):
        if len(self._subdir)==0:
            print "Error: please set the subdirectory first."
            raise
        Tmin = 99999.
        for directory in self._subdir:
            tree = self._thisfile.Get(directory+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if tree.T < Tmin:
                    Tmin = tree.T
        print "find minimum temperature: %.4f over the magnet %s~%s" %(Tmin, self._subdir[0], self._subdir[-1])
        return Tmin




## class to read the root files
class XRootOutput:

    ## constructor
    def __init__(self, path):
        self._directory = path
        self._filename = os.listdir(self._directory)
        if len(self._filename)==0:
            print "Error: no file is founded"
            raise
        

    ## list directory
    def ls(self):
        for eachfile in self._filename:
            print eachfile


    ## 
