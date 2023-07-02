#pragma once
#include "UCodeAnalyzer.hpp"
#include "UCodeLang/LangCore.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include "UCodeLang/LangCore/BitMaker.hpp"
UCodeAnalyzerStart

using Byte = UCodeLang::Byte;

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
using Path = UCodeLang::Path;

template<typename T> using Unique_ptr = UCodeLang::Unique_ptr<T>;
template<typename T> using Unique_Array = UCodeLang::Unique_ptr<T[]>;

template<typename T> using Shared_ptr = UCodeLang::Shared_ptr<T>;
template<typename T> using Weak_ptr = UCodeLang::Weak_ptr<T>;


template<typename T> using Optional = UCodeLang::Optional<T>;
template<typename T> using Vector = UCodeLang::Vector<T>;
template<typename T> using Stack = UCodeLang::Stack<T>;
template<typename T, typename T2> using Unordered_map = UCodeLang::Unordered_map<T, T2>;

template<typename T, size_t Size> using Array = UCodeLang::Array<T, Size>;

template<typename... T> using Variant = UCodeLang::Variant<T...>;

using BytesView = UCodeLang::BytesView;

template<typename Key,typename Value> using BinaryVectorMap = UCodeLang::BinaryVectorMap<Key, Value>;

using BitMaker = UCodeLang::BitMaker;
using BitReader = UCodeLang::BitReader;
UCodeAnalyzerEnd