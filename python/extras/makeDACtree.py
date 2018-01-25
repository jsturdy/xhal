import ast
from array import array
from ROOT import TFile, TTree, vector
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-i", type="string", dest="ifilename", default="DACData.dat",
                  help="Specify input filename", metavar="ifilename")
parser.add_option("-o", type="string", dest="ofilename", default="DACData.root",
                  help="Specify output filename", metavar="ofilename")
parser.add_option("-g", "--gtx", type="int", dest="gtx",
                  help="GTX on the AMC", metavar="gtx", default=0)


(options, args) = parser.parse_args()
ofilename = options.ofilename
ifilename = options.ifilename

myF = TFile(ofilename,'recreate')
myT = TTree('dacTree','Tree Holding CMS GEM DAC Data')

Nev = array( 'i', [ 0 ] )
myT.Branch( 'Nev', Nev, 'Nev/I' )
dacname = vector('string')()
dacname.push_back("N/A")
myT.Branch( 'dacname', dacname )
dacinval = array( 'i', [ 0 ] )
myT.Branch( 'dacinval', dacinval, 'dacinval/I' )
dacoutval = array( 'i', [ 0 ] )
myT.Branch( 'dacoutval', dacoutval, 'dacoutval/I' )
vfatN = array( 'i', [ 0 ] )
myT.Branch( 'vfatN', vfatN, 'vfatN/I' )
link = array( 'i', [ 0 ] )
myT.Branch( 'link', link, 'link/I' )
link[0] = options.gtx

with open(ifilename,"r") as f:
  for line in f.readlines():
    data = ast.literal_eval(line)
    print "data type (expected dict):"
    print type(data)
    dactype = next(iter(data))
    print "Key (DAC Name) %s" %(dactype)
    dacname[0] = str(dactype)
    #print "dac_data type (expected list):"
    #print type(dac_data)
    for event in data[dactype]:
      vfatN[0] = int(event[0])
      dacinval[0] = int(event[1])
      dacoutval[0] = int(event[2])
      myT.Fill()
    myT.AutoSave("SaveSelf")

myF.cd()
myT.Write()
myF.Close()

