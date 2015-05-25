/**
 * This file is part of the zyan core library (zyantific.com).
 * 
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Joel Höner (athre0z)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _ZYCORE_EXCEPTIONS_HPP_
#define _ZYCORE_EXCEPTIONS_HPP_

#include <stdexcept>

namespace zycore
{
    
// ============================================================================================== //
// [RuntimeErrorTemplate]                                                                         //
// ============================================================================================== //

/**
 * @brief   Templated base class for runtime exceptions of any kind.
 * @tparam  MarkerT The marker type used for disambiguation.
 *                  
 * As forwarding constructors for specialized runtime-error exception types can get really tedious,
 * we create them using template aliases. By allocating a new type using the class/struct 
 * keyword and feeding it into the base template, the types become distinguishable in exception
 * handling.
 */
template<typename MarkerT>
class RuntimeErrorTemplate : public std::runtime_error
{
public: // Forwarded constructors.
    explicit RuntimeErrorTemplate(const std::string& message) : runtime_error(message) {}
    explicit RuntimeErrorTemplate(const char* message) : runtime_error(message) {}
};

#define ZYCORE_EXCEPTION_TYPE_FROM_TEMPLATE(template_, newType)                                    \
    class __##newType {};                                                                          \
    using newType = template_<__##newType>;

// ============================================================================================== //
// Exception types used all over the project                                                      //
// ============================================================================================== //

ZYCORE_EXCEPTION_TYPE_FROM_TEMPLATE(RuntimeErrorTemplate, InvalidUsage  );
ZYCORE_EXCEPTION_TYPE_FROM_TEMPLATE(RuntimeErrorTemplate, NotImplemented);
ZYCORE_EXCEPTION_TYPE_FROM_TEMPLATE(RuntimeErrorTemplate, OutOfBounds   );

// ============================================================================================== //

} // namespace zycore

#endif // _ZYCORE_EXCEPTIONS_HPP_
