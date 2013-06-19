#include <QCoreApplication>
#include "TSCommon.h"

//第一个内置函数
void TSPrint(TSString str){
    std::cout << "TS: " << str << std::endl;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TSString oneLine = " class CA{public: void haha(){string str = \"HelloClass\"; print(str)} }  void        print  ( string   str )   {    os.print(str);   }   print ( \"Hellod&^%#$*&&(*( )) World!\" )  ;";

    TSEngine tse;
    tse.DoString(oneLine);

    return a.exec();
}
