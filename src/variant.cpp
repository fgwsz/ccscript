#pragma once

#include<ccscript/variant.hpp>

namespace ccscript{

Variant::Variant(Null const&){
    data_.type_index=TypeIndex::TI_NULL;
}

Variant::Variant(Boolean const& value){
    data_.boolean_value=value;
    data_.type_index=TypeIndex::TI_BOOLEAN;
}

Variant::Variant(Number const& value){
    data_.number_value=value;
    data_.type_index=TypeIndex::TI_NUMBER;
}

Variant::Variant(String const& value){
    auto* ptr=new String{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_STRING;
}

Variant::Variant(Array const& value){
    auto* ptr=new Array{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_ARRAY;
}

Variant::Variant(Object const& value){
    auto* ptr=new Object{value};
    if(!ptr){
        throw Exception("bad alloc");
    }
    data_.nopod_value=ptr;
    data_.type_index=TypeIndex::TI_OBJECT;
}

Variant::Variant(void)
    :Variant(Null{}){
}

Variant::Variant(Variant const&){
    if()
}

Variant::Variant(Variant &&){
    
}

Variant::Variant& operator=(Variant const& variant){
    if(this==&variant){
        return *this;
    }
    if(variant.is_pod()){
        if(is_pod){
            reset();
        }
        data_=variant.data_;
        return *this;
    }
    //TODO
    if(is_pod()){
        
        return *this;
    }
    if(get_type_index()==)
}

Variant::Variant& operator=(Variant &&){

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
    if(!this->is_null()){
        throw Exception("type error");
    }
    return data_.null_value;
}

Boolean& Variant::get_boolean(void){
    if(!this->is_boolean()){
        throw Exception("type error");
    }
    return data_.boolean_value;
}

Number& Variant::get_number(void){
    if(!this->is_number()){
        throw Exception("type error");
    }
    return data_.number_value;
}

String& Variant::get_string(void){
    if(!this->is_string()){
        throw Exception("type error");
    }
    return *reinterpret_cast<String*>(data_.nopod_value);
}

Array& Variant::get_array(void){
    if(!this->is_array()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Array*>(data_.nopod_value);
}

Object& Variant::get_object(void){
    if(!this->is_object()){
        throw Exception("type error");
    }
    return *reinterpret_cast<Object*>(data_.nopod_value);
}

TypeIndex TypeIndex::get_type_index(void)const{
    return data_.type_index;
}

bool TypeIndex::is_type_of(TypeIndex type_index)const{
    return get_type_index()==type_index;
}

bool Variant::is_pod(void){
    return is_null() || is_boolean() || is_number();
}

void Variant::reset(void){
    switch(data_.type_index){
        case TypeIndex::TI_NULL:{break;}
        case TypeIndex::TI_BOOLEAN:{break;}
        case TypeIndex::TI_NUMBER:{break;}
        case TypeIndex::TI_STRING:{
            delete reinterpret_cast<String*>(data_.nopod_value);
            break;
        }
        case TypeIndex::TI_ARRAY:{
            delete reinterpret_cast<Array*>(data_.nopod_value);
            break;
        }
        case TypeIndex::TI_OBJECT:{
            delete reinterpret_cast<Object*>(data_.nopod_value);
            break;
        }
    }
}

}//namespace ccscript
