#ifndef TSENGINE_H
#define TSENGINE_H

#include "TSDefine.h"
#include "TSObject.h"

class TSEngine {
public:
    TSEngine();
    void TSPrint(TSString str);
    void DoString(TSString& str);
    void DoFile(TSString path);
   
private:
    HRESULT CompilationLanguage(TSString &str, std::vector<TSString> &fileTranslate);
    HRESULT DoLanguage(std::vector<TSString> &fileTranslate); //把翻译过来的语句.增加到VM中.
    EN_TS_Type GetType(TSString &str);
    TSFunctionObject *ProcessFunc(std::vector<TSString>::iterator &iter);
        
private:
    TSSet<TSString> m_sTSLangType; //用于比对类型字符串
    TSSet<TSString> m_sTSLangSymbol; //用于比对符号字符串
    TSSet<TSString> m_sTSLangBaseType; //基础类型

//VM
private:
    TSMap<TSString, TSObject*> m_TypeList;
};

#endif // TSENGINE_H
