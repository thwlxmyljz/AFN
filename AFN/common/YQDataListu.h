//---------------------------------------------------------------------------
#ifndef YQDataListuH
#define YQDataListuH

#include "Threadu.h"
#include "yqlist.h"
/*---------------------------------------------------------------------------
 *
 * TYQDataList �ṩ���߳�ʱͬ������д�������������
 * TDataType ������ָ��
 *
 ---------------------------------------------------------------------------*/
template<class TDataType>
class TYQDataList
{
private:
	bool FCreateSync;
    int FDataPtr;
void  Init(TCriticalSection* Sync);
protected:
    YQList*            FDatas;
    TCriticalSection* FSync;
public:
    TYQDataList();
    TYQDataList(TCriticalSection* Sync);		//ʹ���ⲿ��
    virtual ~TYQDataList();
    /*
     * ReadData ��ȡ���ݣ����������б���ɾ���ü�¼���б�Ϊ��ʱ���ؿ�ָ�롣
     * ReadData �ĵ�����ʹ���귵�ص�ָ�������ͷŸÿռ䣬���������ڴ�й©��
     * StepGetData ÿ��ȡһ�����ݲ�����������1��
     * DataItem() ��ȡ���ݵ���ɾ���б������߲����ͷŷ��صĿռ�
     */
    TDataType* ReadData();   //����������ݣ����ݲ�����ʱ����NULL
    TDataType* ReadData(const int Item);   //���ݲ�����ʱ����NULL
    TDataType* StepGetData(int* Item);   //�б�������ʱ����NULL
    TDataType* DataItem(const int Item);  //���ݲ�����ʱ����NULL

    void DeleteData(int Item);

    void Lock();
    void UnLock();

    //��������
    TDataType* CreateData();
    //д������,Data�Ŀռ��ɵ������ͷ�
    void WriteData(const TDataType& Data);
};
//---------------------------------------------------------------------------
template<class TDataType>
 TYQDataList<TDataType>::TYQDataList()
{
    Init(NULL);
}
//---------------------------------------------------------------------------
template<class TDataType>
TYQDataList<TDataType>::TYQDataList(TCriticalSection* Sync)
{
    Init(Sync);
}
//---------------------------------------------------------------------------
template<class TDataType>
TYQDataList<TDataType>::~TYQDataList()
{
    TDataType* Data;
    while((Data = ReadData()) != NULL)
        delete Data;
    if (FCreateSync)
    	delete FSync;
    delete FDatas;
}
//---------------------------------------------------------------------------
template<class TDataType>
void TYQDataList<TDataType>::Init(TCriticalSection* Sync)
{
    if (Sync)
    {
    	FSync = Sync;
        FCreateSync = false;
    }
    else
    {
    	FSync = new TCriticalSection();
        FCreateSync = true;
    }
    FDatas = new YQList();
    FDataPtr = 0;
}
//---------------------------------------------------------------------------
template<class TDataType>
TDataType* TYQDataList<TDataType>::DataItem(const int Item)
{
    if (Item < FDatas->Count())
    	return (TDataType*)FDatas->Items(Item);
    else
    	return NULL;
}
//---------------------------------------------------------------------------
template<class TDataType>
TDataType* TYQDataList<TDataType>::ReadData()
{
 	return ReadData(0);
}
//---------------------------------------------------------------------------
template<class TDataType>
TDataType* TYQDataList<TDataType>::ReadData(const int Item)
{
    TDataType* Data = NULL;
    FSync->Enter();
    try
    {
        if (Item < FDatas->Count())
        {
            Data = DataItem(Item);
            FDatas->RemoveElement(Item);
//            delete 
        }
    }
    catch(...)
    {
    
    }

    FSync->Leave();

    return Data;
}
//---------------------------------------------------------------------------
template<class TDataType>
TDataType* TYQDataList<TDataType>::StepGetData(int* Item)
{
    TDataType* Data;
   /* *Item = FDataPtr;
    Data = DataItem(FDataPtr);
    if (FDatas->Count > 0)
    	FDataPtr = (FDataPtr + 1) % (FDatas->Count);
    else
    	FDataPtr = 0;*/
    return Data;
}
//---------------------------------------------------------------------------
template<class TDataType>
void TYQDataList<TDataType>::DeleteData(int Item)
{
    TDataType* Data;
    FSync->Enter();
    try
    {
        if (Item < FDatas->Count())
        {
            Data = DataItem(Item);
            FDatas->RemoveElement(Item);
            delete Data;
        }
    }
    catch(...)
    {
    }
    FSync->Leave();

}
//---------------------------------------------------------------------------
template<class TDataType>
void TYQDataList<TDataType>::Lock()
{
    FSync->Enter();
}
//---------------------------------------------------------------------------
template<class TDataType>
void TYQDataList<TDataType>::UnLock()
{
   FSync->Leave();
}
//---------------------------------------------------------------------------
template<class TDataType>
TDataType* TYQDataList<TDataType>::CreateData()
{
    TDataType* PData = new TDataType;
    memset(PData, 0,sizeof(TDataType));
    FSync->Enter();
    FDatas->Add(PData);
   FSync->Leave();
    return PData;
}
//---------------------------------------------------------------------------
template<class TDataType>
void TYQDataList<TDataType>::WriteData(const TDataType& Data)
{
    TDataType* PData = CreateData();
    *PData = Data;;
}
//---------------------------------------------------------------------------
#endif
