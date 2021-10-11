#pragma once
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <string_view>
#include <set>
#include <memory>
namespace mcc_utilites {

    // classes

    template<class Element>
    using Vector = std::vector<Element>; 

    template<class... Types>
    using Tuple = std::tuple<Types...>;

    template<class Key, class Value>
    using Map = std::map<Key, Value>;

    using String = std::string;

    using StringView = std::string_view;

    template<class Element>
    using Set = std::set<Element>;

    using Path = std::filesystem::path;

    template<class T>
    using SharedPtr = std::shared_ptr<T>;

    // functions 
    using std::tie; 
}

