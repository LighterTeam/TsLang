#include "TSEngine.h"
#include "TSEvent.h"

TSEngine::TSEngine() : m_iStackDeep(0)
{
    char* TSLangType[9] = {"var","int","float","double","int64","string","wstring","class","void"};
    for (int i = 0 ; i < 9 ; i++) {
        m_sTSLangType.insert(TSString(TSLangType[i]));
    }
    
    char* TSLangBaseType[8] = {"var","int","float","double","int64","string","wstring","void"};
    for (int i = 0 ; i < 8 ; i++) {
        m_sTSLangBaseType.insert(TSString(TSLangType[i]));
    }
    
    char* TSLangSymbol[17] = {"(",")","{","}","+","-","*","/","%",";",":",",",".","=","<",">","!"};
    for (int i = 0 ; i < 17 ; i++) {
        m_sTSLangSymbol.insert(TSString(TSLangSymbol[i]));
    }
    
    char* TSLangCondition[] = {"if","else","switch"};
    for (int i = 0 ; i < 3 ; i++) {
        m_sTSLangCondition.insert(TSString(TSLangCondition[i]));
    }
    
    char* TSLangLoop[] = {"while","do","for"};
    for (int i = 0 ; i < 3 ; i++) {
        m_sTSLangLoop.insert(TSString(TSLangLoop[i]));
    }
    
    TSMap<TSString, TSObject*> vInstanceStack;
    m_vInstanceStacks.push_back(vInstanceStack);
    m_vCurStack = &(*m_vInstanceStacks.begin());
}

void TSEngine::TSPrint(TSString str){
    std::cout << "Sys: " << str << std::endl;
}

HRESULT TSEngine::DoString(TSString &str)
{
    TSVector<TSString> fileTranslate; //文章转义
    TSVector<TSString> runLine;
    CompilationLanguage(str, fileTranslate);
    TSPrint("====================================================================");
    DoLanguage(fileTranslate, runLine);
    RunScript(runLine);
    return S_OK;
}

HRESULT TSEngine::RunScript(TSVector<TSString>& runLine) {
    TSMap<TSString, TSObject*> vInstanceStack;
    
    for (auto iter = runLine.begin(); iter != runLine.end(); iter++) {
        int offset = 0;
        TSString& info = *iter;
        if(m_TypeList.count(info)){ //如果是自定义的类型
            TSObject* pObj = m_TypeList[info];
            if(pObj->m_iType == TS_function){
                TSFunctionObject* pOF = (TSFunctionObject*)pObj;
                RunFunction(pOF, iter, offset);
                
                //如果pOF里面有返回值.则放到堆栈里.赋值给实例变量.
                iter += offset;
            }
            else if (pObj->m_iType == TS_class){
                TSClassObject* pOC = (TSClassObject*)pObj;
                
            }
        } 
        else if (m_sTSLangBaseType.count(info)) { //如果是基础类型
            TSBaseObject* pOB = new TSBaseObject();
            RunCreateInstance(pOB, iter, offset);
            vInstanceStack[pOB->m_sName] = pOB;
            iter += offset;
        }
        else if (m_sTSLangCondition.count(info)) {
            RunConditon(&vInstanceStack, iter, offset);
            iter += offset;
        }
        else if (m_sTSLangLoop.count(info)) {
            RunLoop(&vInstanceStack, iter, offset);
            iter += offset;
        }
    }
    
    m_vInstanceStacks.push_back(vInstanceStack);
    return S_OK;
}

HRESULT TSEngine::RunConditon(TSMap<TSString, TSObject*>* pSS, TSVector<TSString>::iterator& iter, int& offset){
    int index = 0;
    
    
    
    
    
    return S_OK;
}

HRESULT TSEngine::RunLoop(TSMap<TSString, TSObject*>* pSS, TSVector<TSString>::iterator& iter, int& offset){
    int index = 0;
       
    
    
    
    return S_OK;
}

HRESULT TSEngine::RunCreateInstance(TSBaseObject* pOB, TSVector<TSString>::iterator& iter, int& offset){
    if ( *(iter + 2) == "=") {
        pOB->m_iType = GetType(*iter);
        pOB->m_sName = *(iter + 1);
        pOB->m_Value = *(iter + 3);
        offset = 5;
    }
    else {
        pOB->m_iType = GetType(*iter);
        pOB->m_sName = *(iter + 1);
        offset = 3;
    }
    
    return S_OK;
}

HRESULT TSEngine::RunFunction(TSFunctionObject* pOF, TSVector<TSString>::iterator& iter, int& offset){
    TSEvent* pEvent = TSEvent::GetSingleTon();
    
    int index = 1;
    TSVector<TSString> vParam;
    while(true) {
        TSString& info = *(iter + index);
        index ++;        
        if(info == "("){
            
        } 
        else if(info == ")"){
            
        }
        else if(info == ";") {
            break;
        }
        else if(info == "," || info == "\"") {
            
        }
        else {
            vParam.push_back(info);
        }
    }
    offset = --index; //Must!!!
    
    if (vParam.size() != pOF->m_toParameter.size()) {
        return S_ERROR;
    }
    
    //执行函数
    int stack = 0;
    index = 0;
    for (auto iter = pOF->m_toParameter.begin(); iter != pOF->m_toParameter.end(); iter++) {
        TSObject* pObj = *iter;
        if(pObj->m_iType == TS_string){
            TSBaseObject* pOB = (TSBaseObject*)pObj;
            pOB->m_Value = vParam[index];
        }
        index++;
    }
    
    for (auto iter = pOF->m_Body.begin(); iter != pOF->m_Body.end(); iter++) {
        TSString& info = *iter;
        if(info == "{"){
            stack ++;
        }
        else if(info == "}"){
            stack --;
            if (stack <= 0) {
                break;
            }
        }
        
        if("os_print" == info){
            TSBaseObject* pOB = (TSBaseObject*)pOF->m_toParameter[0];
            pEvent->SendMsg(info, pOB->m_Value);
        }
    }
    
    return S_OK;
}

HRESULT TSEngine::DoFile(TSString path)
{
    return S_OK;
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

HRESULT TSEngine::DoLanguage(std::vector<TSString>& fileTranslate, TSVector<TSString>& runLine) {
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
        } else if (info == "class"){ //判定为类声明
            curType = TS_class;
            pObj = new TSClassObject();
            continue;
        } else if(m_sTSLangType.count(info)) {
            int offset = 0;
            if(*(iter+2) == "("){ //判定为函数
                TSPrint("Fun:" + *(iter+1));
                TSFunctionObject* _pO = ProcessFunc(iter, offset);
                m_TypeList[_pO->m_sName] = _pO;
                offset --;
            } 
            else {
                runLine.push_back(info);
            }
            /*else if (*(iter+2) == "=") { //判定为变量声明
                TSPrint("Instanse:" + *(iter+0) + "," + *(iter+1) + "," + *(iter+2) + "," + *(iter+3));
                TSBaseObject* _pO = new TSBaseObject();
                _pO->m_iType = GetType(info);
                _pO->m_sName = *(iter+1);
                _pO->m_Value = *(iter+3);
                m_TypeList[_pO->m_sName] = _pO;
                offset = 4;
            } else if (*(iter+2) == ";") { //判定为变量声明
                TSPrint("Instanse:" + *(iter+0) + "," + *(iter+1) + "," + *(iter+2));                            
                TSBaseObject* _pO = new TSBaseObject();
                _pO->m_iType = GetType(info);
                _pO->m_sName = *(iter+1);
                _pO->m_Value = "0";
                m_TypeList[_pO->m_sName] = _pO;
                offset = 2;
            }*/
            iter += offset;
            continue;
        }
        else{
            runLine.push_back(info);
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
                            TSPrint("Class Fun:" + *(iter+1));
                            TSFunctionObject* _pO = ProcessFunc(iter, offset);
                            pCO->m_Info[curClassRule][TS_int].insert(_pO);
                            offset --;
                        } else if (*(iter+2) == "=") { //判定为变量声明
                            TSPrint("Class Instanse:" + *(iter+0) + "," + *(iter+1) + "," + *(iter+2) + "," + *(iter+3));
                            TSBaseObject* _pO = new TSBaseObject();
                            _pO->m_iType = GetType(info);
                            _pO->m_sName = *(iter+1);
                            _pO->m_Value = *(iter+3);
                            pCO->m_Info[curClassRule][TS_int].insert(_pO);
                            offset = 4;
                        } else if (*(iter+2) == ";") { //判定为变量声明
                            TSPrint("Class Instanse:" + *(iter+0) + "," + *(iter+1));                         
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
    TSPrint("====================================================================");
    for (int i = 0 ; i < runLine.size(); i++) {
        TSPrint(runLine[i]);
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
        TSString& info = *(iter + i);
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
