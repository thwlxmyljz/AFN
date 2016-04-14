#ifndef _YQLIST_H_
#define _YQLIST_H_


//typedef struct __yqnode ;


typedef struct __yqnode
{
void *next;                 /**< next __yqnode_t containing element */
void *element;              /**< element in Current node */
}__yqnode_t;


//typedef struct yq_list ;



typedef struct yq_list
{
int nb_elt;         /**< Number of element in the list */
__yqnode_t *node;     /**< Next node containing element  */

}yq_list_t;





class YQList
{
public:

	//Inserts a new item at the end of the list.
	int Add(void * Item);

	//Adds an object to the Items array at the position specified by Index.
	int Insert(int Index, void * el);	
	

    int Count() const;
    int RemoveElement (int pos);
    void *Items (int pos) const; 

    int YQListClone (YQList *dst,int *(*clone_func) (void *, void *)) const;


    //���������Ϊ��Դ
    void YQListSepcialFree (void *(*free_func) (void *));
    void YQListOfCharFree();//������������� 

  

    //Ϊ����ṹ����ĺ���
    //YQRES�ṹ

    void YQListFreeAllYQRes();
    int YQListResClone (YQList *dst) const;
    int YQListAddResClone(YQList *dst) const;




public:
	YQList();
	virtual ~YQList();
    yq_list_t* GetLI(){return li;};
private:
	int yq_list_init (yq_list_t * li);
	int  IsEof (int i) const;
private:
	yq_list_t *li;
};




#endif
