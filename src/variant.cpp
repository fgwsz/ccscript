#include<new>

#include<ccscript/exception.hpp>

#include<ccscript/variant.hpp>

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
    auto* ptr=new(std::nothrow) String{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_STRING;
}

Variant::Variant(Array const& value){
    auto* ptr=new(std::nothrow) Array{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_ARRAY;
}

Variant::Variant(Object const& value){
    auto* ptr=new(std::nothrow) Object{value};
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
        data_=variant.data_;
        return;
    }
    void* ptr=nullptr;
    if(variant.is_string()){
        ptr=new(std::nothrow) String{variant.get_string()};
    }else if(variant.is_array()){
        ptr=new(std::nothrow) Array{variant.get_array()};
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
    data_=variant.data_;
    variant.reset();
}

Variant& Variant::operator=(Variant const& variant){
    if(this==&variant){
        return *this;
    }
    if(variant.is_pod()){
        if(!is_pod()){
            reset();
        }
        data_=variant.data_;
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
        ptr=new(std::nothrow) Array{variant.get_array()};
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
    data_=variant.data_;
    variant.reset();
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
        data_.boolean_value=false;
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
    }
    data_.nopod_value=nullptr;
    data_.type_index=TypeIndex::TI_NULL;
}

}//namespace ccscript
