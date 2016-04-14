#ifndef YQVALUE_H
#define YQVALUE_H
#include "YQCommon.h"
//TYQValue��������,֧��int��string,Ŀǰ����
typedef enum _EMValueType
{
	VALID_NONE,
	VALID_INT,
	VALID_STR
}EMValueType;

//TYQParam����ֵ
template <class C,typename VALTYPE>
class TYQValue
{
public:
	typedef void (C::*fSetType)();
	typedef void (C::*fSetValue)(const VALTYPE& val);
	TYQValue(C& c,fSetType st,fSetValue sv=0)
		:settype(st),setvalue(sv),Parent(c)
	{
	}
	~TYQValue()
	{
	}
public:	
	VALTYPE& operator=(const VALTYPE& val)
	{
		if (settype)
			(Parent.*settype)();
		if (setvalue)
			(Parent.*setvalue)(val);
		value = val;
		return value;
	}
	TYQValue& operator=(const TYQValue& org)
	{
		value = org.value;
		settype = org.settype;
		setvalue = org.setvalue;
		Parent = org.Parent;
	}
	operator VALTYPE& ()
	{
		return value;
	}
	operator const VALTYPE& () const
	{
		return value;
	}
private:
	VALTYPE value;
	fSetType settype;
	fSetValue setvalue;
	C& Parent;
};
#endif

