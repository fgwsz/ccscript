#pragma once

#include<string>//std::string
#include<vector>//std::vector
#include<map>   //std::map

namespace ccscript{

//基本类型
struct Null{};
inline Null null;
enum class Boolean{
    FALSE=0,
    TRUE=1
};
using Number=long double;
using String=std::string;

//复合类型
class Variant;
using Array=std::vector<Variant>;
using Object=std::map<String,Variant>;

}//namespace ccscript
