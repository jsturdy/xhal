#!/bin/env python
"""
Script to measure the DAC values on the VFATs
By: Jared Sturdy (sturdy@cern.ch), Mykhailo Dalchenko (mykhailo.dalchenko@cern.ch)
"""

import sys
from array import array
from vfat_functions_uhal import *
from optparse import OptionParser

parser = OptionParser()

parser.add_option("--filename", type="string", dest="filename", default="DACData.dat",
                  help="Specify Output Filename", metavar="filename")
parser.add_option("-g", "--gtx", type="int", dest="gtx",
                  help="GTX on the AMC", metavar="gtx", default=0)
parser.add_option("-n", "--nevents", type="int", dest="nevents",
                  help="Number of events per point", metavar="nevents", default=10)

(options, args) = parser.parse_args()
filename = options.filename
import subprocess,datetime,time
startTime = datetime.datetime.now().strftime("%Y.%m.%d.%H.%M")
print startTime
Date = startTime

t0 = time.time()
parseXML()

t1 = time.time()
print "Parsing XML took %s" %(t1-t0)

#ohboard = getOHObject(options.slot,options.gtx,options.shelf,options.debug)
ohboard = 0

N_EVENTS = options.nevents
dacmode = {
    #"IPREAMPIN"   : [1, None, 0,"IPreampIn"],
    #"IPREAMPFEED" : [2, None, 0,"IPreampFeed"],
    #"IPREAMPOUT"  : [3, None, 0,"IPreampOut"],
    #"ISHAPER"     : [4, None, 0,"IShaper"],
    #"ISHAPERFEED" : [5, None, 0,"IShaperFeed"],
    #"ICOMP"       : [6, None, 0,"IComp"],
    #"VTHRESHOLD1" : [7, None, 1,"VThreshold1"],
    #"VTHRESHOLD2" : [8, None, 1,"VThreshold2"],
    "VCAL"        : [9, None, 1,"VCal"],
    "CALOUTVLOW"  : [10,2,    1,"VCal"],
    "CALOUTVHI"   : [10,1,    1,"VCal"],
}

adcReg = {
    0:["VAUX.VAL_4", "VAUX.VAL_1"],
    1:["VAUX.VAL_6", "VAUX.VAL_5"],
    2:["VAUX.VAL_13","VPVN"],
}

final_dict = {}
for dactype in dacmode.keys():
    final_dict[dactype] = []

try:
    # calibration correction seems to already be applied
    # for i in range(4):
    #     oldval = readRegister(ohboard,"GEM_AMC.OH.OH%d.ADC.CONTROL.CONF_REG.CAL%d"%(options.gtx,i))
    #     writeRegister(ohboard,"GEM_AMC.OH.OH%d.ADC.CONTROL.CONF_REG.CAL%d"%(options.gtx,i),0x1)
    #     newval = readRegister(ohboard,"GEM_AMC.OH.OH%d.ADC.CONTROL.CONF_REG.CAL%d"%(options.gtx,i))
    #     print "CAL%d old/new = 0x%x/0x%x"%(i,oldval,newval)
    #     pass
    for dactype in dacmode.keys():
        print "Obtaining DAC scan for %s"%(dactype)
        sys.stdout.flush()
        dac = dacmode[dactype]
        for i in range(8):
            for col in range(3):
                writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0", 0x37)
                print "VFAT%d ContReg0 0x%x"%(((col*8)+i),readVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0"))
                pass
            cr0val = []
            for col in range(3):
                writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg1", dac[0])
                if dac[1]:
                    cr0val.append(readVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0"))
                    writeval = cr0val[col]|(dac[1]<<6)
                    writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0", writeval)
                    pass
            for val in range(256):
                for col in range(3):
                    writeVFAT(ohboard, options.gtx, ((col*8)+i), dacmode[dactype][3], val)
                    pass
                for sample in range(N_EVENTS):
                    for col in range(3):
                        rawval       = readRegister(ohboard,"GEM_AMC.OH.OH%d.ADC.%s"%(options.gtx,adcReg[col][dacmode[dactype][2]]))
                        event = [col*8+i,val,rawval>>6]
                        final_dict[dactype].append(event)
                        pass
                    pass
                pass
            for col in range(3):
                if dac[1]:
                    writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0", cr0val[col])
                    pass
                pass
        for col in range(3):
            writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg0", 0x0)
            writeVFAT(ohboard, options.gtx, ((col*8)+i), "ContReg1", 0x0)
            pass
        sys.stdout.flush()
        with open(filename,"a") as f:
            f.write("%s\n"%({dactype:final_dict[dactype]}))
        final_dict[dactype]=[]
        pass

except Exception as e:
    print "An exception occurred", e
finally:
    t2 = time.time()
    print "Scan took %s" %(t2-t1)
    print "Total execution time %s" %(t2-t0)
    pass
