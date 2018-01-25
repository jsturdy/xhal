from rw_reg import *
NOH=1

def getTTCmain():
  namelist=['MMCM_LOCKED','TTC_SINGLE_ERROR_CNT','BC0_LOCKED','L1A_ID','L1A_RATE']
  res = (c_uint32 * 5)()
  res_code = getRPCTTCmain(res)
  if res_code == 0:
    values = [c for c in res]
  else:
    values = [0,0,0,0,0]
  displaystring=[]
  if values[0]:
    displaystring.append('<span class="label label-success">YES</span>')
  else:
    displaystring.append('<span class="label label-danger">NO</span>')
  value = values[1]
  if value:
    displaystring.append('<div class="progress"><div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="65535" style="min-width: 3em; width:%s%%">%s</div></div>' % (value,value/655.45,value))
  else:
    displaystring.append('<div class="progress"><div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="65535" style="min-width: 3em;">%s</div></div>' % (value,value))
  value = values[2]
  if value:
    displaystring.append('<span class="label label-success">YES</span>')
  else:
    displaystring.append('<span class="label label-danger">NO</span>')
  displaystring.append('<span class="label label-info">%s</span>' % (values[3]))
  reg = getNode('GEM_AMC.TTC.L1A_RATE')
  displaystring.append('<span class="label label-info">%s Hz</span>' % (values[4]))
  return zip(namelist,displaystring) 
 
def getTRIGGERmain():
  values=[]
  namelist=['OR_TRIGGER_RATE',]
  displaystring=[]
  res = (c_uint32 * (NOH+1))()
  res_code = getRPCTRIGGERmain(res, NOH)
  if res_code == 0:
    values = [c for c in res]
  else:
    for i in range(NOH+1):
      values.append(0)
  #reg = getNode('GEM_AMC.TRIGGER.STATUS.OR_TRIGGER_RATE')
  #value=int(readReg(reg),16)
  value = values[0]
  if value>5000000:
    width=100
  else:
    width=value/50000
  if value>1000000:
    displaystring.append('<div class="progress"><div class="progress-bar progress-bar-danger" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="min-width: 3em; width:%s%%">%sHz</div></div>' % (value,width,value))
  elif value>100000:
    displaystring.append('<div class="progress"><div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="min-width: 3em; width:%s%%">%sHz</div></div>' % (value,width,value))
  else:
    displaystring.append('<div class="progress"><div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="width:%s%%">%sHz</div></div>' % (value,width,value))
  for i in range(NOH):
    namelist.append('OH%s.TRIGGER_RATE' % (i))
    #reg = getNode('GEM_AMC.TRIGGER.OH%s.TRIGGER_RATE' % (i))
    #value=int(readReg(reg),16)
    value = values[i+1]
    if value>1000000:
      displaystring.append('<div class="progress"><div class="progress-bar progress-bar-danger" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="min-width: 3em; width:%s%%">%sHz</div></div>' % (value,width,value))
    elif value>100000:
      displaystring.append('<div class="progress"><div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="min-width: 3em; width:%s%%">%sHz</div></div>' % (value,width,value))
    else:
      displaystring.append('<div class="progress"><div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="5000000" style="min-width: 3em; width:%s%%">%sHz</div></div>' % (value,width,value))

  return zip(namelist,displaystring) 
 
def getKILLMASKmain():
  killmask=[]
  reg = getNode('GEM_AMC.TRIGGER.CTRL.OH_KILL_MASK')
  value='{0:010b}'.format(int(readReg(reg),16)) # should be same length as NOH
  for v in value[::-1]:
    if int(v):
      killmask.append('disabled')
    else:
      killmask.append('success')
  return killmask

def getTRIGGEROHmain():
  displaystring=[]
  namelist=[]
  values = []
  res = (c_uint32 * (2*NOH))()
  res_code = getRPCTRIGGEROHmain(res, NOH)
  if res_code == 0:
    values = [c for c in res]
  else:
    for i in range(2*NOH):
      values.append(0)

  nextstr = ''
  for i in range(NOH):
    nextstr+='<td>%s</td>' % (i)
  namelist.append('Register|OH')
  displaystring.append(nextstr)
  nextstr = ''
  namelist+=['LINK0_NOT_VALID_CNT',
             'LINK1_NOT_VALID_CNT',]
  for i,regname in enumerate(namelist[1:]):
    for j in range(NOH):
      #reg=getNode('GEM_AMC.TRIGGER.OH%s.%s' %(i,regname))
      nextstr+='<td><span class="label label-info">%s</span></td>' % (values[i*NOH+j])
    displaystring.append(nextstr)
    nextstr = ''

  return zip(namelist,displaystring) 

def getDAQmain():
  namelist=['DAQ_ENABLE',
            'DAQ_LINK_READY',
            'DAQ_LINK_AFULL',
            'DAQ_OFIFO_HAD_OFLOW',
            'L1A_FIFO_HAD_OFLOW',]
  fullnamelist=[['GEM_AMC.DAQ.CONTROL.DAQ_ENABLE','YES','NO','success','warning'],
                ['GEM_AMC.DAQ.STATUS.DAQ_LINK_RDY','YES','NO','success','warning'],
                ['GEM_AMC.DAQ.STATUS.DAQ_LINK_AFULL','YES','NO','warning','success'],
                ['GEM_AMC.DAQ.STATUS.DAQ_OUTPUT_FIFO_HAD_OVERFLOW','YES','NO','danger','success'],
                ['GEM_AMC.DAQ.STATUS.L1A_FIFO_HAD_OVERFLOW','YES','NO','danger','success']]

  res = (c_uint32 * 9)()
  res_code = getRPCDAQmain(res)
  if res_code == 0:
    values = [c for c in res]
  else:
    values = [0,0,0,0,0,0,0,0,0]
  displaystring=[]
  for i,regname in enumerate(fullnamelist):
    #reg = getNode(regname[0])
    #if int(readReg(reg),16):
    if values[i]:
      displaystring.append('<td><span class="label label-%s">%s</span></td>' % (regname[3],regname[1]))
    else:
      displaystring.append('<td><span class="label label-%s">%s</span></td>' % (regname[4],regname[2]))
  namelist.append('L1A_FIFO_DATA_COUNT')
  #reg = getNode('GEM_AMC.DAQ.EXT_STATUS.L1A_FIFO_DATA_CNT')
  #value=int(readReg(reg),16)
  value = values[5]
  displaystring.append('<td><div class="progress"><div class="progress-bar progress-bar-info" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="8192" style="min-width: 3em; width:%s%%">%s</div></div></td>' % (value,value/81.92,value))
  namelist.append('DAQ_FIFO_DATA_COUNT')
  #reg = getNode('GEM_AMC.DAQ.EXT_STATUS.DAQ_FIFO_DATA_CNT')
  #value=int(readReg(reg),16)
  value=values[6]
  displaystring.append('<td><div class="progress"><div class="progress-bar progress-bar-info" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="8192" style="min-width: 3em; width:%s%%">%s</div></div></td>' % (value,value/81.92,value))
  namelist.append('EVENT_SENT')
  #reg = getNode('GEM_AMC.DAQ.EXT_STATUS.EVT_SENT')
  #value=int(readReg(reg),16)
  value=values[7]
  displaystring.append('<td><div class="progress"><div class="progress-bar progress-bar-info" role="progressbar" aria-valuenow="%s" aria-valuemin="0" aria-valuemax="4294967295" style="min-width: 3em; width:%s%%">%s</div></div></td>' % (value,value/4294967295,value))
  namelist.append('TTS_STATE')
  #reg = getNode('GEM_AMC.DAQ.STATUS.TTS_STATE')
  #value=int(readReg(reg),16)
  value=values[8]
  if value==8:
    displaystring.append('<td><span class="label label-success" style="min-width:5em;">READY</span></td>')
  elif value==1:
    displaystring.append('<td><span class="label label-info" style="min-width:5em;">BUSY</span></td>')
  elif value==2:
    displaystring.append('<td><span class="label label-danger" style="min-width:5em;">ERROR</span></td>')
  elif value==3:
    displaystring.append('<td><span class="label label-warning" style="min-width:5em;">WARN</span></td>')
  elif value==4:
    displaystring.append('<td><span class="label label-primary" style="min-width:5em;">OOS</span></td>')
  else:
    displaystring.append('<td><span class="label label-default" style="min-width:5em;">NDF</span></td>')

  return zip(namelist,displaystring) 

def getIEMASKmain():
  iemask=[]
  reg = getNode('GEM_AMC.DAQ.CONTROL.INPUT_ENABLE_MASK')
  value='{0:010b}'.format(int(readReg(reg),16)) # should be same length as NOH
  for v in value[::-1]:
    if int(v):
      iemask.append('success')
    else:
      iemask.append('disabled')
  return iemask

def getDAQOHmain():
  displaystring=[]
  res = (c_uint32 * (6*NOH))()
  values = []
  res_code = getRPCDAQOHmain(res, NOH)
  if res_code == 0:
    values = [c for c in res]
  else:
    for i in range(6*NOH):
      values.append(0)
 
  namelist=[]
  nextstr = ''
  for i in range(NOH):
    nextstr+='<td>%s</td>' % (i)
  namelist.append('Register|OH')
  displaystring.append(nextstr)
  nextstr = ''
  namelist+=['EVT_SIZE_ERR',
             'EVENT_FIFO_HAD_OFLOW',
             'INPUT_FIFO_HAD_OFLOW',
             'INPUT_FIFO_HAD_UFLOW',
             'VFAT_TOO_MANY',
             'VFAT_NO_MARKER',]
  for i,regname in enumerate(namelist[1:]):
    for j in range(NOH):
      #reg=getNode('GEM_AMC.DAQ.OH%s.STATUS.%s' %(i,regname))
      #if int(readReg(reg),16):
      if values[i*NOH+j]:
        nextstr+='<td><span class="label label-danger">Y</span></td>'
      else:
        nextstr+='<td><span class="label label-success">N</span></td>'
    displaystring.append(nextstr)
    nextstr = ''

  return zip(namelist,displaystring) 

def getOHmain():
  displaystring=[]
  res = (c_uint32 * (NOH*7))()
  values = []
  res_code = getRPCOHmain(res, NOH)
  if res_code == 0:
    values = [c for c in res]
  else:
    for i in range(NOH*7):
      values.append(0)
 
  namelist=[]
  nextstr = ''
  for i in range(NOH):
    nextstr+='<td>%s</td>' % (i)
  namelist.append('Register|OH')
  displaystring.append(nextstr)
  nextstr = ''
  namelist+=['FW_VERSION',
             'EVENT_COUNTER',
             'EVENT_RATE',
             'GTX.TRK_ERR',
             'GTX.TRG_ERR',
             'GBT.TRK_ERR',
             'CORR_VFAT_BLK_CNT',]
  fullnamelist=['GEM_AMC.OH.OH%s.STATUS.FW',
                 'GEM_AMC.DAQ.OH%s.COUNTERS.EVN',
                 'GEM_AMC.DAQ.OH%s.COUNTERS.EVT_RATE',
                 'GEM_AMC.OH.OH%s.COUNTERS.GTX_LINK.TRK_ERR',
                 'GEM_AMC.OH.OH%s.COUNTERS.GTX_LINK.TRG_ERR',
                 'GEM_AMC.OH.OH%s.COUNTERS.GBT_LINK.TRK_ERR',
                 'GEM_AMC.DAQ.OH%s.COUNTERS.CORRUPT_VFAT_BLK_CNT',]

  for i,regname in enumerate(fullnamelist):
    for j in range(NOH):
      #reg=getNode(regname % (i))
      #if 'FW' in regname:
      #  value = readReg(reg)[2:]
      #else:
      #	try:
      #	  value = int(readReg(reg),16)
      #	except ValueError as e:
      #	  value = -1
      value = values[i*NOH+j]
      if value==-1:
        nextstr+='<td><span class="label label-danger">%s</span></td>' % (value)
      elif not 'EVENT' in regname and value>0 and not 'FW' in regname:
        nextstr+='<td><span class="label label-warning">%s</span></td>' % (value)
      elif 'FW' in regname:
        #if 'Error' in value:
        if 0xdeaddead==value:
          nextstr+='<td><span class="label label-danger">ERROR</span></td>' 
        else:
          nextstr+='<td><span class="label label-info">%08X</span></td>' % (value)
      else:
        nextstr+='<td><span class="label label-info">%s</span></td>' % (value)
    displaystring.append(nextstr)
    nextstr = ''

  return zip(namelist,displaystring) 
