/**
 * This file is part of the zyan core library (zyantific.com).
 * 
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Joel Höner (athre0z)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "SignalObject.hpp"

#include <algorithm>

namespace zycore
{

// ============================================================================================== //
// [SignalObject]                                                                                 //
// ============================================================================================== //

SignalObject::~SignalObject()
{
    destroy();
}

void SignalObject::destroy()
{
    std::lock_guard<std::recursive_mutex> lock(m_objectMutex);
    sigDestroy();
    for (const auto& curSignal: m_connectedSignals)
    {
        std::get<1>(curSignal)->onSlotsObjectDestroyed(std::get<0>(curSignal));
    }
}

void SignalObject::onSignalConnected(internal::SignalBase *signal, SlotHandle handle)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectMutex);
    m_connectedSignals.emplace_back(handle, signal);
}

void SignalObject::onSignalDisconnected(SlotHandle handle)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectMutex);
    m_connectedSignals.erase(std::remove_if(m_connectedSignals.begin(), m_connectedSignals.end(),
        [&](const decltype(*m_connectedSignals.data())& element) -> bool
    {
        return std::get<0>(element) == handle;
    }));
}

// ============================================================================================== //

} // namespace zycore
