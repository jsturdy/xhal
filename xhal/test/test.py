import xhalpy as xi
import os

def w1(str):
    print (str)
    wait = raw_input()
    return

print os.getpid()
#w1('starting main..press a key')
def test_log_dups():
  eagle64_1=xi.XHALDevice("eagle64","/opt/cmsgemos/etc/maps/amc_address_table_top.xml")
  eagle64_2=xi.XHALDevice("eagle64","/opt/cmsgemos/etc/maps/amc_address_table_top.xml")
def test_xhaldevice():
  print "TEST OF XHALDEVICE WRAPPER"
  eagle64=xi.XHALDevice("eagle64","/opt/cmsgemos/etc/maps/amc_address_table_top.xml")
  print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  print "read reg by address: eagle64.readReg(0x66400008)=%08x"%(eagle64.readReg(0x66400008))
  print "Now write 0xfeed to GEM_AMC.GEM_SYSTEM.BOARD_ID"
  eagle64.writeReg("GEM_AMC.GEM_SYSTEM.BOARD_ID", 0xfeed)
  print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  print "Now write back 0xbeef to GEM_AMC.GEM_SYSTEM.BOARD_ID"
  eagle64.writeReg("GEM_AMC.GEM_SYSTEM.BOARD_ID", 0xbeef)
  print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  print "\n"
  return

def test_xhalDaqMon():
  print "TEST OF XHALDAQMON WRAPPER"
  eagle64Mon=xi.DaqMonitor("eagle64")
  print "testing getmonTTCmain"
  res = eagle64Mon.getmonTTCmain()
  print "getMonTTCmain function executed, let's check the result"
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "testing getmonDAQmain"
  res = eagle64Mon.getmonDAQmain()
  print "getMonDAQmain function executed, let's check the result"
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  
  print "testing overloads"
  print "result of getmonTRIGGERmain(1):"
  res=eagle64Mon.getmonTRIGGERmain(1)
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "result of getmonTRIGGERmain(2):"
  res=eagle64Mon.getmonTRIGGERmain(2)
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "result of getmonTRIGGERmain():"
  res=eagle64Mon.getmonTRIGGERmain()
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "result of getmonOHmain(1):"
  res=eagle64Mon.getmonOHmain(1)
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "result of getmonOHmain():"
  res=eagle64Mon.getmonOHmain()
  for i,c in enumerate(res):
    print "res[%i] = %i" %(i,c)
  print "\n"
  return

def test_xhalrpc_exception():
  print "TEST OF XHAL RPC EXCEPTIONS HANDLING"
  eagle64=xi.XHALDevice("eagle64","/opt/cmsgemos/etc/maps/amc_address_table_top.xml")
  print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  print "Now disconnect"
  eagle64.disconnect()
  print "RPC service disconnected. Now try to read register by name again"
  try:
    print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  except xi.XHALRPCNotConnectedException as e:
    print "Caught exception: %s" %(e)
  print "Now reconnect"
  eagle64.reconnect()
  print "RPC service reconnected. Now try to read register by name again"
  try:
    print "read reg by name: eagle64.readReg(GEM_AMC.GEM_SYSTEM.BOARD_ID)=%08x"%(eagle64.readReg("GEM_AMC.GEM_SYSTEM.BOARD_ID"))
  except xi.XHALRPCNotConnectedException as e:
    print "Caught exception: %s" %(e)
  print "Test reconnection attempt on connected service"
  try:
    eagle64.reconnect()
  except xi.XHALRPCException as e:
    print "Caught exception: %s" %(e)

def update_lmdb():
  eagle=xi.Utils("eagle34")
  eagle.update_atdb("/mnt/persistent/gemdaq/xml/gem_amc_top_v1_15_2_12oh_no_top_node.xml")
    

test_xhaldevice()
test_xhalDaqMon()
test_xhalrpc_exception()
test_log_dups()
print "All tests are completed"
