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
                {
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
    }catch(std::exception const& e){
        std::cerr<<e.what()<<'\n';
    }
    return 0;
}
