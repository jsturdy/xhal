from django.http import HttpResponse
from django.template import Template, Context
from django.shortcuts import render
from rw_reg import *
from helper_main import *
import threading
import Queue
import timeit


def hello(request):
  reg=getNode("GEM_AMC.GEM_SYSTEM.BOARD_ID")
  print reg
  return HttpResponse('Board ID %s'%(readReg(reg)))

ttclist=[]
triggerlist=[]
triggerohlist=[]
killmask=[]
daqlist=[]
iemask=[]
daqohlist=[]
ohlist=[]

event=threading.Event()
lock = threading.Lock()
monitoringOFF = False

def updateMain():
  global ttclist
  global triggerlist
  global triggerohlist
  global killmask
  global daqlist
  global iemask
  global daqohlist
  global ohlist
  ttclist=getTTCmain()
  triggerlist=getTRIGGERmain()
  triggerohlist=getTRIGGEROHmain()
  killmask=getKILLMASKmain()
  daqlist=getDAQmain()
  iemask=getIEMASKmain()
  daqohlist=getDAQOHmain()
  ohlist=getOHmain()

def updateModule(request, module, q):
  if request.method=="POST":
    if "update" in request.POST:
       regname = 'GEM_AMC.'+module.upper()+'.'+request.POST['update']
       reg = getNode(regname)
       writeReg(reg,int(request.POST[request.POST['update']],16))
  module=module.upper()
  #reglist = getRegsContaining("GEM_AMC."+module)
  reglist = getRegsContaining(module)
# skip chan regs for vfats for the moment
  if "OH.OH" in module:
    reglist = list(r for r in reglist if "ChanReg" not in r.name)
    reglist = list(r for r in reglist if "VFATS.VFAT" not in r.name)
  if "VFATS.VFAT" in module:
    reglist = list(r for r in reglist if "ChanReg" not in r.name)
   
  valuelist = []
  rowcolors = []
  for reg in reglist:
    try: 
      value = readReg(reg)
      valuelist.append(value)
      try: 
        warn_min = reg.warn_min_value
      except AttributeError as ae:
        print "No attribute %s" % (ae)
        warn_min = None
      try: 
        error_min = reg.error_min_value
      except AttributeError as ae:
        print "No attribute %s" % (ae)
        error_min = None
      if (warn_min is not None) or (error_min is not None):
        #print "Error or Warn Value is not None"
        if (warn_min is not None) and (error_min is not None): 
          #print "Error and Warn Value is not None"
          try:
            ivalue = parseInt(value)
            iwarn_min = parseInt(warn_min) 
            ierror_min = parseInt(error_min) 
            if ivalue > ierror_min:
              #print "Value is DANGER"
              rowcolors.append("danger")
            elif ivalue > iwarn_min:
              #print "Value is WARNING"
              rowcolors.append("warning")
            else: 
              #print "Value is correct"
              rowcolors.append("info")
          except ValueError as ve:
            rowcolors.append("info")
            print ve
        elif (error_min is not None):
          #print "Error Value is not None"
          try:
            ivalue = parseInt(value)
            ierror_min = parseInt(error_min) 
            if ivalue > ierror_min:
              #print "Value is DANGER"
              rowcolors.append("danger")
            else: 
              #print "Value is correct"
              rowcolors.append("info")
          except ValueError as ve:
            rowcolors.append("info")
            print ve
        elif (warn_min is not None):
          #print "Warn Value is not None"
          try:
            ivalue = parseInt(value)
            iwarn_min = parseInt(warn_min) 
            if ivalue > iwarn_min:
              rowcolors.append("warning")
            else: 
              #print "Value is correct"
              rowcolors.append("info")
          except ValueError as ve:
            rowcolors.append("info")
            print ve
        else:
          print "WTF?!"
          rowcolors.append("info")
      else:
        rowcolors.append("info")
    except: 
      print "Unexpected error:", sys.exc_info()[0]
      print reg
  forename = "GEM_AMC."+module
  ziplist = zip(list(reg.name[len(forename)+1:] for reg in reglist),valuelist,rowcolors, list(reg.permission for reg in reglist))
  q.put(ziplist)

t1 = threading.Thread(target=updateMain)
t2 = threading.Thread(target=updateModule)

def expert_controls_main_imp(request):
  global monitoringOFF
  if request.method=="POST":
    if "monitoring_off" in request.body:
      monitoringOFF=True
    elif "monitoring_on" in request.body:
      monitoringOFF=False
    elif "daq_reset" in request.body:
      reg = getNode("GEM_AMC.DAQ.CONTROL.RESET")
      writeReg(reg,0x1)
    elif "daq_release" in request.body:
      reg = getNode("GEM_AMC.DAQ.CONTROL.RESET")
      writeReg(reg,0x0)
    elif "daq_enable" in request.body:
      reg = getNode("GEM_AMC.DAQ.CONTROL.DAQ_ENABLE")
      writeReg(reg,0x1)
    elif "daq_disable" in request.body:
      reg = getNode("GEM_AMC.DAQ.CONTROL.DAQ_ENABLE")
      writeReg(reg,0x0)
    elif "block_l1a" in request.body:
      reg = getNode("GEM_AMC.TTC.CTRL.L1A_ENABLE")
      writeReg(reg,0x0)
    elif "unblock_l1a" in request.body:
      reg = getNode("GEM_AMC.TTC.CTRL.L1A_ENABLE")
      writeReg(reg,0x1)
    elif "err_counters_reset" in request.body:
      for i in range(NOH):
        reg = getNode("GEM_AMC.OH.OH%s.COUNTERS.GBT_LINK.TRK_ERR.Reset"%(i))
        writeReg(reg,0x1)
        reg = getNode("GEM_AMC.OH.OH%s.COUNTERS.GTX_LINK.TRK_ERR.Reset"%(i))
        writeReg(reg,0x1)
        reg = getNode("GEM_AMC.OH.OH%s.COUNTERS.GTX_LINK.TRG_ERR.Reset"%(i))
        writeReg(reg,0x1)

def expert_controls_main(request):
  start_time = timeit.default_timer()
  with lock:
    t=threading.Thread(target=expert_controls_main_imp, args = (request,))
    t.start()
    t.join()
  elapsed = timeit.default_timer() - start_time
  print "%s time %s" %(request.POST["id"],elapsed)
  return HttpResponse()

def main(request):
  start_time = timeit.default_timer()
  global ttclist
  global triggerlist
  global triggerohlist
  global killmask
  global daqlist
  global iemask
  global daqohlist
  global ohlist
  global t1
  global lock
  global monitoringOFF
  if monitoringOFF:
    return render(request,'main.html',{'main':True,
                                     'monitoringOFF':True,
                                     'ttclist':ttclist,
                                     'triggerlist':triggerlist,
                                     'triggerohlist':triggerohlist,
                                     'killmask':killmask,
                                     'daqlist':daqlist,
                                     'iemask':iemask,
                                     'daqohlist':daqohlist,
                                     'ohlist':ohlist})
  else:
    with lock:
      t1=threading.Thread(target=updateMain)
      t1.start()
      t1.join()

    elapsed = timeit.default_timer() - start_time
    print "Update main page time %s" %(elapsed)
    return render(request,'main.html',{'main':True,
                                     'monitoringOFF':False,
                                     'ttclist':ttclist,
                                     'triggerlist':triggerlist,
                                     'triggerohlist':triggerohlist,
                                     'killmask':killmask,
                                     'daqlist':daqlist,
                                     'iemask':iemask,
                                     'daqohlist':daqohlist,
                                     'ohlist':ohlist})

def read_fw(request):
  start_time = timeit.default_timer()
  for i in range(1000):
    reg=getNode("GEM_AMC.GEM_SYSTEM.BOARD_ID")
    board_id = readReg(reg)
  exec_time = timeit.default_timer() - start_time
  return HttpResponse('Board ID %s\n Time to read 1000 times %s'%(board_id,exec_time))

def read_gem_system_module(request,module):
  global t1
  global lock
  q = Queue.Queue()
  with lock:
    t1=threading.Thread(target=updateModule, args=[request, module, q])
    t1.start()
    t1.join()
  return render(request,'module.html',{'module':module,'ziplist':q.get()})

