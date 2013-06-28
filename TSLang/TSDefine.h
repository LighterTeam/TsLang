#ifndef TSDEFINE_H
#define TSDEFINE_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>

typedef std::string TSString;
typedef std::wstring TSWString;
#define TSMap std::map
#define TSSet std::set
#define TSVector std::vector
#define TSList std::list

// Error定义
enum HRESULT {
    S_OK,
    S_ERROR,
    S_True,
    S_False,
};

// 定义TS实例类型
enum EN_TS_Type{
    TS_Type_Min,
    TS_var,             //动态变量
    TS_int,             //整形
    TS_float,           //浮点数
    TS_double,          //双浮点
    TS_int64,           //64位整形
    TS_string,          //字符串UTF-8
    TS_wstring,         //宽字符UTF-16
    TS_class,           //类
    TS_interface,       //接口
    TS_function,        //函数
    TS_void,            //无类型
    TS_Type_Max
};

// 定义TS类规则
enum EN_TS_ClassRule{
    TS_ClassRule_Min,
    TS_ClassPublic,     //公有
    TS_ClassProtect,    //保护
    TS_ClassPrivate,    //私有
    TS_ClassRule_Max
};


#endif // TSDEFINE_H
