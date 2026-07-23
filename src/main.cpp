#include<ccscript/ccscript.hpp>

int main(int argc,char* argv[])noexcept{
    using namespace ccscript;
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
    return 0;
}
