#pragma once
#include "../UCodeAnalyzer.hpp"
#include "UCodeLang/LangCore.hpp"
UCodeAnalyzerStart

using Int8 = UCodeLang::Int8;
using Int16 = UCodeLang::Int16;
using Int32 = UCodeLang::Int32;
using Int64 = UCodeLang::Int64;

using UInt8 = UCodeLang::UInt8;
using UInt16 = UCodeLang::UInt16;
using UInt32 = UCodeLang::UInt32;
using UInt64 = UCodeLang::UInt64;

using float32 = UCodeLang::float32;
using float64 = UCodeLang::float64;

using String = UCodeLang::String;
using StringView = UCodeLang::String_view;


template<typename T> using Vector = UCodeLang::Vector<T>;
template<typename T, typename T2> using unordered_map = UCodeLang::unordered_map<T, T2>;

UCodeAnalyzerEnd