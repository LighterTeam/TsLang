#include <QCoreApplication>
#include "TSCommon.h"

//第一个内置函数
void TSPrint(TSString str){
    std::cout << "TS: " << str << std::endl;
}

class TSRegist {
public:
    TSRegist(){
        
    }
    
    void print(std::string sBuffer){
        cout << "JS: " << sBuffer << endl;
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TSString oneLine = "int TSFuc(int a, int b){int c = 0; c = a + b; return c;}  int a=TSFuc(5,6); print(\"What! Fack!!!!\");  print(\"Awesome!!!!\"); CA ca = new CA(); ca.haha(15,16); class CA{public: void haha(int a, int b){ if (a == 50) { print(\"hahaha\") }  string str = \"HelloClass\"; print(str);} private: int a = 50; int b;}  void        print  ( string   str )   { os_print(str); os_print(str);   }   void main(){ print( \"Hellod&^%#$*&&(*( )) World!\" );  }  int a=50; a = 60; int b = 50; print(\"HelloWorld!\");";
    
    TSRegist tsr;
    TSEvent::GetSingleTon()->RegistEvent("os_print",(void*)&tsr,(TpInstEventFun)&TSRegist::print);
    
    TSEngine tse;
    tse.DoString(oneLine);

    return a.exec();
}
