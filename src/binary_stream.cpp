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

#include "binary_stream.hpp"

#include <sstream>
#include <iomanip>

namespace zycore
{

// ============================================================================================== //
// [IBinaryStream]                                                                                //
// ============================================================================================== //

std::string IBinaryStream::extractString8(StreamOffs pos, size_t maxLen) const
{
    auto curPos = pos;
    while (*constPtr<char>(curPos) != 0 && (maxLen == 0 || curPos - pos < maxLen))
    {
        ++curPos;
    }
    return std::string(constPtr<char>(pos), curPos - pos);
}

std::wstring IBinaryStream::extractString16(StreamOffs pos, size_t maxLen) const
{
    auto curPos = pos;
    while (*constPtr<wchar_t>(curPos) != 0 
        && (maxLen == 0 || curPos - pos < maxLen * sizeof(wchar_t)))
    {
        ++curPos;
    }
    return std::wstring(constPtr<wchar_t>(pos), curPos - pos);
}

std::string IBinaryStream::hexDump(StreamOffs pos, size_t len) const
{
    validateOffset(pos, len);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    // Loop through buffer's bytes
    for (int i = pos, j = 0; i < static_cast<int>(len + pos); ++i, ++j)
    {
        // First byte in line? Prefix with position
        if (j == 0)
        {
            ss << "0x" << std::setw(4) << i - pos;
        }

        // Print byte
        ss << ' ' << std::setw(2) << static_cast<int>(m_buffer->data()[i]);

        // Last byte in row or last byte? Append ASCII dump
        bool lastRound = i == static_cast<int>(len + pos - 1);
        if (j == 15 || lastRound)
        {
            // If last round, fill delta with spaces
            if (lastRound)
            {
                for (int k = 16 - j; k != 1; --k)
                {
                    ss << "   ";
                }
            }

            ss << ' ';

            // Create ASCII dump
            for (int k = 0; lastRound && k <= j || !lastRound && k < 16; ++k)
            {
                unsigned char chCur = m_buffer->data()[i - j + k];
                ss << (isprint(chCur) ? static_cast<char>(chCur) : '.');
            }
            ss << std::endl;
            j = -1;
        }
    }

    return ss.str();
}

// ============================================================================================== //

} // namespace zycore
