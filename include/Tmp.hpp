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

#ifndef _ZYCORE_TMP_HPP_
#define _ZYCORE_TMP_HPP_

namespace zycore
{

// ============================================================================================== //
// [TypeStack]                                                                                    //
// ============================================================================================== //

/**
 * @brief   Stack of types.
 * @tparam  ElementsT   Initial content of the stack, top element up front.
 */
template<typename... ElementsT>
struct TypeStack final
{
    /**   
     * @brief   Bottom type indicating that there are no more elements on the stack.
     */
    class Bottom {};
private:
    /**
     * @brief   Push implementation.
     * @tparam  ItemT   The new item to push.
     */
    template<typename ItemT>
    struct PushImpl
    {
        using NewStack = TypeStack<ItemT, ElementsT...>;
    };

    /**
     * @brief   Pop implementation capturing on empty stacks.
     * @tparam  TypeStackT  Type stack type.
     */
    template<typename TypeStackT>
    struct PopImpl
    {
        using NewStack = TypeStackT;
        using Item = Bottom;
    };

    /**
     * @brief   Pop implementation capturing on stacks with elements.
     * @tparam  TypeStackT  Type stack type.
     */
    template<template<typename...> class TypeStackT, typename... ContentT, typename TopItemT>
    struct PopImpl<TypeStackT<TopItemT, ContentT...>>
    {
        using NewStack = TypeStackT<ContentT...>;
        using Item = TopItemT;
    };
public:
    template<typename ItemT>
    using Push = typename PushImpl<ItemT>::NewStack;

    using Pop = typename PopImpl<TypeStack<ElementsT...>>::NewStack;
    using Top = typename PopImpl<TypeStack<ElementsT...>>::Item;

    static const std::size_t kSize = sizeof...(ElementsT);
    
    static const bool kEmpty = kSize == 0;
};

// ============================================================================================== //

} // namespace zycore

#endif // _ZYCORE_TMP_HPP_
