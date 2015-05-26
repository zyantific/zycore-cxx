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

#include "ReflectableObject.hpp"
#include "Exceptions.hpp"

#include <algorithm>
#include <string>

namespace zycore
{

// ============================================================================================== //
// [ReflectableObject]                                                                            //
// ============================================================================================== //

void ReflectableObject::setObjectName(const std::string& name)
{
    if (!m_objectName)
    {
        m_objectName.reset(new std::string(name));
    } 
    else
    {
        *m_objectName = name;
    }
}

const std::string& ReflectableObject::objectName() const
{
    static const std::string sharedEmptyName;
    return m_objectName ? *m_objectName : sharedEmptyName;
}

void ReflectableObject::registerProperty(PropertyBase* prop)
{
    std::lock_guard<std::recursive_mutex> lock(m_propertyListLock);
    if (std::find(m_propertyList.cbegin(), m_propertyList.cend(), prop) != m_propertyList.cend())
    {
        throw InvalidUsage("property already registered");
    }
    m_propertyList.push_back(prop);
}

void ReflectableObject::unregisterProperty(PropertyBase* prop)
{
    std::lock_guard<std::recursive_mutex> lock(m_propertyListLock);
    m_propertyList.erase(std::remove(m_propertyList.begin(), m_propertyList.end(), prop));
}

// ============================================================================================== //

} // namespace zycore