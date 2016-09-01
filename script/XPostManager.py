
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors

## class to contain the data
class XPostContainer:

    ## constructor
    def __init__(self):
        self._pos = np.array([0.,0.,0.])
        self._id  = np.array([0,0,0])
        self._T   = 0.
        self._B   = 0.
        self._RRR = 0.
        self._C   = 0.
        self._k   = np.array([0.,0.,0.])
        self._dose = 0.

    ## setup position
    def SetPosition(self, z, phi, r):
        self._pos[0] = z
        self._pos[1] = phi
        self._pos[2] = r

    ## return position
    def GetPosition(self, cor=None):
        if cor==None:
            return self._pos
        elif cor=="z" or cor=="Z":
            return self._pos[0]
        elif cor=="phi" or cor=="Phi":
            return self._pos[1]
        elif cor=="r" or cor=="R":
            return self._pos[2]
        else:
            raise

    ## setup mesh id
    def SetId(self, z, phi, r):
        self._id[0] = z
        self._id[1] = phi
        self._id[2] = r

    ## return mesh id
    def GetId(self, cor=None):
        if cor==None:
            return self._id
        elif cor=="z" or cor=="Z":
            return self._id[0]
        elif cor=="phi" or cor=="Phi":
            return self._id[1]
        elif cor=="r" or cor=="R":
            return self._id[2]
        else:
            raise

    ## setup heat capacity
    def SetCapacity(self, C):
        self._C = C

    ## return heat capacity
    def GetCapacity(self):
        return self._C

    ## setup thermal conductivity
    def SetConductivity(self, kz, kp, kr):
        self._k[0] = kz
        self._k[1] = kp
        self._k[2] = kr

    ## return thermal conductivity
    def GetConductivity(self, cor=None):
        if cor==None:
            return self._k
        elif cor=="z" or cor=="Z":
            return self._k[0]
        elif cor=="phi" or cor=="Phi":
            return self._k[1]
        elif cor=="r" or cor=="R":
            return self._k[2]
        else:
            raise

    ## setup temperature
    def SetTemperature(self, T):
        self._T = T

    ## return temperature
    def GetTemperature(self):
        return self._T

    ## setup magnetic field
    def SetField(self, B):
        self._B = B

    ## return magnetic field
    def GetField(self):
        return self._B

    ## setup RRR
    def SetRRR(self, RRR):
        self._RRR = RRR

    ## return RRR
    def GetRRR(self):
        return self._RRR

    ## setup dose
    def SetDose(self, dose):
        self._dose = dose

    ## return dose
    def GetDose(self):
        return self._dose


class XPostLoading:

    ## constructor
    def __init__(self, filename):
        self._mesh = {"z":0, "phi":0, "r":0}
        print "loading file: %s ... " %filename
        tfile = open(filename, "r")
        dataline = tfile.readlines()
        tfile.close()
        self.initial(dataline)
        self.checkmesh("z")
        self.checkmesh("p")
        self.checkmesh("r")

    ## put the data into structure
    def initial(self, data):
        self._collect = np.array([])
        for eachline in data:
            eachline.strip()
            item = eachline.split()
            vec = XPostContainer()
            vec.SetId( int(item[0]), int(item[1]), int(item[2]) )
            vec.SetPosition( float(item[3]), float(item[4]), float(item[5]) )
            vec.SetTemperature( float(item[6]) )
            vec.SetRRR( float(item[7]) )
            vec.SetField( float(item[8]) )
            vec.SetCapacity( float(item[9]) )
            vec.SetConductivity( float(item[10]), float(item[11]), float(item[12]) )
            vec.SetDose( float(item[13]) )
            self._collect = np.append(self._collect, vec)

    ## check the mesh
    def checkmesh(self, cor):
        name = ""
        if cor=="z" or cor=="Z":
            name = "z"
        elif cor=="p" or cor=="Phi":
            name = "phi"
        elif cor=="r" or cor=="R":
            name = "r"
        else:
            raise
        for collect in self._collect:
            if self._mesh[name]<collect.GetId(name):
                self._mesh[name] = collect.GetId(name) - 1

    ## return mesh
    def GetMesh(self):
        return self._mesh

    ## return collect
    def GetCollect(self):
        return self._collect



class XPostManager:

    ## constructor
    def __init__(self, filename):
        data = XPostLoading(filename)
        self._mesh = data.GetMesh()
        self._data = data.GetCollect()
        self._phi  = 1

    ## check this id is over range
    def is_overrange(self, num):
        over = False
        size = (self._mesh["z"]+2) * (self._mesh["phi"]+2) * (self._mesh["r"]+2)
        if num>=size:
            over = True
        return over

    ## return the id number
    def Id(self, z, phi, r):
        idnum = r*(self._mesh["z"]+2)*(self._mesh["phi"]+2) + phi*(self._mesh["z"]+2) + z
        if self.is_overrange(idnum):
            raise
        return idnum

    ## setup the piece along phi direction
    def SetPhi(self, num):
        self._phi = num

    ## plot magnetic field
    def plot_field(self, fig, ax):
        print "ploting field..."
        data = {"z":np.array([]), "r":np.array([]), "B":np.array([])}
        for i in range(len(self._data)):
            if self._data[i].GetId("phi")==self._phi and \
               self._data[i].GetId("z") > 0 and self._data[i].GetId("z") < self._mesh["z"]+1 and \
               self._data[i].GetId("r") > 0 and self._data[i].GetId("r") < self._mesh["r"]+1:
                data["z"] = np.append( data["z"], self._data[i].GetPosition("z") )
                data["r"] = np.append( data["r"], self._data[i].GetPosition("r") )
                data["B"] = np.append( data["B"], self._data[i].GetField() )
        # reshape data
        data["z"] = data["z"].reshape((self._mesh["r"], self._mesh["z"]))
        data["r"] = data["r"].reshape((self._mesh["r"], self._mesh["z"]))
        data["B"] = data["B"].reshape((self._mesh["r"], self._mesh["z"]))
        # plot
        cs = ax.contourf( data["z"], data["r"], data["B"], 50, cmap=plt.cm.gnuplot2 )
        cbar = fig.colorbar( cs, ax=ax, shrink=0.9 )
        cbar.set_label("Magnetic Field [Tesla]")
        ax.set_xlabel("Z [m]")
        ax.set_ylabel("R [m]")

    ## plot heat capacity
    def plot_capacity(self, fig, ax):
        print "ploting capacity..."
        data = {"z":np.array([]), "r":np.array([]), "C":np.array([])}
        for i in range(len(self._data)):
            if self._data[i].GetId("phi")==self._phi and \
               self._data[i].GetId("z") > 0 and self._data[i].GetId("z") < self._mesh["z"]+1 and \
               self._data[i].GetId("r") > 0 and self._data[i].GetId("r") < self._mesh["r"]+1:
                data["z"] = np.append( data["z"], self._data[i].GetPosition("z") )
                data["r"] = np.append( data["r"], self._data[i].GetPosition("r") )
                data["C"] = np.append( data["C"], self._data[i].GetCapacity() )
        # reshape data
        data["z"] = data["z"].reshape((self._mesh["r"], self._mesh["z"]))
        data["r"] = data["r"].reshape((self._mesh["r"], self._mesh["z"]))
        data["C"] = data["C"].reshape((self._mesh["r"], self._mesh["z"]))
        # plot
        cs = ax.contourf( data["z"], data["r"], data["C"], 50, cmap=plt.cm.gnuplot2 )
        cbar = fig.colorbar( cs, ax=ax, shrink=0.9 )
        cbar.set_label("Heat Capacity [J/kg/K]")
        ax.set_xlabel("Z [m]")
        ax.set_ylabel("R [m]")

    ## plot heat capacity
    def plot_conductivity(self, fig, ax, opt="z"):
        print "ploting conductivity along %s direction..." %opt
        data = {"z":np.array([]), "r":np.array([]), "k":np.array([])}
        for i in range(len(self._data)):
            if self._data[i].GetId("phi")==self._phi and \
               self._data[i].GetId("z") > 0 and self._data[i].GetId("z") < self._mesh["z"]+1 and \
               self._data[i].GetId("r") > 0 and self._data[i].GetId("r") < self._mesh["r"]+1:
                data["z"] = np.append( data["z"], self._data[i].GetPosition("z") )
                data["r"] = np.append( data["r"], self._data[i].GetPosition("r") )
                data["k"] = np.append( data["k"], self._data[i].GetConductivity(opt) )
        # reshape data
        data["z"] = data["z"].reshape((self._mesh["r"], self._mesh["z"]))
        data["r"] = data["r"].reshape((self._mesh["r"], self._mesh["z"]))
        data["k"] = data["k"].reshape((self._mesh["r"], self._mesh["z"]))
        # plot
        cs = ax.contourf( data["z"], data["r"], data["k"], \
                          #norm=colors.LogNorm(vmin=data["k"].min(), vmax=data["k"].max()), \
                          50, cmap=plt.cm.gnuplot2 )
        cbar = fig.colorbar( cs, ax=ax, shrink=0.9 )
        cbar.set_label("Thermal Conductivity [W/m/K]")
        ax.set_title("%s direction" %opt)
        ax.set_xlabel("Z [m]")
        ax.set_ylabel("R [m]")

    ## plot RRR
    def plot_RRR(self, fig, ax):
        print "ploting residual resistance ratio..."
        data = {"z":np.array([]), "r":np.array([]), "RRR":np.array([])}
        for i in range(len(self._data)):
            if self._data[i].GetId("phi")==self._phi and \
               self._data[i].GetId("z") > 0 and self._data[i].GetId("z") < self._mesh["z"]+1 and \
               self._data[i].GetId("r") > 0 and self._data[i].GetId("r") < self._mesh["r"]+1:
                data["z"] = np.append( data["z"], self._data[i].GetPosition("z") )
                data["r"] = np.append( data["r"], self._data[i].GetPosition("r") )
                data["RRR"] = np.append( data["RRR"], self._data[i].GetRRR() )
        # reshape data
        data["z"] = data["z"].reshape((self._mesh["r"], self._mesh["z"]))
        data["r"] = data["r"].reshape((self._mesh["r"], self._mesh["z"]))
        data["RRR"] = data["RRR"].reshape((self._mesh["r"], self._mesh["z"]))
        # plot
        cs = ax.contourf( data["z"], data["r"], data["RRR"], 50, cmap=plt.cm.gnuplot2 )
        cbar = fig.colorbar( cs, ax=ax, shrink=0.9 )
        cbar.set_label("RRR")
        ax.set_xlabel("Z [m]")
        ax.set_ylabel("R [m]")

    ## plot energy deposit
    def plot_dose(self, fig, ax):
        print "ploting energy deposition..."
        data = {"z":np.array([]), "r":np.array([]), "dose":np.array([])}
        for i in range(len(self._data)):
            if self._data[i].GetId("phi")==self._phi and \
               self._data[i].GetId("z") > 0 and self._data[i].GetId("z") < self._mesh["z"]+1 and \
               self._data[i].GetId("r") > 0 and self._data[i].GetId("r") < self._mesh["r"]+1:
                data["z"] = np.append( data["z"], self._data[i].GetPosition("z") )
                data["r"] = np.append( data["r"], self._data[i].GetPosition("r") )
                data["dose"] = np.append( data["dose"], self._data[i].GetDose() )
        # reshape data
        data["z"] = data["z"].reshape((self._mesh["r"], self._mesh["z"]))
        data["r"] = data["r"].reshape((self._mesh["r"], self._mesh["z"]))
        data["dose"] = data["dose"].reshape((self._mesh["r"], self._mesh["z"]))
        # plot
        cs = ax.contourf( data["z"], data["r"], data["dose"], 50, cmap=plt.cm.gnuplot2 )
        cbar = fig.colorbar( cs, ax=ax, shrink=0.9 )
        cbar.set_label("Energy Deposit [W/kg]")
        ax.set_xlabel("Z [m]")
        ax.set_ylabel("R [m]")


    ## plot
    def Plot(self):
        fig, ax = plt.subplots(3,2,figsize=(12,7))
        #fig.tight_layout(w_pad=1.7, h_pad=1.0)
        self.plot_field(fig, ax[0][0])
        self.plot_capacity(fig, ax[0][1])
        self.plot_conductivity(fig, ax[1][0], "r")
        self.plot_conductivity(fig, ax[1][1], "z")
        self.plot_RRR(fig, ax[2][0])
        self.plot_dose(fig, ax[2][1])
        plt.show()
