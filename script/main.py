#!/usr/bin/env python2.7

from sys import argv
import XPostManager

if __name__=="__main__":
    post = XPostManager.XPostManager(argv[1])
    post.Plot()
