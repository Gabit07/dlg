#ifndef Queue_H_
#define Queue_H_

#include "common/Lock.h"
#include "common/CommonDefine.h"
#include <afxwin.h>

template <class Type>
class CQueue
{

private :
	template <class Type>
	class Node
	{
	public:
		Type Data;
		Node<Type> *Next;

		void operator=(const Node &Source)
		{
			Data = Source.Data;
		}
	};

private : 
	UINT uCount;
	Node<Type> *First;
	Node<Type> *Rear;

	CLock  m_Lock;

public :
	CQueue();
	virtual ~CQueue();
	bool Push(const Type &Source);
	Type Pop();
	Type Recent();
	Type Last();
	UINT GetCount();
	bool Empty();
	void RemoveAll();
	void Remove();
};

template <class Type>
CQueue<Type>::CQueue()
{
	uCount = 0;
	First  = NULL;
	Rear   = NULL;
}

template <class Type>
CQueue<Type>::~CQueue()
{
	RemoveAll();
}

template<class Type>
bool CQueue<Type>::Push(const Type &Source)
{
	Node<Type> *pStorage;
	m_Lock.Lock();
	pStorage = new Node<Type>; // Storage Create
	if(pStorage == NULL)
		return false;
	pStorage->Data = Source;   // data copy
	pStorage->Next = NULL;

	/* Node Link  */
	if(Empty())   
	{
		First = pStorage;    
		Rear  = pStorage;
	}
	else 
	{
		Rear->Next = pStorage;
		Rear       = pStorage;
	}
	uCount++;
	m_Lock.UnLock();
	return true;
}

template<class Type>
Type CQueue<Type>::Pop()
{
	Type ReturnData;           // Data for Return
	Node<Type> *FreeNode;    

	m_Lock.Lock();
	ReturnData = First->Data;  // Data Copy
	FreeNode   = First;        // fist Pointer Backup for Delete
	First = First->Next;       // first Pointer Moving
	uCount--;

	SAFE_DELETE(FreeNode);  // free old Node
	m_Lock.UnLock();
	return ReturnData;   // return
}


template<class Type>
Type CQueue<Type>::Recent()
{ 
	return Rear->Data;
}

template<class Type>
Type CQueue<Type>::Last()
{
	return First->Data;
}

template<class Type>
UINT CQueue<Type>::GetCount()
{
	return uCount;
}

template<class Type>
bool CQueue<Type>::Empty()
{
	return (uCount == 0);
}

template<class Type>
void CQueue<Type>::RemoveAll()
{
	m_Lock.Lock();
	if(First != NULL)    
	{
		while(!Empty())
		{
			Node<Type> *FreeNode;
			FreeNode = First;   // fist Pointer Backup for Delete
			First = First->Next;   // first Pointer Moving
			uCount--;

			SAFE_DELETE(FreeNode);  // free old Node
		}
	}
	m_Lock.UnLock();
}

template<class Type>
void CQueue<Type>::Remove()
{
	Node<Type> *FreeNode;    

	m_Lock.Lock();
	FreeNode   = First;        // fist Pointer Backup for Delete
	First = First->Next;       // first Pointer Moving
	uCount--;

	SAFE_DELETE(FreeNode);  // free old Node
	m_Lock.UnLock();
}

#endif // Queue_H_