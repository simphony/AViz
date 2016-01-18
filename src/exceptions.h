#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace aviz {

/* file parsing error */
class parse_error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}

#endif // EXCEPTIONS_H
