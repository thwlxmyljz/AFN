//---------------------------------------------------------------------------
#ifndef YQDataListuH
#define YQDataListuH

#include "Threadu.h"
#include "yqlist.h"
/*---------------------------------------------------------------------------
 *
 * TYQDataList 提供多线程时同步读、写数据链表的能力
 * TDataType 不能是指针
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
    TYQDataList(TCriticalSection* Sync);		//使用外部锁
    virtual ~TYQDataList();
    /*
     * ReadData 读取数据，并在容器列表中删除该记录，列表为空时返回空指针。
     * ReadData 的调用者使用完返回的指针后必需释放该空间，否则会造成内存泄漏。
     * StepGetData 每次取一条数据并将计数器加1。
     * DataItem() 读取数据但不删除列表，调用者不能释放返回的空间
     */
    TDataType* ReadData();   //读最早的数据，数据不存在时返回NULL
    TDataType* ReadData(const int Item);   //数据不存在时返回NULL
    TDataType* StepGetData(int* Item);   //列表无数据时返回NULL
    TDataType* DataItem(const int Item);  //数据不存在时返回NULL

    void DeleteData(int Item);

    void Lock();
    void UnLock();

    //创建数据
    TDataType* CreateData();
    //写入数据,Data的空间由调用者释放
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
