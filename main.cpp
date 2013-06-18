#include <QCoreApplication>

#include <string>
#include <vector>
#include <map>
typedef std::string TSString;
typedef std::wstring TSWString;
typedef std::map TSMap;

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

// 类型基类
class TSType{
public:
    EN_TS_Type m_iType;
    TSString m_sName;
};

class TSBaseType : public TSType {
public:

};

class TSClassType : public TSType{
public:
    TSMap<EN_TS_ClassRule, TSMap<EN_TS_Type, TSString>> m_Info;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    return a.exec();
}
