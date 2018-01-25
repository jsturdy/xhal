#include "xhal/utils/XHALXMLParser.h"

xhal::utils::XHALXMLParser::XHALXMLParser(const std::string& xmlFile)
{
  m_xmlFile = xmlFile;
  log4cplus::SharedAppenderPtr myAppender(new log4cplus::ConsoleAppender());
  std::auto_ptr<log4cplus::Layout> myLayout = std::auto_ptr<log4cplus::Layout>(new log4cplus::TTCCLayout());
  myAppender->setLayout( myLayout );
  auto t_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("XHALXMLParser"));
  m_logger = t_logger;
  m_logger.addAppender(myAppender);
  m_logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
  m_nodes = new std::unordered_map<std::string,Node>();
  //m_root = new xercesc::DOMNode();
}

xhal::utils::XHALXMLParser::~XHALXMLParser()
{
  if (m_nodes) delete m_nodes;
  m_logger.shutdown();
}

void xhal::utils::XHALXMLParser::setLogLevel(int loglevel)
{
  switch(loglevel)
  {
    case 0:
      m_logger.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
      break;
    case 1:
      m_logger.setLogLevel(log4cplus::WARN_LOG_LEVEL);
      break;
    case 2:
      m_logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
      break;
    case 3:
      m_logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
      break;
    case 4:
      m_logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);
      break;
  }
}

void xhal::utils::XHALXMLParser::parseXML()
{
  //
  /// Initialize XML4C system
  try {
    xercesc::XMLPlatformUtils::Initialize();
    INFO("Successfully initialized XML4C system");
  } catch(const xercesc::XMLException& toCatch) {
    ERROR("Error during Xerces-c Initialization." << std::endl
          << "  Exception message:"
          << xercesc::XMLString::transcode(toCatch.getMessage()));
    throw xhal::utils::Exception("XHALParser: initialization failed"); 
    return;
  }

  //  Create our parser, then attach an error handler to the parser.
  //  The parser will call back to methods of the ErrorHandler if it
  //  discovers errors during the course of parsing the XML document.
  //
  xercesc::XercesDOMParser* parser = new xercesc::XercesDOMParser;
  DEBUG("Xerces parser created ");
  parser->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
  parser->setDoNamespaces(false);
  parser->setCreateEntityReferenceNodes(false);
  // parser->setCreateEntityReferenceNodes(true);
  // parser->setExpandEntityReferences(true);
  parser->setDoXInclude(true);
  // parser->setToCreateXMLDeclTypeNode(true);
  DEBUG("Xerces parser tuned up ");

  //  Parse the XML file, catching any XML exceptions that might propogate
  //  out of it.
  //
  bool errorsOccured = false;
  try {
    parser->parse(m_xmlFile.c_str());
  } catch (const xercesc::XMLException& e) {
    ERROR("An error occured during parsing" << std::endl
          << "   Message: "
          << xercesc::XMLString::transcode(e.getMessage()));
    errorsOccured = true;
    // fileError = "An error occured during parsing of selected file. Please select another configuration file.";
  } catch (const xercesc::DOMException& e) {
    ERROR("An error occured during parsing" << std::endl
          << "   Message: "
          << xercesc::XMLString::transcode(e.msg));
    errorsOccured = true;
    // fileError = "An error occured during parsing of selected file. Please select another configuration file.";
  } catch (...) {
    ERROR("An error occured during parsing");
    errorsOccured = true;
    // fileError = "An error occured during parsing of selected file. Please select another configuration file.";
  }

  if (!errorsOccured) {
    DEBUG("DOM tree created succesfully");
    if (parser->getDocument()->getDocumentElement()!=NULL){
      m_root = parser->getDocument()->getDocumentElement();
    }
    DEBUG("Root node (getDocumentElement) obtained");
    makeTree(m_root,"",0x0,m_nodes,NULL,m_vars,false);
    DEBUG("Number of nodes: " << m_nodes->size());
  } else{
    throw xhal::utils::Exception("XHALParser: an error occured during parsing"); 
  }
  DEBUG("Parsing done!");
  if (parser) delete(parser);
}

void xhal::utils::XHALXMLParser::makeTree(xercesc::DOMNode * node, std::string baseName, uint32_t baseAddress, std::unordered_map<std::string, Node> * nodes, Node * parentNode, std::unordered_map<std::string, int> vars, bool isGenerated)
{
  DEBUG("Call makeTree");
  unsigned int generateSize;
  unsigned int generateAddressStep;
  std::string generateIdxVar;
  DEBUG("Declare some local variables");
  if (isGenerated == false)
  {
    if (auto tmp = getAttVal(node, "generate"))
    {
      if (*tmp == "true"){
        tmp = getAttVal(node, "generate_size");
        generateSize = parseInt(*tmp);
        tmp = getAttVal(node, "generate_address_step");
        generateAddressStep = parseInt(*tmp);
        auto generateIdxVar = getAttVal(node, "generate_idx_var");
        for (unsigned int i = 0; i < generateSize; i++)// in range(0, generateSize):
        {
          vars[*generateIdxVar] = i;
          makeTree(node, baseName, baseAddress + generateAddressStep * i, nodes, parentNode, vars, true);
        }
        return;
      }
    }
  }
  Node newNode = Node();
  DEBUG("Create Node() object");
  std::string name;
  uint32_t address;
  name = baseName;
  address = baseAddress;
  if (baseName != "") {name += ".";}
  name += *getAttVal(node, "id");
  name = substituteVars(name, vars);
  DEBUG("Node name: " << name);
  newNode.name = name;
  if (auto tmp = getAttVal(node, "description"))
  { 
    newNode.description = *tmp;
  }
  if (auto tmp = getAttVal(node, "address"))
  { 
    address = baseAddress + parseInt(*tmp);
    newNode.address = address;
    newNode.real_address = (address<<2)+0x64000000;
  } else {
    DEBUG("getAttVal returned NONE");
  }
  if (auto tmp = getAttVal(node, "permission"))
  { 
    newNode.permission = *tmp;
  }
  if (auto tmp = getAttVal(node, "mask"))
  { 
    newNode.mask = parseInt(*tmp);
  }
  newNode.isModule = (getAttVal(node, "fw_is_module")) && (*getAttVal(node, "fw_is_module") == "true");
  if (auto tmp = getAttVal(node, "sw_monitor_warn_min_threshold"))
  { 
    newNode.warn_min_value = parseInt(*tmp);
  }
  if (auto tmp = getAttVal(node, "sw_monitor_error_min_threshold"))
  { 
    newNode.warn_min_value = parseInt(*tmp);
  }

  //nodes->push_back(newNode);
  nodes->insert(std::make_pair(newNode.name,newNode));
  //TRACE("Node vector size after push_back: " << nodes->size());
  TRACE("Node map size after insert " << nodes->size());
  if (parentNode)
  {
    parentNode->addChild(newNode);
    newNode.parent = parentNode;
    newNode.level = parentNode->level+1;
  }
  xercesc::DOMNodeList *children_ = node->getChildNodes();
  const XMLSize_t nodeCount = children_->getLength();
  DEBUG("Node children length: " << nodeCount);
          
  for( XMLSize_t ix = 0 ; ix < nodeCount ; ++ix )
  {
    if (children_->item(ix)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) 
    {
      makeTree(children_->item(ix),name,address,nodes,&newNode,vars,false);
    } else {
      continue;
    }
  }
}

std::experimental::optional<std::string> xhal::utils::XHALXMLParser::getAttVal(xercesc::DOMNode * t_node_, const char * attname)
{
  DEBUG("Call getAttVal for attribute " << attname);
  XMLCh * tmp = xercesc::XMLString::transcode(attname);
  xercesc::DOMElement* t_node = static_cast<xercesc::DOMElement*>(t_node_);
  TRACE("tmp: " << tmp);
  TRACE("successfull call of getAttribute: " << t_node->getAttribute(tmp));
  char * tmp2 = xercesc::XMLString::transcode(t_node->getAttribute(tmp)); 
  TRACE("tmp2: " << tmp2);
  if (tmp2[0]!='\0')
  {
    std::string value = tmp2;
    xercesc::XMLString::release(&tmp);
    xercesc::XMLString::release(&tmp2);
    DEBUG("result " << value);
    return value;
  } else 
  {
    DEBUG("Attribute not found");
    xercesc::XMLString::release(&tmp);
    xercesc::XMLString::release(&tmp2);
    return {};
  }
}

unsigned int xhal::utils::XHALXMLParser::parseInt(std::string & s)
{
  DEBUG("Call parseInt for argument " << s);
  std::stringstream converter(s);
  uint32_t value;
  if (s.find("0x") != std::string::npos) {
    converter >> std::hex >> value;
    DEBUG("result " << value);
    return value;
  } else if (s.find("0b") != std::string::npos) {
    DEBUG("result " << std::stoi(s,nullptr,2));
    return std::stoi(s,nullptr,2);
  } else {
    converter >> std::dec >> value;
    DEBUG("result " << value);
    return value;
  }
}

std::string xhal::utils::XHALXMLParser::substituteVars(std::string & s, std::unordered_map<std::string, int> dict)
{
  if (s == "") {return s;}
  std::string ret;
  ret = s;
  for (auto& p: dict)
  {
    ret = replaceAll(ret,"${" + p.first + "}",std::to_string(p.second));
  }
  return ret;
}

std::string xhal::utils::XHALXMLParser::replaceAll( std::string const& original, std::string const& from, std::string const& to )
{
    std::string results;
    std::string::const_iterator end = original.end();
    std::string::const_iterator current = original.begin();
    std::string::const_iterator next = std::search( current, end, from.begin(), from.end() );
    while ( next != end ) {
        results.append( current, next );
        results.append( to );
        current = next + from.size();
        next = std::search( current, end, from.begin(), from.end() );
    }
    results.append( current, next );
    return results;
}

std::experimental::optional<xhal::utils::Node> xhal::utils::XHALXMLParser::getNode(const char* nodeName)
{
  DEBUG("Call getNode for argument " << nodeName);
  //Node * res = NULL;
  TRACE("Searching unordered map");
  auto search = m_nodes->find(nodeName);
  if (search!=m_nodes->end())
  {
    return search->second;
  } else {
    return {};
  }
  //TRACE("Looping over nodes");
  //for (auto & n: *m_nodes)
  //{
  //  TRACE("Node name: " << n.name);
  //  if (nodeName == n.name) 
  //  {
  //    res = &n;
  //    break;
  //  }
  //}
  //if (res) 
  //{
  //  return *res;
  //} else {
  //  return {};
  //}
}

std::experimental::optional<xhal::utils::Node> xhal::utils::XHALXMLParser::getNodeFromAddress(const uint32_t nodeAddress)
{
  //Node * res = NULL;
  //for (auto & n: *m_nodes)
  //{
  //  if (nodeAddress == n.real_address) 
  //  {
  //    res = &n;
  //    break;
  //  }
  //}
  //if (res) 
  //{
  //  return *res;
  //} else {
  //  return {};
  //}

}

std::unordered_map<std::string,xhal::utils::Node> xhal::utils::XHALXMLParser::getAllNodes()
{
  return *m_nodes;
}
