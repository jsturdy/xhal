#!/bin/env python
"""
Script to take Scurve data using OH ultra scans
By: Cameron Bravo (c.bravo@cern.ch)
"""

import sys, os, random, time
from array import array
from vfat_functions_uhal import *
from optparse import OptionParser
#from ROOT import TFile,TTree

parser = OptionParser()

parser.add_option("-s", "--slot", type="int", dest="slot",
                  help="slot in uTCA crate", metavar="slot", default=10)
parser.add_option("-g", "--gtx", type="int", dest="gtx",
                  help="GTX on the GLIB", metavar="gtx", default=0)
parser.add_option("--nglib", type="int", dest="nglib",
                  help="Number of register tests to perform on the glib (default is 100)", metavar="nglib", default=100)
parser.add_option("--noh", type="int", dest="noh",
                  help="Number of register tests to perform on the OptoHybrid (default is 100)", metavar="noh", default=100)
parser.add_option("--ni2c", type="int", dest="ni2c",
                  help="Number of I2C tests to perform on the VFAT2s (default is 100)", metavar="ni2c", default=100)
parser.add_option("--ntrk", type="int", dest="ntrk",
                  help="Number of tracking data packets to readout (default is 100)", metavar="ntrk", default=100)
parser.add_option("--writeout", action="store_true", dest="writeout",
                  help="Write the data to disk when testing the rate", metavar="writeout")
parser.add_option("--tests", type="string", dest="tests",default="A,B,C,D,E",
                  help="Tests to run, default is all", metavar="tests")
parser.add_option("-f", "--filename", type="string", dest="filename", default="SCurveData.root",
                  help="Specify Output Filename", metavar="filename")
parser.add_option("-d", "--debug", action="store_true", dest="debug",
                  help="print extra debugging information", metavar="debug")

(options, args) = parser.parse_args()
link = options.gtx

import subprocess,datetime
startTime = datetime.datetime.now().strftime("%d.%m.%Y-%H.%M.%S.%f")
print startTime
Date = startTime


t0 = time.time()
#amc = rpc_connect("eagle33")
parseXML()

t1 = time.time()
print "Connection to CTP7 and parsing XML took %s" %(t1-t0)
filename = options.filename
#myF = TFile(filename,'recreate')
#myT = TTree('scurveTree','Tree Holding CMS GEM SCurve Data')
#
#Nev = array( 'i', [ 0 ] )
#Nev[0] = 1000
#myT.Branch( 'Nev', Nev, 'Nev/I' )
#vcal = array( 'i', [ 0 ] )
#myT.Branch( 'vcal', vcal, 'vcal/I' )
#Nhits = array( 'i', [ 0 ] )
#myT.Branch( 'Nhits', Nhits, 'Nhits/I' )
#vfatN = array( 'i', [ 0 ] )
#myT.Branch( 'vfatN', vfatN, 'vfatN/I' )
#vfatCH = array( 'i', [ 0 ] )
#myT.Branch( 'vfatCH', vfatCH, 'vfatCH/I' )
#trimRange = array( 'i', [ 0 ] )
#myT.Branch( 'trimRange', trimRange, 'trimRange/I' )
#vthr = array( 'i', [ 0 ] )
#myT.Branch( 'vthr', vthr, 'vthr/I' )
#trimDAC = array( 'i', [ 0 ] )
#myT.Branch( 'trimDAC', trimDAC, 'trimDAC/I' )

t2 = time.time()
print "Tree init took %s" %(t2-t1)

SCURVE_MIN = 0
SCURVE_MAX = 254

N_EVENTS = 1000
CHAN_MIN = 0
CHAN_MAX = 128
mask = 0

setTriggerSource(0,link,1)
configureLocalT1(0, link, 1, 0, 40, 250, 0, options.debug)
startLocalT1(0, link)
print "Local T1 Status: %s"%(readRegister(0,"GEM_AMC.OH.OH%d.T1Controller.MONITOR"%(link)))

#bias vfats
biasAllVFATs(0,link,0x0,enable=False)
writeAllVFATs(0, link, "VThreshold1", 70, 0)


writeAllVFATs(0, link, "Latency",    37, mask)
writeAllVFATs(0, link, "ContReg0",    0x37, mask)
writeAllVFATs(0, link, "ContReg2",    48, mask)

t3 = time.time()
print "Init VFATs took %s" %(t3-t2)

#for scCH in range(CHAN_MIN,CHAN_MAX):
#    #writeAllVFATs(0,link,"VFATChannels.ChanReg%d.MASK"%(scCH),0x0,mask)
#    for i in range(24):
#        writeVFAT(0,link,i,"VFATChannels.ChanReg%d.MASK"%(scCH),0x0,mask)
#disable CalPulses in all channels and all VFATs
for scCH in range(CHAN_MIN,CHAN_MAX):
    writeAllVFATs(0,link,"VFATChannels.ChanReg%d"%(scCH),0x0,mask)
    #for i in range(24):
    #    #writeVFAT(0,link,i,"VFATChannels.ChanReg%d.PULSE"%(scCH),0x0,mask)
    #    #trimVal = (0x3f & readVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH)))
    #    writeVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH),0)


t4 = time.time()
print "Disabling PULSE at all channels took %s" %(t4-t3)
#sys.exit()

configureScanModule(0, link, 3, mask, channel = 0, scanmin = SCURVE_MIN, scanmax = SCURVE_MAX, numtrigs = int(N_EVENTS), useUltra = True, debug = options.debug)

for scCH in range(CHAN_MIN,CHAN_MAX):
    t5 = time.time()
    vfatCH[0] = scCH
    print "Channel #"+str(scCH)
    writeAllVFATs(0,link,"VFATChannels.ChanReg%d"%(scCH),0x40,mask)
    #for i in range(24):
    #    #trimVal = (0x3f & readVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH)))
    #    writeVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH),0x40)
    writeRegister(0,"GEM_AMC.OH.OH%d.ScanController.ULTRA.CHAN"%(link), scCH)
    #configureScanModule(0, link, 3, mask, channel = scCH, scanmin = SCURVE_MIN, scanmax = SCURVE_MAX, numtrigs = int(N_EVENTS), useUltra = True, debug = options.debug)
    #printScanConfiguration(0, link, useUltra = True, debug = options.debug)
    startScanModule(0, link, useUltra = True, debug = options.debug)
    tX = time.time()
    scanData = getUltraScanResults(0, link, SCURVE_MAX - SCURVE_MIN + 1, False)
    tY = time.time()
    print "getData took %s" %(tY-tX)
    #for i in range(0,24):
    #    vfatN[0] = i
    #    dataNow = scanData[i]
    #    #trimDAC[0] = (0x1f & readVFAT(0,link, i,"VFATChannels.ChanReg%d"%(scCH)))
    #    trimDAC[0] = 0
    #    #trimDAC[0] = readVFAT(0,link, i,"VFATChannels.ChanReg%d.TRIM"%(scCH))
    #    #vthr[0] = (0xff & readVFAT(0,link, i,"VThreshold1"))
    #    vthr[0] = 70
    #    for VC in range(SCURVE_MIN,SCURVE_MAX+1):
    #        vcal[0] = int((dataNow[VC] & 0xff000000) >> 24)
    #        Nhits[0] = int(dataNow[VC] & 0xffffff)
    #        myT.Fill()
    #writeAllVFATs(0,link,"VFATChannels.ChanReg%d.PULSE"%(scCH),0x0,mask)
    writeAllVFATs(0,link,"VFATChannels.ChanReg%d"%(scCH),0x00,mask)
    #for i in range(24):
    #    #writeVFAT(0,link,i,"VFATChannels.ChanReg%d.PULSE"%(scCH),0x0,mask)
    #    #trimVal = (0x3f & readVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH)))
    #    writeVFAT(0,link,i,"VFATChannels.ChanReg%d"%(scCH),0)
    t6 = time.time()
    print "One point took %s" %(t6-t5)

stopLocalT1(0, link)
writeAllVFATs(0, link, "ContReg0",    0, mask)

#myF.cd()
#myT.Write()
#myF.Close()

t7 = time.time()
print "Total execution time %s" %(t7-t0)






