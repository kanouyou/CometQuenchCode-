## check the data output

import numpy as np
import matplotlib.pyplot as plt
import XPostLoading as pt

class XPostOutput:

    ## constructor
    def __init__(self, geofile, matfile):
        self._geof = pt.XGeoFileLoad(geofile)
        self._matf = pt.XMatFileLoad(matfile)
        self._phi  = 1


    ## setup phi for output data
    def SetPhi(self, phi=1):
        self._phi = phi


    ## return the set phi
    def GetPhi(self):
        return self._phi


    ## get maximum temperature
    def GetMaxTemp(self):
        maxtemp = 0.
        node = 0
        for mat in self._matf.GetCollection():
            if mat.GetTemperature() > maxtemp:
                maxtemp = mat.GetTemperature()
                node = mat.GetNode()
        return node, maxtemp


    ## get minimum RRR
    def GetMinRRR(self, geom):
        maxrrr = 999999.
        node = 0
        for mat in self._matf.GetCollection():
            if mat.GetRRR() < maxrrr and self._geof.GetCollect(mat.GetNode()).GetGeometry()==geom:
            #if mat.GetRRR() < maxrrr:
                maxrrr = mat.GetRRR()
                node = mat.GetNode()
        return node, maxrrr


    ## print max temperature info
    def Print(self):
        node, maxtemp = self.GetMaxTemp()
        idz = self._geof.GetCollect(node).GetId(pt.kZ)
        idp = self._geof.GetCollect(node).GetId(pt.kPhi)
        idr = self._geof.GetCollect(node).GetId(pt.kR)
        z   = self._geof.GetCollect(node).GetPosition(pt.kZ)
        p   = self._geof.GetCollect(node).GetPosition(pt.kPhi)
        r   = self._geof.GetCollect(node).GetPosition(pt.kR)
        print "/////////////////////////////////////"
        print "// Maximum Temperature"
        print "/////////////////////////////////////"
        print " node id: %i" %node
        print " temperature: %.4f [K]" %maxtemp
        print " id: (%i, %i, %i)" %(idz, idp, idr)
        print " position: (%.2f, %.2f, %.2f)" %(z, p, r)
        print " min. RRR conductor: %.2f, strip: %.2f" %(self.GetMinRRR(0)[1], self.GetMinRRR(1)[1])


    ##
