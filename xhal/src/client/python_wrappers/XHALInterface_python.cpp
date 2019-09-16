#include <boost/python.hpp>
#include "xhal/common/utils/Exception.h"
#include "xhal/client/XHALDevice.h"
#include "xhal/client/utils/PyTypes.h"
#include "xhal/client/rpcman/daq_monitor.h"
#include "xhal/client/rpcman/utils.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

// Boost Python exception translators

//https://stackoverflow.com/questions/9620268/boost-python-custom-exception-class
PyObject* createExceptionClass(const char* name, PyObject* baseTypeObj = PyExc_Exception)
{
    using std::string;
    namespace bp = boost::python;

    string scopeName = bp::extract<string>(bp::scope().attr("__name__"));
    string qualifiedName0 = scopeName + "." + name;
    char* qualifiedName1 = const_cast<char*>(qualifiedName0.c_str());

    PyObject* typeObj = PyErr_NewException(qualifiedName1, baseTypeObj, 0);
    if (!typeObj) bp::throw_error_already_set();
    bp::scope().attr(name) = bp::handle<>(bp::borrowed(typeObj));
    return typeObj;
}

// https://www.boost.org/doc/libs/1_51_0/libs/python/doc/tutorial/doc/html/python/exception.html
#ifndef PY_EXCEPTION_TRANSLATOR
#define PY_EXCEPTION_TRANSLATOR(TRANSLATOR_NAME,EXCEPTION_NAME,EXCEPTION_OBJ)        \
PyObject* EXCEPTION_OBJ = NULL;                                                      \
inline void TRANSLATOR_NAME(EXCEPTION_NAME const& e)                                 \
{                                                                                    \
  assert(EXCEPTION_OBJ != NULL);                                                     \
  /* Use the Python 'C' API to set up an exception object*/                          \
  PyErr_SetString(EXCEPTION_OBJ, e.msg.c_str());                                     \
}
#endif

PY_EXCEPTION_TRANSLATOR(translate_XHALException,xhal::common::utils::XHALException, obj_XHALException)
PY_EXCEPTION_TRANSLATOR(translate_XHALXMLParserException,xhal::common::utils::XHALXMLParserException, obj_XHALXMLParserException)
PY_EXCEPTION_TRANSLATOR(translate_XHALRPCException,xhal::common::utils::XHALRPCException, obj_XHALRPCException)
PY_EXCEPTION_TRANSLATOR(translate_XHALRPCNotConnectedException,xhal::common::utils::XHALRPCNotConnectedException, obj_XHALRPCNotConnectedException)

// https://stackoverflow.com/questions/7577410/boost-python-select-between-overloaded-methods
uint32_t (xhal::client::XHALDevice::*readReg_byname)(std::string regName) = &xhal::client::XHALDevice::readReg;
uint32_t (xhal::client::XHALDevice::*readReg_byaddress)(uint32_t address) = &xhal::client::XHALDevice::readReg;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getmonTRIGGERmain_overloads, getmonTRIGGERmain, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getmonTRIGGEROHmain_overloads, getmonTRIGGEROHmain, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getmonDAQOHmain_overloads, getmonDAQOHmain, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getmonOHmain_overloads, getmonOHmain, 0, 1)

BOOST_PYTHON_MODULE(xhalpy){
    using namespace boost::python;

    obj_XHALException = createExceptionClass("XHALException");
    obj_XHALXMLParserException = createExceptionClass("XHALXMLParserException");
    obj_XHALRPCException = createExceptionClass("XHALRPCException");
    obj_XHALRPCNotConnectedException = createExceptionClass("XHALRPCNotConnectedException");

    register_exception_translator<xhal::common::utils::XHALException>(&translate_XHALException);
    register_exception_translator<xhal::common::utils::XHALXMLParserException>(&translate_XHALXMLParserException);
    register_exception_translator<xhal::common::utils::XHALRPCException>(&translate_XHALRPCException);
    register_exception_translator<xhal::common::utils::XHALRPCNotConnectedException>(&translate_XHALRPCNotConnectedException);

    class_<xhal::client::XHALDevice>("XHALDevice", init<const std::string&, const std::string&>())
        .def("connect",&xhal::client::XHALDevice::connect)
        .def("reconnect",&xhal::client::XHALDevice::reconnect)
        .def("disconnect",&xhal::client::XHALDevice::disconnect)
        .def("loadModule",&xhal::client::XHALDevice::loadModule)
        .def("setLogLevel",&xhal::client::XHALDevice::setLogLevel)
        .def("readReg",readReg_byname)
        .def("readReg",readReg_byaddress)
        .def("writeReg",&xhal::client::XHALDevice::writeReg);

    class_<PyListUint32>("PyListUint32")
        .def(vector_indexing_suite<PyListUint32>() );

    class_<PyDictVecUint32>("PyDictVecUint32")
        .def(map_indexing_suite<PyDictVecUint32>() );

    class_<xhal::client::rpcman::Utils>("Utils", init<const std::string&>())
        .def("update_atdb",&xhal::client::rpcman::Utils::update_atdb)
        .def("getRegInfoDB",&xhal::client::rpcman::Utils::getRegInfoDB);

    class_<xhal::client::rpcman::DaqMonitor>("DaqMonitor", init<const std::string&>())
        .def("getmonTTCmain",&xhal::client::rpcman::DaqMonitor::getmonTTCmain)
        .def("getmonTRIGGERmain",&xhal::client::rpcman::DaqMonitor::getmonTRIGGERmain,getmonTRIGGERmain_overloads())
        .def("getmonTRIGGEROHmain",&xhal::client::rpcman::DaqMonitor::getmonTRIGGEROHmain,getmonTRIGGEROHmain_overloads())
        .def("getmonDAQOHmain",&xhal::client::rpcman::DaqMonitor::getmonDAQOHmain,getmonDAQOHmain_overloads())
        .def("getmonOHmain",&xhal::client::rpcman::DaqMonitor::getmonOHmain,getmonOHmain_overloads())
        .def("getmonDAQmain",&xhal::client::rpcman::DaqMonitor::getmonDAQmain);
}
