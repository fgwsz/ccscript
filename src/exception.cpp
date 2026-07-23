#include<format>//std::format

#include<ccscript/exception.hpp>

namespace ccscript{

char const* Exception::what(void)const{
    return this->what_.c_str();
}
Exception::Exception(
    std::string_view const message,
    std::source_location const location=std::source_location::current()
):what_(
    std::format(
        "file: {}({}:{}) `{}`: {}",
        location.file_name(),
        location.line(),
        location.column(),
        location.function_name(),
        message
    )
){}

}//namespace ccscript
