#ifndef TSENGINE_H
#define TSENGINE_H

#include "TSDefine.h"
#include "TSObject.h"

class TSEngine {
public:
    TSEngine();
    void TSPrint(TSString str);
    HRESULT DoString(TSString& str);
    HRESULT DoFile(TSString path);
    
    HRESULT RunConditon(std::map<TSString, TSObject *> *pSS, std::vector<TSString>::iterator &iter, int &offset);
    HRESULT RunLoop(std::map<TSString, TSObject *> *pSS, std::vector<TSString>::iterator &iter, int &offset);


    HRESULT isStackName(std::list<std::map<TSString, TSObject *> *> &sCurSs, TSString sName);
    HRESULT getStackObject(std::list<std::map<TSString, TSObject *> *> &sCurSs, TSString sName, TSObject **pObj);
private:
    HRESULT RunCreateInstance(TSBaseObject *pOB, std::vector<TSString>::iterator &iter, int &offset);
    HRESULT CompilationLanguage(TSString &str, std::vector<TSString> &fileTranslate);
    HRESULT DoLanguage(std::vector<TSString> &fileTranslate, std::vector<TSString> &runLine); //把翻译过来的语句.增加到VM中.
    EN_TS_Type GetType(TSString &str);
    TSFunctionObject *ProcessFunc(std::vector<TSString>::iterator &iter, int &offset);
    HRESULT RunScript(TSVector<TSString>& runLine);
    HRESULT RunFunction(TSFunctionObject *pOF, std::vector<TSString>::iterator& iter, int &offset);
        
private:
    TSSet<TSString> m_sTSLangType; //用于比对类型字符串
    TSSet<TSString> m_sTSLangSymbol; //用于比对符号字符串
    TSSet<TSString> m_sTSLangBaseType; //基础类型
    TSSet<TSString> m_sTSLangCondition; //条件类型循环类型
    TSSet<TSString> m_sTSLangLoop; //循环类型
    
//VM
private:
    TSMap<TSString, TSObject*> m_TypeList; //用户定义的类型
    int m_iStackDeep;
    
    TSList<TSMap<TSString, TSObject*>> m_vInstanceStacks;
    TSMap<TSString, TSObject*>* m_vCurStack;
};

#endif // TSENGINE_H
