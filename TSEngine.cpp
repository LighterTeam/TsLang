#include "TSEngine.h"

TSEngine::TSEngine()
{
    char* TSLangType[9] = {"var","int","float","double","int64","string","wstring","class","void"};
    for (int i = 0 ; i < 9 ; i++) {
        m_sTSLangType.insert(TSString(TSLangType[i]));
    }
    
    char* TSLangBaseType[8] = {"var","int","float","double","int64","string","wstring","void"};
    for (int i = 0 ; i < 8 ; i++) {
        m_sTSLangBaseType.insert(TSString(TSLangType[i]));
    }
    
    char* TSLangSymbol[12] = {"(",")","{","}","+","-","*","/","%",";",":",","};
    for (int i = 0 ; i < 12 ; i++) {
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
    EN_TS_ClassRule curClassRule = TS_ClassRule_Min;

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
                    curClassRule = TS_ClassRule_Min;
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
                    curClassRule = TS_ClassPublic;
                } else if (info == "protect"){
                    curClassRule = TS_ClassProtect;
                } else if (info == "private"){
                    curClassRule = TS_ClassPrivate;
                }
                if (curClassRule != TS_ClassRule_Min){
                    if(m_sTSLangType.count(info)) {
                        int offset = 0;
                        if(*(iter+2) == "("){ //判定为函数
                            TSPrint("Fun:" + *(iter+1));
                            TSFunctionObject* _pO = ProcessFunc(iter, offset);
                            pCO->m_Info[curClassRule][TS_int].insert(_pO);
                            offset --;
                        } else if (*(iter+2) == "=") { //判定为变量声明
                            TSPrint("Instanse:" + *(iter+0) + "," + *(iter+1) + "," + *(iter+2) + "," + *(iter+3));
                            TSBaseObject* _pO = new TSBaseObject();
                            _pO->m_iType = GetType(info);
                            _pO->m_sName = *(iter+1);
                            _pO->m_Value = *(iter+3);
                            pCO->m_Info[curClassRule][TS_int].insert(_pO);
                            offset = 4;
                        } else if (*(iter+2) == ";") { //判定为变量声明
                            TSPrint("Instanse:" + *(iter+0) + "," + *(iter+1) + "," + *(iter+2));                            
                            TSBaseObject* _pO = new TSBaseObject();
                            _pO->m_iType = GetType(info);
                            _pO->m_sName = *(iter+1);
                            _pO->m_Value = "0";
                            pCO->m_Info[curClassRule][TS_int].insert(_pO);
                            offset = 2;
                        }
                        iter += offset;
                    }
                }
                break;
            }
        }
    }
    return S_OK;
}

TSFunctionObject* TSEngine::ProcessFunc(std::vector<TSString>::iterator& iter, int& offset){
    TSFunctionObject* pFo = new TSFunctionObject();
    pFo->m_sTypeReturn = *(iter);
    pFo->m_iType = TS_function;
    pFo->m_sName = *(iter + 1);
    
    int i = 2;
    int stack = 0;
    
    bool bParaOnce = true;
    
    bool bParaOnce_PO = true;
    TSObject* pO = nullptr;
    int stackPO = 0;
    while(true){
        TSString info = *(iter + i);
        i++;
        
        if (bParaOnce) {
            if (info == "(") {

            } else {
                if (info == ")") {
                    pFo->m_toParameter.push_back(pO);
                    bParaOnce = false;
                    stackPO = 0;
                }
                else if (bParaOnce_PO) {
                    if (m_sTSLangBaseType.count(info)){
                        pO = new TSBaseObject();
                        pO->m_iType = GetType(info);
                        pO->m_sName = *(iter + i);
                    }
                    else {
                        pO = new TSClassObject();
                        pO->m_iType = TS_class;
                        pO->m_sName = *(iter + i);
                    }                    
                    
                    bParaOnce_PO = false;
                    stackPO = 1;
                }
                else if (info == ",") {
                    pFo->m_toParameter.push_back(pO);
                    bParaOnce_PO = true;
                    stackPO = 0;
                }
            }
        }
        else {
            pFo->m_Body.push_back(info);
            if (info == "{") {
                stack++;
            } else if (info == "}") {
                stack--;
                if(stack <= 0) {
                    break;
                }
            }
        }
    }
    offset = i;
    return pFo;
}

EN_TS_Type TSEngine::GetType(TSString& str){
    if("var" == str){
        return TS_var;
    }
    else if ("int" == str){
        return TS_int;
    }
    else if ("float" == str){
        return TS_float;
    }
    else if ("double" == str){
        return TS_double;
    }
    else if ("int64" == str){
        return TS_int64;
    }
    else if ("string" == str){
        return TS_string;
    }
    else if ("wstring" == str){
        return TS_wstring;
    }
    else if ("class" == str){
        return TS_class;
    }
    else if ("void" == str) {
        return TS_void;
    }
    else if ("interface" == str) {
        return TS_interface;
    }
    return TS_Type_Min;
}
