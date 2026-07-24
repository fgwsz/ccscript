#pragma once

#include<cstddef>       //std::size_t
#include<string_view>   //std::string_view
#include<string>        //std::string

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

    Variant(String &&);
    Variant(Array &&);
    Variant(Object &&);
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

    Null const& get_null(void)const;
    Boolean const& get_boolean(void)const;
    Number const& get_number(void)const;
    String const& get_string(void)const;
    Array const& get_array(void)const;
    Object const& get_object(void)const;

    Null const& get_const_null(void)const;
    Boolean const& get_const_boolean(void)const;
    Number const& get_const_number(void)const;
    String const& get_const_string(void)const;
    Array const& get_const_array(void)const;
    Object const& get_const_object(void)const;
    //类型下标
    TypeIndex get_type_index(void)const;
    bool is_type_of(TypeIndex)const;
    Variant(TypeIndex);
    //针对字面量
    Variant(bool);
    Variant(int);
    Variant(double);
    Variant(char const*);
    //元素访问
    Variant& operator[](std::size_t index);
    Variant& operator[](std::string_view key);

    Variant const& operator[](std::size_t index)const;
    Variant const& operator[](std::string_view key)const;
    //打印相关
    std::string to_string(void)const;
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
    bool is_pod(void)const;
    void reset(void);
};

}//namespace ccscript
