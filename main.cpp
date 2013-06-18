#include <QCoreApplication>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

typedef std::string TSString;
typedef std::wstring TSWString;
#define TSMap std::map
#define TSSet std::set

// 定义TS实例类型
enum EN_TS_Type{
    TS_var,             //动态变量
    TS_int,             //整形
    TS_float,           //浮点数
    TS_double,          //双浮点
    TS_int64,           //64位整形
    TS_string,          //字符串UTF-8
    TS_wstring,         //宽字符UTF-16
    TS_class,           //类
    TS_interface,       //接口
    TS_Function,        //函数
    TS_Type_Max
};

// 定义TS类规则
enum EN_TS_ClassRule{
    TS_ClassPublic,     //公有
    TS_ClassProtect,    //保护
    TS_ClassPrivate,    //私有
    TS_ClassRule_Max
};

// 类型基类
class TSObject{
public:
    EN_TS_Type m_iType;
    TSString m_sName;
};

class TSBaseObject : public TSObject {
public:
    TSString m_Value;
};

class TSClassObject : public TSObject{
public:
    TSMap<EN_TS_ClassRule, TSMap<EN_TS_Type, TSSet<TSObject*>>> m_Info;
};

class TSEngine {
public:
    TSEngine(){
        char* TSLangType[8] = {"var","int","float","double","int64","string","wstring","class"};
        for (int i = 0 ; i < 8 ; i++) {
            m_sTSLangType.insert(std::string(TSLangType[i]));
        }
    }

    int TST_StringFilt(std::string& lpszString, char szSeps, std::vector<std::string>& tArray)
    {
        char* pTok = NULL;
        char* sz = new char[lpszString.length() + 2];
        strcpy(sz, lpszString.c_str());
        pTok = strtok(sz, &szSeps);
        while ( pTok )
        {
            tArray.push_back(pTok);
            pTok = strtok(NULL, &szSeps);
        }

        if (tArray.size() == 0)
        {
            tArray.push_back(lpszString);
        }

        delete [] sz;
        return 0;
    }

    void RunPrograme() {

    }

    void GetLineType(str){

    }

    void RunLine(TSString& str) {
        std::cout << str << std::endl;

    }

    void RunFile(TSString& str){
        int i = 0;
        char c = str[i];
        TSString line;
        int begin = 0;
        while((int)str.size() > i) {
            c = str[i];
            i ++;
            if(begin == 0 && (c == ' ' || c == '\11' || c == '\n')){
                continue;
            }
            begin = 1;
            line += c;
            if (c == ';') {
                RunLine(line);
                line = "";
                begin = 0;
            }
        }
    }

private:
    TSSet<TSString> m_sTSLangType; //用于比对类型字符串
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TSString oneLine = "int a = 50; a = 60; CA ca = new CA(); ca.haha = 50; main";

    TSEngine tse;
    tse.RunFile(oneLine);

    return a.exec();
}
