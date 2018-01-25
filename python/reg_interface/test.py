import xml.etree.ElementTree as xml
import sys, os, subprocess
from time import sleep
from ctypes import *
import timeit
from rw_reg import parseXML, readReg

from gempython.tools.vfat_user_functions_xhal import *

lib = CDLL(os.getenv("XHAL_ROOT")+"/lib/x86_64/librpcman.so")

addrTable = os.getenv("XHAL_ROOT")+'/etc/gem_amc_top.xml'
nodes = parseXML(addrTable)
#nodes = parseXML()

rpc_connect = lib.init
rpc_connect.argtypes = [c_char_p]
rpc_connect.restype = c_uint

update_atdb = lib.update_atdb
update_atdb.argtypes = [c_char_p]
update_atdb.restype = c_uint

ttcGenConf = lib.ttcGenConf
ttcGenConf.restype = c_uint
ttcGenConf.argtypes = [c_uint, c_uint, c_uint, c_uint, c_uint, c_uint, c_bool]

genScan = lib.genScan
genScan.restype = c_uint
genScan.argtypes = [c_uint, c_uint, c_uint, c_uint, 
                    c_uint, c_uint, c_uint, c_uint, 
                    c_char_p, c_bool, c_bool, POINTER(c_uint32)]

rReg = lib.getReg
rReg.restype = c_uint
rReg.argtypes=[c_uint]

broadcastWrite = lib.broadcastWrite
broadcastWrite.argtypes = [c_uint, c_char_p, c_uint, c_uint]
broadcastWrite.restype = c_uint

DEBUG = True

def main():
    print "Start testing"
    start_time_ = timeit.default_timer()
    #rpc_connect("eagle60")
    vfatBoard = HwVFAT(5, 0, 3, False)
    elapsed_ = timeit.default_timer() - start_time
    print "Connect time %s" %(elapsed_)
    #print "Connection to eagle60 successful"
    
    #addrTable_CTP7 = "/mnt/persistent/texas/gem_amc_top.xml"
    #print "updating address table to: %s"%(addrTable_CTP7)
    #update_atdb(addrTable_CTP7)
    #print "Address table updated"

    #mask = 0x30
    mask = 0xFFB3F3

    chan=128
    nevts=20
    scanmin=41
    scanmax=41
    stepSize=1
    scanDataSizeVFAT = (scanmax-scanmin+1)/stepSize
    scanDataSizeNet = scanDataSizeVFAT * 24
    scanData = (c_uint32 * scanDataSizeNet)()
        
    scanReg = "LATENCY"

    #genScan(nevts, 0, scanmin, scanmax, stepSize, chan, False, mask, scanReg, False, True, scanData)
    rpcResp = vfatBoard.parentOH.performCalibrationScan(chan=chan, scanReg=scanReg, outData=scanData, enableCal=False, nevts=nevts, 
                                                        dacMin=scanmin, dacMax=scanmax, stepSize=stepSize, mask=mask, useUltra=False, useExtTrig=True)

    if rpcResp != 0:
        print("latency scan failed")
        sys.exit(os.EX_SOFTWARE)
    print("Done scanning, processing output")
    print "vfatN, lat, N_Evt, N_Fired"
    for vfat in range(0,24):
        if (mask >> vfat) & 0x1: continue
        for latReg in range(vfat*scanDataSizeVFAT,(vfat+1)*scanDataSizeVFAT):
            lat = scanmin + (latReg - vfat*scanDataSizeVFAT) * stepSize
            N_GoodEvt = scanData[latReg] & 0xffff
            N_ChanFire = (scanData[latReg]>>16) & 0xffff
            print vfat, lat, N_GoodEvt, N_ChanFire
            pass

    return

if __name__ == '__main__':
  start_time = timeit.default_timer()
  # code you want to evaluate
  main()
  elapsed = timeit.default_timer() - start_time
  print "Total time %s" %(elapsed)
