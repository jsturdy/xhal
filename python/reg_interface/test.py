import xml.etree.ElementTree as xml
import sys, os, subprocess
from time import sleep
from ctypes import *
import timeit

lib = CDLL(os.getenv("XHAL_ROOT")+"/lib/x86_64/librwreg.so")
rReg = lib.getReg
rReg.restype = c_uint
rReg.argtypes=[c_uint]
wReg = lib.putReg
wReg.argtypes=[c_uint,c_uint]
rpc_connect = lib.init
rpc_connect.argtypes = [c_char_p]
rpc_connect.restype = c_uint
update_atdb = lib.update_atdb
update_atdb.argtypes = [c_char_p]
update_atdb.restype = c_uint
getRegInfo = lib.getRegInfoDB
getRegInfo.argtypes = [c_char_p]
getRegInfo.restype = c_uint
confTRIMDAC = lib.configureTRIMDAC
confTRIMDAC.argtypes = [c_char_p, c_char_p]
confTRIMDAC.restype = c_uint
confVT1 = lib.configureVT1
confVT1.argtypes = [c_char_p, c_char_p, c_uint]
confVT1.restype = c_uint
confVFATs = lib.configureVFATs
confVFATs.argtypes = [c_char_p, c_char_p, c_char_p, c_uint, c_uint]
confVFATs.restype = c_uint
getTTCmain = lib.getmonTTCmain
getTTCmain.argtypes = [POINTER(c_uint32)]
getTTCmain.restype = c_uint
getTRIGGERmain = lib.getmonTRIGGERmain
getTRIGGERmain.argtypes = [POINTER(c_uint32), c_uint]
getTRIGGERmain.restype = c_uint
rBlock = lib.getBlock
rBlock.restype = c_uint
rBlock.argtypes=[c_uint,POINTER(c_uint32)]

rList = lib.getList
rList.restype = c_uint
rList.argtypes=[POINTER(c_uint32),POINTER(c_uint32)]

DEBUG = True

def main():
  print "Start testing"
  start_time_ = timeit.default_timer()
  rpc_connect("eagle34")
  elapsed_ = timeit.default_timer() - start_time
  print "Connect time %s" %(elapsed_)
  print "Connection to eagle34 successful"
  getRegInfo("GEM_AMC.GEM_SYSTEM.BOARD_ID")
  res = (c_uint32 * 5)()
  res_code = getTTCmain(res)
  print "\ngetTTCmain result code: %s" %(res_code)
  if res_code == 0:
    print "getTTCmain result: %s" %(res)
    for c in res:
      print c
  noh = 2
  res2 = (c_uint32 * (noh+1))()
  res_code = getTRIGGERmain(res2,noh)
  if res_code == 0:
    print "getTRIGGERmain result: %s" %(res2)
    for c in res2:
      print c
  update_atdb("/mnt/persistent/texas/gem_amc_top.xml")
  print "Address table updated"
  #getRegInfo("GEM_AMC.GEM_SYSTEM.BOARD_ID")
  #start_time_ = timeit.default_timer()
  #res = confTRIMDAC('2','/mnt/persistent/texas/test/chConfig_GEMINIm01L1.txt')
  #elapsed_ = timeit.default_timer() - start_time_
  #print "Config time %s" %(elapsed_)
  #if res == 0:
  #  print "confTRIMDAC executed"
  #else:
  #  print "confTRIMDAC failed"

  #start_time_ = timeit.default_timer()
  #res = confVT1('2','',64)
  #elapsed_ = timeit.default_timer() - start_time_
  #print "Config time %s" %(elapsed_)
  #if res == 0:
  #  print "confVT1 executed"
  #else:
  #  print "confVT1 failed"

  #start_time_ = timeit.default_timer()
  #res = confVFATs('2','/mnt/persistent/texas/test/chConfig_GEMINIm01L1.txt','/mnt/persistent/texas/test/vfatConfig_GEMINIm27L1.txt',1,64)
  #elapsed_ = timeit.default_timer() - start_time_
  #print "Config time %s" %(elapsed_)
  #if res == 0:
  #  print "confVFATs executed"
  #else:
  #  print "confVFATs failed"


if __name__ == '__main__':
  start_time = timeit.default_timer()
  # code you want to evaluate
  main()
  elapsed = timeit.default_timer() - start_time
  print "Total time %s" %(elapsed)
