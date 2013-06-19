#include "TSEngine.h"

TSEngine::TSEngine()
{
    char* TSLangType[8] = {"var","int","float","double","int64","string","wstring","class"};
    for (int i = 0 ; i < 8 ; i++) {
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

void TSEngine::CompilationLanguage(TSString& str){
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
