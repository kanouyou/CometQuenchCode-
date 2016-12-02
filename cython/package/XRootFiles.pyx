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

    ## find the maximum value
    def FindMax(self, opt="B"):
        if len(self._subdir)==0:
            print "Error: please set the subdirectory first."
            raise
        Vmax = -99999
        for directory in self._subdir:
            tree = self._thisfile.Get(directory+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if opt=="B":
                    if tree.B > Vmax:
                        Vmax = tree.B
                elif opt=="R":
                    if tree.R > Vmax:
                        Vmax = tree.R
                elif opt=="Q":
                    if tree.Q > Vmax:
                        Vmax = tree.Q
        print "---------------------------------------------------------------"
        print "find maximum value: %.4f over the magnet %s~%s" %(Vmax, self._subdir[0], self._subdir[-1])
        print "---------------------------------------------------------------"
        return Vmax

    ## find the minimum value
    def FindMin(self, opt="B"):
        if len(self._subdir)==0:
            print "Error: please set the subdirectory first."
            raise
        Vmin = 99999
        for directory in self._subdir:
            tree = self._thisfile.Get(directory+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if opt=="B":
                    if tree.B < Vmin:
                        Vmin = tree.B
                elif opt=="R":
                    if tree.R < Vmin:
                        Vmin = tree.R
                elif opt=="Q":
                    if tree.Q < Vmin:
                        Vmin = tree.Q
        print "---------------------------------------------------------------"
        print "find minimum value: %.4f over the magnet %s~%s" %(Vmin, self._subdir[0], self._subdir[-1])
        print "---------------------------------------------------------------"
        return Vmin

    ## find the maximum temperature
    def FindMaxTemp(self):
        if len(self._subdir)==0:
            print "Error: please set the subdirectory first."
            raise
        Tmax = 4.5
        TmaxId = np.array([0, 0, 0])
        for directory in self._subdir:
            TmaxCoil = 4.5
            tree = self._thisfile.Get(directory+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if tree.T > Tmax:
                    Tmax = tree.T
                if tree.T > TmaxCoil:
                    TmaxCoil = tree.T
                    TmaxId[0] = tree.id[0]
                    TmaxId[1] = tree.id[1]
                    TmaxId[2] = tree.id[2]
            print "maximum temperature of %s: %.4f [K] at (%i,%i,%i)" %(directory, TmaxCoil, TmaxId[0], TmaxId[1], TmaxId[2])
        print "---------------------------------------------------------------"
        print "find maximum temperature: %.4f over the magnet %s~%s" %(Tmax, self._subdir[0], self._subdir[-1])
        print "---------------------------------------------------------------"
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
        print "accessing directory: %s" %path
        #self._filename = os.listdir(self._directory)
        #self._filename.sort()
        self._filename  = []
        for i in range(len(os.listdir(self._directory))):
            self._filename.append("qchout%i.root" %i)
        self._id = np.array([0, 0, 0])
        self._coil = "CS1"
        if len(self._filename)==0:
            print "Error: no file is founded"
            raise
        
    ## list directory
    def ls(self):
        for eachfile in self._filename:
            print eachfile

    ## setup id fot output information
    def SetCoilPosition(self, coilname, z, r, phi):
        self._coil = coilname
        self._id[0] = z
        self._id[1] = r
        self._id[2] = phi
        self._legname = coilname
        #self._legname = "%s: (z=%i,phi=%i,r=%i)" %(self._coil,self._id[0],self._id[1],self._id[2])

    ## get given information from the output
    def GetInfo(self):
        info = {"time": np.array([]), "T": np.array([]), "V": np.array([]), "I": np.array([])}
        # get the time information
        for eachfile in self._filename:
            thisfile = ROOT.TFile(self._directory+"/"+eachfile)
            tree = thisfile.Get("head")
            tree.GetEntry(0)
            info["time"] = np.append(info["time"], tree.time)
            info["I"] = np.append(info["I"], tree.I)
        # get the information from material conatainer for the given position id
        cnt = 0
        for eachfile in self._filename:
            thisfile = ROOT.TFile(self._directory+"/"+eachfile)
            tree = thisfile.Get(self._coil+"/tree")
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                if tree.id[0]==self._id[0] and tree.id[1]==self._id[1] and tree.id[2]==self._id[2]:
                    info["T"] = np.append(info["T"], tree.T)
                    info["V"] = np.append(info["V"], tree.R * info["I"][cnt])
                    break
            cnt += 1
        return info

    ## calculate each coil voltage
    def GetCoilVoltage(self, coil):
        data = {"t":np.array([]), "V":np.array([]), "I":np.array([])}
        for eachfile in self._filename:
            thisfile = ROOT.TFile(self._directory+"/"+eachfile)
            tree = thisfile.Get("head")
            tree.GetEntry(0)
            data["t"] = np.append(data["t"], tree.time)
            data["I"] = np.append(data["I"], tree.I)

        cnt = 0
        for eachfile in self._filename:
            thisfile = ROOT.TFile(self._directory+"/"+eachfile)
            tree = thisfile.Get(coil+"/tree")
            V = 0.
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                V += (tree.R * data["I"][cnt])
            data["V"] = np.append(data["V"], V)
            #print "%i, %.3f sec, %.3e V" %(cnt, data["t"][cnt], data["V"][cnt])
            cnt += 1
        return data

    ## plot coil voltage
    def PlotCoilVoltage(self):
        fig, ax = plt.subplots(1, 1,figsize=(10, 6))
        coil = ["CS0", "CS1", "MS1", "MS2"]
        for eachcoil in coil:
            data = self.GetCoilVoltage(eachcoil)
            ax.plot(data["t"], data["V"], linewidth=2, marker=None, label=eachcoil)
        ax.set_xlabel("Time [sec]", fontsize=15)
        ax.set_ylabel("Voltage [V]", fontsize=15)
        ax.tick_params(axis="both", labelsize=15)
        ax.legend(loc="upper right")
        plt.savefig("CoilVolt.pdf")

    ## set output file
    def SetOutput(self, filename="currdat.txt"):
        self._wfile = open(filename, "w")

    ## write out the maximum data
    def Write(self):
        data = self.GetInfo()
        for i in range(len(data["T"])):
            self._wfile.write( "%.3f  %.3f  %.3f  %.3e\n" %(data["time"][i], data["I"][i], data["T"][i], data["V"][i]) )

    ## close the file
    def Close(self):
        self._wfile.close()

    ## construct figure
    def ConstructFigure(self, length=10, height=6):
        fig, self._ax = plt.subplots(1, 1, figsize=(length, height))

    ## plot current together
    def TwinPlot(self):
        data = self.GetInfo()
        self._ax.plot(data["time"], data["I"], linewidth=2, marker=None, color="k")
        self._ax.set_xlabel("Time [sec]", fontsize=15)
        self._ax.set_ylabel("Current [A]", fontsize=15)
        self._ax.tick_params(axis="both", labelsize=15)
        self._ax = self._ax.twinx()

    ## return the plor editor
    def GetPlotEdit(self):
        return self._ax

    ## plot
    def Plot(self, opt="T", label=""):
        data = self.GetInfo()
        self._ax.plot(data["time"], data[opt], linewidth=2, marker=None, label=label)

