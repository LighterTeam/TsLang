#ifndef TSENGINE_H
#define TSENGINE_H

#include "TSDefine.h"
#include "TSObject.h"

class TSEngine {
public:
    TSEngine();
    void TSPrint(TSString str);
    void CompilationLanguage(TSString& str);

private:
    TSSet<TSString> m_sTSLangType; //用于比对类型字符串
    TSSet<TSString> m_sTSLangSymbol; //用于比对符号字符串
    TSVector<TSString> m_FileTranslate; //文章转义
};

#endif // TSENGINE_H
