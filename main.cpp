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
    TSString oneLine = "int a=50; if(true){a=60;} for(int i=0; i<=10; i++){} os_print(\"Hello\");";
    
    TSRegist tsr;
    TSEvent::GetSingleTon()->RegistEvent("os_print",(void*)&tsr,(TpInstEventFun)&TSRegist::print);
    
    TSEngine tse;
    tse.DoString(oneLine);

    return a.exec();
}
