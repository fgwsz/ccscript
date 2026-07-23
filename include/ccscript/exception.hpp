#pragma once

#include<exception>         //std::exception
#include<source_location>   //std::source_location
#include<string_view>       //std::string_view

namespace ccscript{

class Exception:public std::exception{
public:
    virtual char const* what(void)const override;
    Exception(
        std::string_view const message,
        std::source_location const location=std::source_location::current()
    );
private:
    std::string what_;
};

}//namespace ccscript
