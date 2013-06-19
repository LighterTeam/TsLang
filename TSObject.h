#ifndef TSOBJECT_H
#define TSOBJECT_H

#include "TSDefine.h"

// 类型基类
class TSObject{
public:
    EN_TS_Type m_iType;
    TSString m_sName;
};

class TSBaseObject : public TSObject {
public:
    TSString m_Value;
};

class TSClassObject : public TSObject{
public:
    TSMap<EN_TS_ClassRule, TSMap<EN_TS_Type, TSSet<TSObject*>>> m_Info;
};

class TSFunctionObject : public TSObject{
public:
    EN_TS_Type m_iTypeReturn; //返回值类型
    TSVector<TSObject*> m_toParameter;  //参数
    TSMap<EN_TS_Type, TSSet<TSObject*>> m_Info;
};
#endif // TSOBJECT_H
