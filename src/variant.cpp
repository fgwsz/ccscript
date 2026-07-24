#include<new>       //std::nothrow
#include<utility>   //std::move
#include<format>    //std::format

#include<ccscript/exception.hpp>

#include<ccscript/variant.hpp>

//tip:
//  GCC编译器下,Array{arr}语句会导致无限递归,(MSVC不会导致无限循环)
//      必须使用Array(arr)代替Array{arr}语句!!!
//  原因:
//  Array{array}
//  GCC函数匹配规则把{array}解释为包含一个元素的initializer_list<?>
//  即优先匹配为包含一个元素(值为arr)的initializer_list
//  但是arr的类型是Array,它可以隐式转换为Variant
//  所以?是Array还是Variant呢?两种匹配路径都说得通.
//  如果编译器选择了?=Variant路线.(恰好GCC选择了这条路)
//  Array{arr}->Variant(Array const&/&& arr)->new Array{arr}
//      ->Variant(Array const&/&& arr)->...无限循环

namespace ccscript{

Variant::Variant(Null value){
    data_.null_value=value;
    data_.type_index=TypeIndex::TI_NULL;
}

Variant::Variant(Boolean value){
    data_.boolean_value=value;
    data_.type_index=TypeIndex::TI_BOOLEAN;
}

Variant::Variant(Number value){
    data_.number_value=value;
    data_.type_index=TypeIndex::TI_NUMBER;
}

Variant::Variant(String const& value){
    void* ptr=nullptr;
    ptr=new(std::nothrow) String{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_STRING;
}

Variant::Variant(Array const& value){
    void* ptr=nullptr;
    ptr=new(std::nothrow) Array(value);
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_ARRAY;
}

Variant::Variant(Object const& value){
    void* ptr=nullptr;
    ptr=new(std::nothrow) Object{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_OBJECT;
}

Variant::Variant(String && str){
    void* ptr=nullptr;
    ptr=new(std::nothrow) String{std::move(str)};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_STRING;
}

Variant::Variant(Array && arr){
    void* ptr=nullptr;
    ptr=new(std::nothrow) Array(std::move(arr));
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_ARRAY;
}

Variant::Variant(Object && obj){
    void* ptr=nullptr;
    ptr=new(std::nothrow) Object{std::move(obj)};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_OBJECT;
}

Variant::Variant(void)
    :Variant(Null{}){
}

Variant::Variant(Variant const& variant){
    if(variant.is_pod()){
        if(variant.is_null()){
            data_.null_value=variant.get_null();
        }else if(variant.is_boolean()){
            data_.boolean_value=variant.get_boolean();
        }else{//if(variant.is_number())
            data_.number_value=variant.get_number();
        }
        data_.type_index=variant.get_type_index();
        return;
    }
    void* ptr=nullptr;
    if(variant.is_string()){
        ptr=new(std::nothrow) String{variant.get_string()};
    }else if(variant.is_array()){
        ptr=new(std::nothrow) Array(variant.get_array());
    }else{//if(variant.is_object())
        ptr=new(std::nothrow) Object{variant.get_object()};
    }
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=variant.get_type_index();
}

Variant::Variant(Variant && variant){
    if(variant.is_null()){
        data_.null_value=variant.get_null();
    }else if(variant.is_boolean()){
        data_.boolean_value=variant.get_boolean();
    }else if(variant.is_number()){
        data_.number_value=variant.get_number();
    }else{
        data_.nopod_value=variant.data_.nopod_value;
        variant.data_.nopod_value=nullptr;
    }
    data_.type_index=variant.get_type_index();
    variant.data_.null_value={};
    variant.data_.type_index=TypeIndex::TI_NULL;
}

Variant& Variant::operator=(Variant const& variant){
    if(this==&variant){
        return *this;
    }
    if(variant.is_pod()){
        if(!is_pod()){
            reset();
        }
        if(variant.is_null()){
            data_.null_value=variant.get_null();
        }else if(variant.is_boolean()){
            data_.boolean_value=variant.get_boolean();
        }else{//if(variant.is_number())
            data_.number_value=variant.get_number();
        }
        data_.type_index=variant.get_type_index();
        return *this;
    }
    if(get_type_index()==variant.get_type_index()){
        if(variant.is_string()){
            get_string()=variant.get_string();
        }else if(variant.is_array()){
            get_array()=variant.get_array();
        }else{//if(variant.is_object())
            get_object()=variant.get_object();
        }
        return *this;
    }
    if(!is_pod()){
        reset();
    }
    void* ptr=nullptr;
    if(variant.is_string()){
        ptr=new(std::nothrow) String{variant.get_string()};
    }else if(variant.is_array()){
        ptr=new(std::nothrow) Array(variant.get_array());
    }else{//if(variant.is_object())
        ptr=new(std::nothrow) Object{variant.get_object()};
    }
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=variant.get_type_index();
    return *this;
}

Variant& Variant::operator=(Variant && variant){
    if(this==&variant){
        return *this;
    }
    if(!is_pod()){
        reset();
    }
    if(variant.is_null()){
        data_.null_value=variant.get_null();
    }else if(variant.is_boolean()){
        data_.boolean_value=variant.get_boolean();
    }else if(variant.is_number()){
        data_.number_value=variant.get_number();
    }else{
        data_.nopod_value=variant.data_.nopod_value;
        variant.data_.nopod_value=nullptr;
    }
    data_.type_index=variant.get_type_index();
    variant.data_.null_value={};
    variant.data_.type_index=TypeIndex::TI_NULL;
    return *this;
}

Variant::~Variant(void){
    reset();
}

bool Variant::is_null(void)const{
    return data_.type_index==TypeIndex::TI_NULL;
}

bool Variant::is_boolean(void)const{
    return data_.type_index==TypeIndex::TI_BOOLEAN;
}

bool Variant::is_number(void)const{
    return data_.type_index==TypeIndex::TI_NUMBER;
}

bool Variant::is_string(void)const{
    return data_.type_index==TypeIndex::TI_STRING;
}

bool Variant::is_array(void)const{
    return data_.type_index==TypeIndex::TI_ARRAY;
}

bool Variant::is_object(void)const{
    return data_.type_index==TypeIndex::TI_OBJECT;
}

Null& Variant::get_null(void){
    if(!is_null()){
        throw Exception("type error");
    }
    return data_.null_value;
}

Boolean& Variant::get_boolean(void){
    if(!is_boolean()){
        throw Exception("type error");
    }
    return data_.boolean_value;
}

Number& Variant::get_number(void){
    if(!is_number()){
        throw Exception("type error");
    }
    return data_.number_value;
}

String& Variant::get_string(void){
    if(!is_string()){
        throw Exception("type error");
    }
    return *reinterpret_cast<String*>(data_.nopod_value);
}

Array& Variant::get_array(void){
    if(!is_array()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Array*>(data_.nopod_value);
}

Object& Variant::get_object(void){
    if(!is_object()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Object*>(data_.nopod_value);
}

Null const& Variant::get_null(void)const{
    if(!is_null()){
        throw Exception("type error");
    }
    return data_.null_value;
}

Boolean const& Variant::get_boolean(void)const{
    if(!is_boolean()){
        throw Exception("type error");
    }
    return data_.boolean_value;
}

Number const& Variant::get_number(void)const{
    if(!is_number()){
        throw Exception("type error");
    }
    return data_.number_value;
}

String const& Variant::get_string(void)const{
    if(!is_string()){
        throw Exception("type error");
    }
    return *reinterpret_cast<String const*>(data_.nopod_value);
}

Array const& Variant::get_array(void)const{
    if(!is_array()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Array const*>(data_.nopod_value);
}

Object const& Variant::get_object(void)const{
    if(!is_object()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Object const*>(data_.nopod_value);
}

Null const& Variant::get_const_null(void)const{
    if(!is_null()){
        throw Exception("type error");
    }
    return data_.null_value;
}

Boolean const& Variant::get_const_boolean(void)const{
    if(!is_boolean()){
        throw Exception("type error");
    }
    return data_.boolean_value;
}

Number const& Variant::get_const_number(void)const{
    if(!is_number()){
        throw Exception("type error");
    }
    return data_.number_value;
}

String const& Variant::get_const_string(void)const{
    if(!is_string()){
        throw Exception("type error");
    }
    return *reinterpret_cast<String const*>(data_.nopod_value);
}

Array const& Variant::get_const_array(void)const{
    if(!is_array()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Array const*>(data_.nopod_value);
}

Object const& Variant::get_const_object(void)const{
    if(!is_object()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Object const*>(data_.nopod_value);
}

TypeIndex Variant::get_type_index(void)const{
    return data_.type_index;
}

bool Variant::is_type_of(TypeIndex type_index)const{
    return get_type_index()==type_index;
}

Variant::Variant(TypeIndex type_index){
    if(type_index==TypeIndex::TI_NULL){
        data_.null_value={};
    }else if(type_index==TypeIndex::TI_BOOLEAN){
        data_.boolean_value=Boolean::FALSE;
    }else if(type_index==TypeIndex::TI_NUMBER){
        data_.number_value=0.0;
    }else{
        void* ptr=nullptr;
        if(type_index==TypeIndex::TI_STRING){
            ptr=new(std::nothrow) String{};
        }else if(type_index==TypeIndex::TI_ARRAY){
            ptr=new(std::nothrow) Array{};
        }else{//if(type_index==TypeIndex::TI_OBJECT)
            ptr=new(std::nothrow) Object{};
        }
        if(!ptr){
            throw Exception("bad alloc");
        }
        data_.nopod_value=ptr;
    }
    data_.type_index=type_index;
}

Variant::Variant(bool value)
    :Variant(value?Boolean::TRUE:Boolean::FALSE)
{}

Variant::Variant(int value)
    :Variant(static_cast<Number>(value))
{}

Variant::Variant(double value)
    :Variant(static_cast<Number>(value))
{}

Variant::Variant(char const* c_str)
    :Variant(String{c_str})
{}

Variant& Variant::operator[](std::size_t index){
    if(!is_array()){
        throw Exception("type error");
    }
    return get_array().at(index);
}

Variant& Variant::operator[](std::string_view key){
    if(!is_object()){
        throw Exception("type error");
    }
    return get_object().at(String{key});
}

Variant const& Variant::operator[](std::size_t index)const{
    if(!is_array()){
        throw Exception("type error");
    }
    return get_array().at(index);
}

Variant const& Variant::operator[](std::string_view key)const{
    if(!is_object()){
        throw Exception("type error");
    }
    return get_object().at(String{key});
}

namespace detail{

std::string string_dump(std::string const& str){
    static constexpr char hex_digits[]="0123456789ABCDEF";
    std::string ret;
    ret.reserve(str.size()*2+2);
    ret.push_back('"');
    for(unsigned char ch:str){
        switch(ch){
            case '"' :{ret+="\\\"";break;}
            case '\\':{ret+="\\\\";break;}
            case '/' :{ret+="\\/"; break;}
            case '\b':{ret+="\\b"; break;}
            case '\f':{ret+="\\f"; break;}
            case '\n':{ret+="\\n"; break;}
            case '\r':{ret+="\\r"; break;}
            case '\t':{ret+="\\t"; break;}
            default:{
                if(ch<0x20){//控制字符(0x00-0x1F)==>"\uXXXX"
                    ret+="\\u";
                    ret+=hex_digits[(ch>>12)&0x0F]; //高4位
                    ret+=hex_digits[(ch>>8)&0x0F];  //次高4位
                    ret+=hex_digits[(ch>>4)&0x0F];  //次低4位
                    ret+=hex_digits[ch&0x0F];       //低4位
                }else{
                    ret.push_back(static_cast<char>(ch));
                }
            }
        }
    }
    ret.push_back('"');
    return ret;
}

std::string number_dump(long double num){
    return std::format("{:g}",num);
}

}//namespace detail


std::string Variant::to_string(void)const{
    if(is_null()){
        return "null";
    }else if(is_boolean()){
        return get_boolean()==Boolean::TRUE
            ?"true"
            :"false";
    }else if(is_number()){
        return detail::number_dump(get_number());
    }else if(is_string()){
        return detail::string_dump(get_string());
    }else if(is_array()){
        std::string ret={"["};
        for(auto const& element:get_array()){
            ret+=element.to_string()+",";
        }
        ret.pop_back();//remove last ','
        ret+="]";
        return ret;
    }else{//if(is_object())
        std::string ret={"{"};
        for(auto const& [key,value]:get_object()){
            ret+=std::format(
                "{}:{},",detail::string_dump(key),value.to_string()
            );
        }
        ret.pop_back();//remove last ','
        ret+="}";
        return ret;
    }
}

bool Variant::is_pod(void)const{
    return is_null() || is_boolean() || is_number();
}

void Variant::reset(void){
    if(!is_pod()&&data_.nopod_value){
        if(is_string()){
            delete reinterpret_cast<String*>(data_.nopod_value);
        }else if(is_array()){
            delete reinterpret_cast<Array*>(data_.nopod_value);
        }else{//if(is_object())
            delete reinterpret_cast<Object*>(data_.nopod_value);
        }
        data_.nopod_value=nullptr;
    }
    data_.null_value={};
    data_.type_index=TypeIndex::TI_NULL;
}

}//namespace ccscript
