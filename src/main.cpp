#include"main.hpp"

#include<iostream>
#include<format>

void say_hello(void)noexcept{
    std::cout<<"hello world!\n";
}

int main(int argc,char* argv[])noexcept{
    for(int index=1;index<argc;++index){
        std::cout<<std::format("argv[{}]: \"{}\"\n",index,argv[index]);
    }
    say_hello();
    return 0;
}
