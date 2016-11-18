# filename: XRootFiles.pyx

import matplotlib.pyplot as plt
import numpy as np
import os
import ROOT


## class to access the root files
class XRootFiles:

    ## constructor
    def __init__(self, filename):
        self.tfile = ROOT.TFile(filename)




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
