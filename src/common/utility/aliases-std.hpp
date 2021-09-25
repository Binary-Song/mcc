#pragma once
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <string_view>
#include <set>
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

    // functions 
    using std::tie; 
}

