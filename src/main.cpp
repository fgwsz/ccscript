#include<iostream>  //std::cout std::cerr
#include<exception> //std::exception

#include<ccscript/ccscript.hpp>

int main(int argc,char* argv[]){
    using namespace ccscript;
    try{
        Variant var=Object{
            {String{"null"},Null{}},
            {String{"boolean"},Boolean{false}},
            {String{"number"},Number{3.14}},
            {String{"string"},String{"hello world"}},
            {String{"array"},
                Array{
                    Null{},
                    Boolean{true},
                    Number{6.28},
                    String{"ha ha!"}
                }
            },
        };
        Variant str(TypeIndex::TI_STRING);
        str.get_string()="str = abc\n";
        std::cout<<str.get_const_string();
    }catch(std::exception const& e){
        std::cerr<<e.what()<<'\n';
    }
    return 0;
}
