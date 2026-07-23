#pragma once

#include<ccscript/type.hpp>
#include<ccscript/type_index.hpp>

namespace ccscript{

class Variant{
public:
    //装箱
    Variant(Null);
    Variant(Boolean);
    Variant(Number);
    Variant(String const&);
    Variant(Array const&);
    Variant(Object const&);
    //生命周期
    Variant(void);
    Variant(Variant const&);
    Variant(Variant &&);
    Variant& operator=(Variant const&);
    Variant& operator=(Variant &&);
    ~Variant(void);
    //类型识别
    bool is_null(void)const;
    bool is_boolean(void)const;
    bool is_number(void)const;
    bool is_string(void)const;
    bool is_array(void)const;
    bool is_object(void)const;
    //拆箱
    Null& get_null(void);
    Boolean& get_boolean(void);
    Number& get_number(void);
    String& get_string(void);
    Array& get_array(void);
    Object& get_object(void);
    //类型下标
    TypeIndex get_type_index(void)const;
    bool is_type_of(TypeIndex)const;
private:
    struct{
        TypeIndex type_index;
        union{
            Null null_value;
            Boolean boolean_value;
            Number number_value;
            void* nopod_value;
        };
    }data_;
    bool is_pod(void);
    void reset(void);
};

}//namespace ccscript
