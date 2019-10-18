/**
 * @file Exception.h
 * XHAL exception base class
 *
 * @author Mykhailo Dalchenko
 * @version 1.0 
 */

#ifndef XHAL_UTILS_EXCEPTION_H
#define XHAL_UTILS_EXCEPTION_H

#include <string>
#include <exception>

#define XHAL_UTILS_DEFINE_EXCEPTION(EXCEPTION_NAME)                      \
namespace xhal {                                                         \
  namespace utils {                                                      \
    class EXCEPTION_NAME : public std::exception {                       \
      public:                                                            \
        EXCEPTION_NAME(const char* message) : m(message) {               \
                                                                         \
        }                                                                \
                                                                         \
        virtual ~EXCEPTION_NAME() throw() {                              \
                                                                         \
        }                                                                \
                                                                         \
        virtual const char* what() const throw() {                       \
            return m.c_str();                                            \
        }                                                                \
                                                                         \
      protected:                                                         \
        std::string m;                                                   \
                                                                         \
      private:                                                           \
        EXCEPTION_NAME();                                                \
    };                                                                   \
  } /* namespace xhal::utils */                                          \
} /* namespace xhal */                                                   

XHAL_UTILS_DEFINE_EXCEPTION(Exception)

#endif //XHAL_UTILS_EXCEPTION_H
