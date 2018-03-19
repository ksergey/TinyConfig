/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_120318163900
#define KSERGEY_exception_120318163900

#include <stdexcept>

namespace TinyConfig {

/// Field check error exception
struct CheckError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// Field behaviour error exception
struct PolicyError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// Field type error exception
struct TypeError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

} /* namespace TinyConfig */

#endif /* KSERGEY_exception_120318163900 */
