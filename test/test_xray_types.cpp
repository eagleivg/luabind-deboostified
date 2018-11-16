// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "test.hpp"

#include <luabind/luabind.hpp>
#include <luabind/function_introspection.hpp>
#include <luabind/set_package_preload.hpp>

#include <cstdint>
#include <limits>
#include <iostream>

// Type defs
using s8 = std::int8_t;
using u8 = std::uint8_t;

using s16 = std::int16_t;
using u16 = std::uint16_t;

using s32 = std::int32_t;
using u32 = std::uint32_t;

using s64 = std::int64_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using pstr = char*;
using pcstr = const char*;

using namespace luabind;

std::string test(s8 a, u8 b, s16 c, u16 d, s32 e, u32 f, s64 g, u64 h, f32 i, f64 k)
{
    return std::to_string(a);
}

luabind::string test_s8(s8 a)
{
    std::cout << __func__ << std::endl;
    char tmp[16] = { 0 };
    sprintf(tmp, "%d", a);
    return luabind::string(tmp);
}

luabind::string test_u8(u8 a)
{
    std::cout << __func__ << std::endl;
    char tmp[16] = { 0 };
    sprintf(tmp, "%u", a);
    return luabind::string(tmp);
}


void test_main(lua_State* L)
{
    
    using namespace luabind;
    set_package_preload(L, "luabind.function_introspection", &bind_function_introspection);
    
    DOSTRING(L,
        "require('luabind.function_introspection')");
    DOSTRING(L,
        "assert(function_info.get_function_name)");
    DOSTRING(L,
        "assert(function_info.get_function_overloads)");
    
    module(L)
    [
        def("test", &test),
            def("test", &test_u8),
            def("test", &test_s8),
            def("test_u8", &test_u8),
            def("test_s8", &test_s8)
    ];
    
    DOSTRING(L,
        "for _,v in ipairs(function_info.get_function_overloads(test)) do print(v) end");
    
    //test overload
    DOSTRING(L, "print(test(257))");
    DOSTRING(L, "print(test(-1))");
    
    //test direct
    DOSTRING(L, "print(test_u8(257))");
    DOSTRING(L, "print(test_s8(-1))");
    
}

