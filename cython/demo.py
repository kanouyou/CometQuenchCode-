#!/usr/bin/env python

import sys
import XPostLoading as pt


if __name__=="__main__":
    plot = pt.XPost2dPlot(sys.argv[1], sys.argv[2])
    plot.SetMatInfo(pt.kDose)
    plot.Draw()
