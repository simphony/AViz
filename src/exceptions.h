#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace aviz {

/*! file parsing error */
class parse_error : public std::runtime_error {

public:
     explicit parse_error (const std::string& what_arg)
         :std::runtime_error(what_arg) {
     }
     explicit parse_error (const char* what_arg)
         :std::runtime_error(what_arg) {
     }
};

}

#endif // EXCEPTIONS_H
