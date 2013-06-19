#include "TSEngine.h"

TSEngine::TSEngine()
{
    char* TSLangType[9] = {"var","int","float","double","int64","string","wstring","class","void"};
    for (int i = 0 ; i < 9 ; i++) {
        m_sTSLangType.insert(TSString(TSLangType[i]));
    }

    char* TSLangSymbol[11] = {"(",")","{","}","+","-","*","/","%",";",":"};

    for (int i = 0 ; i < 11 ; i++) {
        m_sTSLangSymbol.insert(TSString(TSLangSymbol[i]));
    }
}

void TSEngine::TSPrint(TSString str){
    std::cout << "Sys: " << str << std::endl;
}

void TSEngine::DoString(TSString &str)
{
    TSVector<TSString> fileTranslate; //文章转义
    CompilationLanguage(str, fileTranslate);
    DoLanguage(fileTranslate);
}

void TSEngine::DoFile(TSString path)
{

}

HRESULT TSEngine::CompilationLanguage(TSString& str,
                                      TSVector<TSString>& fileTranslate){
    int i = 0;
    char c = str[i];
    TSString line;
    bool once = false;
    bool stringprocess = false;
    while((int)str.size() > i) {
        c = str[i];
        i ++;

        if ( c == '\"' ) {
            if (stringprocess){
                stringprocess = false;
                fileTranslate.push_back(line);
                fileTranslate.push_back(TSString("")+c);
                TSPrint(line);
                TSPrint(TSString("")+c);
                line = "";
            } else {
                stringprocess = true;
                fileTranslate.push_back(TSString("")+c);
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
                fileTranslate.push_back(line);
                TSPrint(line);
                once = false;
                line = "";
            }
            continue;
        }
        else if ( m_sTSLangSymbol.count(TSString("")+c) ){
            if(line != ""){
                fileTranslate.push_back(line);
                TSPrint(line);
            }
            fileTranslate.push_back(TSString("")+c);
            TSPrint(TSString("")+c);
            once = false;
            line = "";
            continue;
        }

        once = true;
        line += c;
    }

    return S_OK;
}



HRESULT TSEngine::DoLanguage(std::vector<TSString> &fileTranslate) {
    EN_TS_Type curType = TS_Type_Min;
    int curStack = 0;
    TSString curClassRule = "";

    //临时函数处理
    TSObject* pFunc = nullptr;

    //全局Obj.
    TSObject* pObj = nullptr;
    for (auto iter = fileTranslate.begin(); iter != fileTranslate.end(); iter++) {
        TSString& info = *iter;

        if (curType != TS_Type_Min) { //安全判断.
            if (pObj == nullptr) {
                std::cout << "pObj == nullptr" << std::endl;
                return S_ERROR;
            }
        } else if (info == "class"){
            curType = TS_class;
            pObj = new TSClassObject();
            continue;
        }

        if (curType == TS_class) {
            TSClassObject* pCO = (TSClassObject*)pObj;

            if (info == "{") {
                curStack ++;
            } else if (info == "}") {
                curStack --;
                if (curStack <= 0) {
                    curType = TS_Type_Min;
                    pObj = nullptr;
                    curClassRule = "";
                    m_TypeList[pCO->m_sName] = pCO;
                    continue;
                }
            }

            switch (curStack){
            case 0:
                pCO->m_sName = info;
                pCO->m_iType = TS_class;
                break;
            case 1:
                if(info == "public"){
                    curClassRule = info;
                } else if (info == "protect"){
                    curClassRule = info;
                } else if (info == "private"){
                    curClassRule = info;
                }
                if (curClassRule != ""){
                    if(m_sTSLangType.count(info)) {
                        if(*(iter+2) == "("){ //判定为函数
                            TSPrint("Fun:" + *(iter+1));
                        } else if (*(iter+2) == "=" || *(iter+2) == ";") { //判定为变量声明
                            TSPrint("Instanse:" + *(iter+1));
                        }
                    }
                }
                break;
            }
        }
    }
    return S_OK;
}
