
#include <stdlib.h>
#include <stdio.h>
#include "yqlist.h"
#include <string.h>
#include <assert.h>
#include "YQResource.h"


int YQList::yq_list_init (yq_list_t * li)
{
	if (li == NULL)
		return -1;
	memset (li, 0, sizeof (yq_list_t));
		return 0;                     /* ok */
}
YQList::YQList()
{

	li = new yq_list_t();
	assert(li != NULL);	
	yq_list_init(li);
	
}
YQList::~YQList()
{
	if(li != NULL)
		delete li;
	
}

void YQList::YQListSepcialFree (void *(*free_func) (void *))
{
  void *element;

  if (li == NULL)
    return;
  while (!IsEof (0))
    {
      element = (void *) Items ( 0);
      RemoveElement (0);
      if (free_func != NULL)
        free_func (element);
    }
}

void YQList::YQListOfCharFree()
{
  int pos = 0;
  char *chain;

  if (li == NULL)
    return;
  while (!IsEof ( pos))
    {
      chain = (char *) Items (pos);
      RemoveElement ( pos);
      free (chain);
    }
}

int YQList::Count() const
{
  if (li != NULL)
    return li->nb_elt;
  else
  {
  return -1;
  	}
}

int  YQList::IsEof (int i) const
{
  if (li == NULL)
    return -1;
  if (i < li->nb_elt)
    return 0;                   /* not end of list */
  return 1;                     /* end of list */
}


int YQList::Add(void * Item)
{
	int nPos =  Count();
	return Insert(nPos,Item);
}
/* index starts from 0; */
int YQList::Insert (int Index, void * el)
{
  __yqnode_t *ntmp;
  int i = 0;


  if (li == NULL)
  	{
    return -1;
  	}

  if (li->nb_elt == 0)
    {

      li->node = (__yqnode_t *) malloc (sizeof (__yqnode_t));
      if (li->node == NULL)
        return -1;
      li->node->element = el;
      li->node->next = NULL;
      li->nb_elt++;
      return li->nb_elt;
    }

  if (Index == -1 || Index >= li->nb_elt)
    {                           /* insert at the end  */
      Index = li->nb_elt;
    }

  ntmp = li->node;              /* exist because nb_elt>0  */

  if (Index == 0)                 /* pos = 0 insert before first elt  */
    {
      li->node = (__yqnode_t *) malloc (sizeof (__yqnode_t));
      if (li->node == NULL)
        {
          /* leave the list unchanged */
          li->node = ntmp;
          return -1;
        }
      li->node->element = el;
      li->node->next = ntmp;
      li->nb_elt++;
      return li->nb_elt;
    }


  while (Index > i + 1)
    {
      i++;
      /* when pos>i next node exist  */
      ntmp = (__yqnode_t *) ntmp->next;
    }

  /* if pos==nb_elt next node does not exist  */
  if (Index == li->nb_elt)
    {
      ntmp->next = (__yqnode_t *) malloc (sizeof (__yqnode_t));
      if (ntmp->next == NULL)
        return -1;              /* leave the list unchanged */
      ntmp = (__yqnode_t *) ntmp->next;
      ntmp->element = el;
      ntmp->next = NULL;
      li->nb_elt++;
      return li->nb_elt;
    }

  /* here pos==i so next node is where we want to insert new node */
  {
    __yqnode_t *nextnode = (__yqnode_t *) ntmp->next;

    ntmp->next = (__yqnode_t *) malloc (sizeof (__yqnode_t));
    if (ntmp->next == NULL)
      {
        /* leave the list unchanged */
        ntmp->next = nextnode;
		
        return -1;
      }
    ntmp = (__yqnode_t *) ntmp->next;
    ntmp->element = el;
    ntmp->next = nextnode;
    li->nb_elt++;
  }
  return li->nb_elt;
}

/* index starts from 0 */
void *YQList::Items (int pos) const
{
  __yqnode_t *ntmp;
  int i = 0;

  if (li == NULL)
    return NULL;

  if (pos < 0 || pos >= li->nb_elt)
    /* element does not exist */
    return NULL;


  ntmp = li->node;              /* exist because nb_elt>0 */

  while (pos > i)
    {
      i++;
      ntmp = (__yqnode_t *) ntmp->next;
    }
  return ntmp->element;
}



/* return -1 if failed */
int YQList::RemoveElement (int pos)
{

  __yqnode_t *ntmp;
  int i = 0;

  if (li == NULL)
    return -1;

  if (pos < 0 || pos >= li->nb_elt)
    /* element does not exist */
    return -1;

  ntmp = li->node;              /* exist because nb_elt>0 */

  if ((pos == 0))
    {                           /* special case  */
      li->node = (__yqnode_t *) ntmp->next;
      li->nb_elt--;
      free (ntmp);
      return li->nb_elt;
    }

  while (pos > i + 1)
    {
      i++;
      ntmp = (__yqnode_t *) ntmp->next;
    }

  /* insert new node */
  {
    __yqnode_t *remnode;

    remnode = (__yqnode_t *) ntmp->next;
    ntmp->next = ((__yqnode_t *) ntmp->next)->next;
    free (remnode);
    li->nb_elt--;
  }
  return li->nb_elt;
}
int YQList::YQListClone (YQList *dst,int *(*clone_func) (void *, void *)) const
{
	void *data;
	void *data2;
	int pos=0;
	if(dst == NULL)
		return -1;


	while (!IsEof (pos))
	{
		data = (void*)Items (pos);
		if (clone_func (data, &data2)!=0)
			return -1;
		dst->Add (data2);
		pos++;
	}
	return 0;

}
void YQList::YQListFreeAllYQRes()
{
  YQRes*element;

  if (li == NULL)
    return;
  while (!IsEof (0))
    {
      element = (YQRes *) Items ( 0);
      RemoveElement (0);
	  delete element;
	  element = NULL;
    }
}
int YQList::YQListResClone (YQList *dst) const
{

	if(dst == NULL)
		return -1;
	//自拷贝
	if(dst == this)
		return 0;
//	if(dst->GetLI() == this->li)
	//	return;
	//先将dst清空
	dst->YQListFreeAllYQRes();
	return YQListAddResClone(dst);
}

int YQList::YQListAddResClone(YQList *dst) const
{
	YQRes *data;
	YQRes *data2;
	int pos=0;

	if(dst == NULL)
		return -1;

	while (!IsEof (pos))
	{
		data = (YQRes*)Items (pos);
		if(data != NULL)
		{
			data2 = new YQRes;
			if(data2 != NULL)
			{
				*data2 = *data;
				dst->Add ((void*)data2);
				pos++;
			}
		}
	}
	return 0;
}

