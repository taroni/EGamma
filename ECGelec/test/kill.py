#!/bin/env python

from os import popen

#numero iniziale
i=774423810
  

#inizio ciclo
while i<774423959:
    out = popen(" echo \"bkill "+str(i)+" \" ")
    for x in out:
        print x
    out=popen("bkill "+str(i)+"" )
    for x in out:
        print x
    i+=1
