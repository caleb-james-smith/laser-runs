#!/usr/bin/python
import sys
from sendCommands import *
from argparse import ArgumentParser
import time



def main():
    parser = ArgumentParser()
    parser.add_argument("--rbx",    "-r", default=0, help="RBX to setup")
    parser.add_argument("--shunt",  "-s", default=0, help="Shunt setting")
    
    options = parser.parse_args()
    rbx = int(options.rbx)
    shunt = int(options.shunt)
    
    control_hub = "hcal904daq04"
    
    get_rm_shunts = "get RBX-[1-4]-QIE[1-48]_Gsel_rr"
    get_cu_shunts = "get RBX-calib-QIE[1-12]_Gsel_rr"
    
    cmdLists = []
    cmdLists.append([])
    cmdLists.append([])
    cmdLists[0].append("put RBX-[1-4]-QIE[1-48]_Gsel 192*{0}".format(shunt))
    cmdLists[0].append("put RBX-calib-QIE[1-12]_Gsel 12*{0}".format(shunt))
    cmdLists[0].append("tput RBX-lg push")
    cmdLists[1].append(get_rm_shunts)
    cmdLists[1].append(get_cu_shunts)

    rm_shunts = " ".join(4*48*[hex(shunt)])
    cu_shunts = " ".join(1*12*[hex(shunt)])
    print "Setting shunt={0} for RBX {1}".format(shunt, rbx)
    
    if rbx > 0:
        port = 64000
    else:
        port = 64100
    
    for cmdList in cmdLists:
        commands = list(c.replace("RBX", "HE%d" % rbx) for c in cmdList)
        get_rm_shunts = get_rm_shunts.replace("RBX", "HE%d" % rbx)
        get_cu_shunts = get_cu_shunts.replace("RBX", "HE%d" % rbx)
        results = send_commands(cmds=commands,script=True,port=port,control_hub=control_hub)
        if cmdList != cmdLists[-1]: # don't sleep the last time
            time.sleep(1)
        for result in results:
            c = result["cmd"]
            r = result["result"]
            while "quit" in r:
                r = r.split("quit")[0]
            while r[-1] == "\n":
                r = r[:-1]
            r.strip()
            print c + " : " + r
            if c == get_rm_shunts:
                print "Checking result for {0}".format(c)
                #print "result: {0}".format(r)
                #print "expected: {0}".format(rm_shunts)
                if r != rm_shunts:
                    print "returning 1"
                    return 1
            if c == get_cu_shunts:
                r = r[:-1] # remove remaining mystery unseen character
                print "Checking result for {0}".format(c)
                #print "result: {0}".format(r)
                #print "expected: {0}".format(cu_shunts)
                if r != cu_shunts:
                    print "returning 1"
                    print "end of r: " + r[-1:]
                    print "end of cu_shunts: " + cu_shunts[-1:]
                    return 1

    return 0
        
if __name__ == "__main__":
    sys.exit(main())

