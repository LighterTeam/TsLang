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
#define TSVector std::vector

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
    TS_Void,            //无类型
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

//第一个内置函数
void TSPrint(TSString str){
    std::cout << "TS: " << str << std::endl;
}

class TSEngine {
public:
    TSEngine(){
        char* TSLangType[8] = {"var","int","float","double","int64","string","wstring","class"};
        for (int i = 0 ; i < 8 ; i++) {
            m_sTSLangType.insert(TSString(TSLangType[i]));
        }

        char* TSLangSymbol[11] = {"(",")","{","}","+","-","*","/","%",";",":"};

        for (int i = 0 ; i < 11 ; i++) {
            m_sTSLangSymbol.insert(TSString(TSLangSymbol[i]));
        }
    }

    void RunPrograme() {

    }

    void GetLineType(TSString& str){

    }

    void CallFunction(TSVector<TSString>& vctObj) {

    }

    void RunLine(TSString& str) {

    }

    void RunFile(TSString& str){
        int i = 0;
        char c = str[i];
        TSString line;
        bool once = false;
        bool stringprocess = false;
        while(str.size() > i) {
            c = str[i];
            i ++;

            if ( c == '\"' ) {
                if (stringprocess){
                    stringprocess = false;
                    m_FileTranslate.push_back(line);
                    m_FileTranslate.push_back(TSString("")+c);
                    TSPrint(line);
                    TSPrint(TSString("")+c);
                    line = "";
                } else {
                    stringprocess = true;
                    m_FileTranslate.push_back(TSString("")+c);
                    TSPrint(TSString("")+c);
                    line = "";
                }
                continue;
            }

            if(stringprocess){
                line += c;
                continue;
            }

            if ( c == ' ' || c == '\t'){
                if(once){
                    m_FileTranslate.push_back(line);
                    TSPrint(line);
                    once = false;
                    line = "";
                }
                continue;
            }
            else if ( m_sTSLangSymbol.count(TSString("")+c) ){
                if(line != ""){
                    m_FileTranslate.push_back(line);
                    TSPrint(line);
                }
                m_FileTranslate.push_back(TSString("")+c);
                TSPrint(TSString("")+c);
                once = false;
                line = "";
                continue;
            }

            once = true;
            line += c;
        }
    }

private:
    TSSet<TSString> m_sTSLangType; //用于比对类型字符串
    TSSet<TSString> m_sTSLangSymbol; //用于比对符号字符串
    TSVector<TSString> m_FileTranslate; //文章转义
};



int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TSString oneLine = "   void        print  ( string   str )   {    os.print(str);   }   print ( \"Hellod&^%#$*&&(*( )) World!\" )  ;";

    TSEngine tse;
    tse.RunFile(oneLine);

    return a.exec();
}
