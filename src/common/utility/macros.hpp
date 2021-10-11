#pragma once

#define TYPE(...) __VA_ARGS__


/// 定义变量和变量的访问器。
#define VAR(VarType, VarName)\
    private: VarType VarName; /*var itself*/\
    public: VarType const& get_##VarName() const&  { return VarName; } /* const getter*/\
    VarType& get_##VarName()& {return VarName;} /* mutable getter */ \
    VarType get_##VarName()&& { return VarName; } /* copy getter (for temporary `this`)*/\
    auto set_##VarName(VarType const& value) {VarName = value; return *this; } /* copy setter */ \
    auto set_##VarName(VarType && value) {VarName = std::move(value); return *this ; }  /* move setter */