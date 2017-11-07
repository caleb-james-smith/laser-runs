#!/usr/bin/python
import sys
from sendCommands import *
from argparse import ArgumentParser

control_hub = "hcal904daq04"

cmdLists = []
cmdLists.append([])
cmdLists.append([])
cmdLists[0].append("put RBX-[1-4]-QIE[1-48]_Gsel 192*31")
cmdLists[0].append("put RBX-calib-QIE[1-12]_Gsel 12*31")
cmdLists[1].append("get RBX-[1-4]-QIE[1-48]_Gsel_rr")
cmdLists[1].append("get RBX-calib-QIE[1-12]_Gsel_rr")


def main():
    parser = ArgumentParser()
    parser.add_argument("--rbx",  "-r", default=0, help="RBX to setup")
    options = parser.parse_args()
    rbx = int(options.rbx)
    print "Set shunt 31 for RBX {0}".format(rbx)
    if rbx > 0:
        port = 64000
    else:
        port = 64100
    
    for cmdList in cmdLists:
        commands = list(c.replace("RBX", "HE%d" % rbx) for c in cmdList)
        results = send_commands(cmds=commands,script=True,port=port,control_hub=control_hub)
        for result in results:
            print result["cmd"] + " : " + result["result"] 
        
if __name__ == "__main__":
    sys.exit(main())
