#include<cstddef>   //std::size_t
#include<format>    //std::format
#include<iostream>  //std::cout std::cerr
#include<exception> //std::exception

#include<ccscript/ccscript.hpp>

int main(int argc,char* argv[]){
    using namespace ccscript;
    try{
        Variant var=Object{
            {"null",null},
            {"boolean",true},
            {"number",3.14},
            {"string","hello world"},
            {"array",
                Array{
                    null,
                    false,
                    100,
                    "ha ha!"
                }
            }
        };

        Variant str(TypeIndex::TI_STRING);
        str.get_string()="str = abc\n";
        std::cout<<str.get_const_string();

        Variant arr=Array{
            0,1,2,3
        };
        for(std::size_t index=0;index<arr.get_array().size();++index){
            std::cout
                <<std::format("arr[{}]:{}\n",index,arr[index].get_number());
        }

        Variant obj=Object{
            {"\"null",null},
            {"\\boolean",true},
            {"/number",-3.14e2},
            {"\bstring","\fhello"},
            {"\narray",Array{1,2,3}},
            {"\robject",Object{{"\tname","Tom"},{"age",20}}}
        };
        std::cout<<obj.to_string()<<'\n';

    }catch(std::exception const& e){
        std::cerr<<e.what()<<'\n';
    }
    return 0;
}
