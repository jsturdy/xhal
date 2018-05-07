import sys, os, subprocess, socket
from time import sleep
from ctypes import *
import cPickle as pickle
import gc
from collections import OrderedDict

hostname = socket.gethostname()
if 'eagle' in hostname:
  lib = CDLL(os.getenv("GEM_PATH")+"/lib/librwreg.so")
  rReg = lib.getReg
  rReg.restype = c_uint
  rReg.argtypes=[c_uint]
  wReg = lib.putReg
  wReg.argtypes=[c_uint,c_uint]
  ADDRESS_TABLE_TOP = os.getenv("GEM_PATH")+'/xml/gem_amc_top.xml'
else:
  lib = CDLL(os.getenv("XHAL_ROOT")+"/lib/x86_64/librpcman.so")
  rReg = lib.getReg
  rReg.restype = c_uint
  rReg.argtypes=[c_uint]
  wReg = lib.putReg
  wReg.argtypes=[c_uint,c_uint]
  rpc_connect = lib.init
  rpc_connect.argtypes = [c_char_p]
  rpc_connect.restype = c_uint
  rBlock = lib.getBlock
  rBlock.restype = c_uint
  rBlock.argtypes=[c_uint,POINTER(c_uint32)]
  getRPCTTCmain = lib.getmonTTCmain
  getRPCTTCmain.argtypes = [POINTER(c_uint32)]
  getRPCTTCmain.restype = c_uint
  
  getRPCTRIGGERmain = lib.getmonTRIGGERmain
  getRPCTRIGGERmain.argtypes = [POINTER(c_uint32), c_uint32]
  getRPCTRIGGERmain.restype = c_uint
  
  getRPCTRIGGEROHmain = lib.getmonTRIGGEROHmain
  getRPCTRIGGEROHmain.argtypes = [POINTER(c_uint32), c_uint32]
  getRPCTRIGGEROHmain.restype = c_uint
  
  getRPCDAQmain = lib.getmonDAQmain
  getRPCDAQmain.argtypes = [POINTER(c_uint32)]
  getRPCDAQmain.restype = c_uint
  
  getRPCDAQOHmain = lib.getmonDAQOHmain
  getRPCDAQOHmain.argtypes = [POINTER(c_uint32), c_uint32]
  getRPCDAQOHmain.restype = c_uint
  
  getRPCOHmain = lib.getmonOHmain
  getRPCOHmain.argtypes = [POINTER(c_uint32), c_uint32]
  getRPCOHmain.restype = c_uint
  
  rList = lib.getList
  rList.restype = c_uint
  rList.argtypes=[POINTER(c_uint32),POINTER(c_uint32)]

  update_atdb = lib.update_atdb
  update_atdb.argtypes = [c_char_p]
  update_atdb.restype = c_uint

  ADDRESS_TABLE_TOP = os.getenv("XHAL_ROOT")+'/etc/gem_amc_top.xml'


DEBUG = True
nodes = OrderedDict()

class Node:
    name = ''
    description = ''
    vhdlname = ''
    address = 0x0
    real_address = 0x0
    permission = ''  
    mask = 0x0
    isModule = False
    parent = None
    level = 0
    warn_min_value = None
    error_min_value = None

    def __init__(self):
        self.children = []

    def addChild(self, child):
        self.children.append(child)

    def getVhdlName(self):
        return self.name.replace(TOP_NODE_NAME + '.', '').replace('.', '_')

    def output(self):
        print 'Name:',self.name
        print 'Description:',self.description
        print 'Address:','{0:#010x}'.format(self.address)
        print 'Permission:',self.permission
        if self.mask is not None: 
            print 'Mask:','{0:#010x}'.format(self.mask)
        else:
            print 'Mask: None'
        print 'Module:',self.isModule
        print 'Parent:',self.parent.name

def main():
    parseXML()
    print 'Example:'
    random_node = nodes[76]
    #print str(random_node.__class__.__name__)
    print 'Node:',random_node.name
    print 'Parent:',random_node.parent.name
    kids = []
    getAllChildren(random_node, kids)
    print len(kids), kids.name

def parseXML():
    print 'Open pickled address table if available ',ADDRESS_TABLE_TOP[:-3]+'pickle...'

    fname =  ADDRESS_TABLE_TOP[:-3] + "pickle"
    try:
        gc.disable()
        f = open(fname, 'r')
        global nodes 
        nodes = pickle.load(f) 
        f.close()
        gc.enable()
    except IOError:
        if 'eagle' in hostname:
            print 'Pickle file not found, please create new pickle file at the host PC and upload it to the CTP7 card'
            sys.exit()
        else:
            print 'Pickle file not found, parsing ',ADDRESS_TABLE_TOP,'...'
            import lxml.etree as xml
            tree = xml.parse(ADDRESS_TABLE_TOP)
            tree.xinclude()
            root = tree.getroot()
            vars = {}
            makeTree(root,'',0x0,nodes,None,vars,False)

            # Save parsed nodes as pickle
            name = ADDRESS_TABLE_TOP[:-3] + "pickle"
            f = open(name, 'w')
            pickle.dump(nodes, f, -1)
            f.close()

    return nodes

def makeTree(node,baseName,baseAddress,nodes,parentNode,vars,isGenerated):
    
    if (isGenerated == None or isGenerated == False) and node.get('generate') is not None and node.get('generate') == 'true':
        generateSize = parseInt(node.get('generate_size'))
        generateAddressStep = parseInt(node.get('generate_address_step'))
        generateIdxVar = node.get('generate_idx_var')
        for i in range(0, generateSize):
            vars[generateIdxVar] = i
            makeTree(node, baseName, baseAddress + generateAddressStep * i, nodes, parentNode, vars, True)
        return
    newNode = Node()
    name = baseName
    if baseName != '': name += '.'
    if node.get('id') is not None:
        name += node.get('id')
    name = substituteVars(name, vars)
    newNode.name = name
    if node.get('description') is not None:
        newNode.description = node.get('description')
    address = baseAddress
    if node.get('address') is not None:
        address = baseAddress + parseInt(node.get('address'))
    newNode.address = address
    newNode.real_address = (address<<2)+0x64000000
    newNode.permission = node.get('permission')
    newNode.mask = parseInt(node.get('mask'))
    newNode.isModule = node.get('fw_is_module') is not None and node.get('fw_is_module') == 'true'
    if node.get('sw_monitor_warn_min_threshold') is not None:
        newNode.warn_min_value = node.get('sw_monitor_warn_min_threshold') 
    if node.get('sw_monitor_error_min_threshold') is not None:
        newNode.error_min_value = node.get('sw_monitor_error_min_threshold') 
    #nodes.append(newNode)
    nodes[name] = newNode
    if parentNode is not None:
        parentNode.addChild(newNode)
        newNode.parent = parentNode
        newNode.level = parentNode.level+1
    for child in node:
        makeTree(child,name,address,nodes,newNode,vars,False)


def getAllChildren(node,kids=[]):
    if node.children==[]:
        kids.append(node)
        return kids
    else:
        for child in node.children:
            getAllChildren(child,kids)

def getNode(nodeName):
    #return next((node for node in nodes if node.name == nodeName),None)
    try: 
        return nodes[nodeName]
    except KeyError:
        print "Node %s not found" %(nodeName)
        return None

def getNodeFromAddress(nodeAddress):
    return next((node for node in nodes.values() if node.real_address == nodeAddress),None)
    #return next((node for node in nodes if node.real_address == nodeAddress),None)

def getNodesContaining(nodeString):
    #nodelist = [node for node in nodes if nodeString in node.name]
    nodelist = [nodes[key] for key in nodes if nodeString in key]
    if len(nodelist): 
        #nodelist.sort()
        return nodelist
    else: return None

#returns *readable* registers
def getRegsContaining(nodeString):
    nodelist = [nodes[key] for key in nodes if nodeString in key and nodes[key].permission is not None]
    if len(nodelist):
        #nodelist.sort()
        return nodelist
    else: return None


def readAddress(address):
    output = rReg(address) 
    return '{0:#010x}'.format(parseInt(str(output)))

def readRawAddress(raw_address):
    try: 
        address = (parseInt(raw_address) << 2)+0x64000000
        return readAddress(address)
    except:
        return 'Error reading address. (rw_reg)'

def mpeek(address):
    try: 
        output = subprocess.check_output('mpeek '+str(address), stderr=subprocess.STDOUT , shell=True)
        value = ''.join(s for s in output if s.isalnum())
    except subprocess.CalledProcessError as e: value = parseError(int(str(e)[-1:]))
    return value

def mpoke(address,value):
    try: output = subprocess.check_output('mpoke '+str(address)+' '+str(value), stderr=subprocess.STDOUT , shell=True)
    except subprocess.CalledProcessError as e: return parseError(int(str(e)[-1:]))
    return 'Done.'


def readReg(reg):
    address = reg.real_address
    if 'r' not in reg.permission:
        return 'No read permission!'
    value = rReg(parseInt(address))
    if parseInt(value) == 0xdeaddead:
        #return 'Bus Error'
        return '{0:#010x}'.format(0xdeaddead)
    if reg.mask is not None:
        shift_amount=0
        for bit in reversed('{0:b}'.format(reg.mask)):
            if bit=='0': shift_amount+=1
            else: break
        final_value = (parseInt(str(reg.mask))&parseInt(value)) >> shift_amount
    else: final_value = value
    final_int =  parseInt(str(final_value))
    return '{0:#010x}'.format(final_int)

def displayReg(reg,option=None):
    address = reg.real_address
    if 'r' not in reg.permission:
        return 'No read permission!'
    value = rReg(parseInt(address))
    if parseInt(value) == 0xdeaddead:
        if option=='hexbin': return hex(address).rstrip('L')+' '+reg.permission+'\t'+tabPad(reg.name,7)+'Bus Error'
        else: return hex(address).rstrip('L')+' '+reg.permission+'\t'+tabPad(reg.name,7)+'Bus Error'
    if reg.mask is not None:
        shift_amount=0
        for bit in reversed('{0:b}'.format(reg.mask)):
            if bit=='0': shift_amount+=1
            else: break
        final_value = (parseInt(str(reg.mask))&parseInt(value)) >> shift_amount
    else: final_value = value
    final_int =  parseInt(final_value)
    if option=='hexbin': return hex(address).rstrip('L')+' '+reg.permission+'\t'+tabPad(reg.name,7)+'{0:#010x}'.format(final_int)+' = '+'{0:032b}'.format(final_int)
    else: return hex(address).rstrip('L')+' '+reg.permission+'\t'+tabPad(reg.name,7)+'{0:#010x}'.format(final_int)
        
def writeReg(reg, value):
    address = reg.real_address
    if 'w' not in reg.permission:
        return 'No write permission!'
    # Apply Mask if applicable
    print "Initial value to write: %s, register %s"% (value,reg.name)
    if reg.mask is not None:
        shift_amount=0
        for bit in reversed('{0:b}'.format(reg.mask)):
            if bit=='0': shift_amount+=1
            else: break
        shifted_value = value << shift_amount
        for i in range(10):
            initial_value = readAddress(address)
            try: initial_value = parseInt(initial_value) 
            except ValueError: return 'Error reading initial value: '+str(initial_value)
            if initial_value == 0xdeaddead:
                print "Writing masked reg %s : Error while reading, retry attempt (%s)"%(reg.name,i)
                sleep(0.1)
                continue
            else: break
        if initial_value == 0xdeaddead:
             print "Writing masked reg %s failed. Exiting..." %(reg.name)
             #sys.exit()
        final_value = (shifted_value & reg.mask) | (initial_value & ~reg.mask)
    else: final_value = value
    output = wReg(parseInt(address),parseInt(final_value))
    if output != final_value:
        print "Writing masked reg %s failed. Exiting..." %(reg.name)
        print "wReg output %s" % (output)
        #sys.exit()
    return str('{0:#010x}'.format(final_value)).rstrip('L')+'('+str(value)+')\twritten to '+reg.name
    
def isValid(address):
    try: subprocess.check_output('mpeek '+str(address), stderr=subprocess.STDOUT , shell=True)
    except subprocess.CalledProcessError as e: return False
    return True


def completeReg(string):
    possibleNodes = [] 
    completions = []
    currentLevel = len([c for c in string if c=='.'])
  
    possibleNodes = [node for node in nodes.values() if node.name.startswith(string) and node.level == currentLevel]
    if len(possibleNodes)==1:
        if possibleNodes[0].children == []: return [possibleNodes[0].name]
        for n in possibleNodes[0].children:
            completions.append(n.name)
    else:
        for n in possibleNodes:
            completions.append(n.name)
    return completions


def parseError(e):
    if e==1:
        return "Failed to parse address"
    if e==2:
        return "My Bus error"
    else:
        return "Unknown error: "+str(e)

def parseInt(s):
    if s is None:
        return None
    string = str(s)
    if string.startswith('0x'):
        return int(string, 16)
    elif string.startswith('0b'):
        return int(string, 2)
    else:
        return int(string)


def substituteVars(string, vars):
    if string is None:
        return string
    ret = string
    for varKey in vars.keys():
        ret = ret.replace('${' + varKey + '}', str(vars[varKey]))
    return ret

def tabPad(s,maxlen):
    return s+"\t"*((8*maxlen-len(s)-1)/8+1) 

if __name__ == '__main__':
    main()
