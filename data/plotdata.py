#!/usr/bin/env python2.7

from sys import argv
import matplotlib.pyplot as plt
import numpy as np

def load(filename):
    tfile = open(filename, "r")
    dataline = tfile.readlines()
    tfile.close()

    data = {"i":np.array([]), "j":np.array([]), "k":np.array([]), \
            "neutron":np.array([]), "dose":np.array([])}

    for eachline in dataline:
        eachline.strip()
        item = eachline.split()
        data["i"] = np.append(data["i"], int(item[0]))
        data["j"] = np.append(data["j"], int(item[1]))
        data["k"] = np.append(data["k"], int(item[2]))
        data["neutron"] = np.append(data["neutron"], float(item[3]))
        data["dose"] = np.append(data["dose"], float(item[4]))

    return data


if __name__=="__main__":
    data = load(argv[1])
    mshz = max(data["i"])+1
    mshp = max(data["j"])+1
    mshr = max(data["k"])+1

    z = np.array([])
    r = np.array([])
    neu = np.array([])

    for i in range(len(data["j"])):
        if data["j"][i]==1:
            z = np.append(z, data["i"][i])
            r = np.append(r, data["k"][i])
            neu = np.append(neu, data["neutron"][i])

    z = z.reshape((mshz, mshr))
    r = r.reshape((mshz, mshr))
    neu = neu.reshape((mshz, mshr))

    plt.contourf( z, r, neu, 50, cmap=plt.cm.gnuplot2 )
    plt.show()
