#pragma once

#include <vector>
#include "common/Lock.h"
#include "common/CommonDefine.h"

template<class Type>
class CVector
{
public:
	CVector() = default;
	CVector(int _buffersize)
	{
		m_vector.reserve(_buffersize);
	}
	virtual ~CVector() = default;

	void push(Type v);
	Type getdata(int _idx);
	int  getsize();
	auto getbegin();
	auto getend();
	void erase(Type v);
	void erase_all();
	auto getbufferptr() const { return m_vector; }

private:

	CLock m_Lock;
	std::vector<Type> m_vector;
	
};

template<class Type>
void CVector<Type>::push(Type v) {
	m_Lock.Lock();
	m_vector.emplace_back(std::move(v));
	m_Lock.UnLock();
}

template<class Type>
void CVector<Type>::erase(Type value)
{
	std::vector<Type>::iterator itr;
	m_Lock.Lock();
	for (itr = m_vector.begin(); itr != m_vector.end();)
	{
		if ((*itr) == value)
			itr = m_vector.erase(itr);
		else
		    itr++;
	}
	m_Lock.UnLock();
}

template<class Type>
Type CVector<Type>::getdata(int _idx) {
	Type v;
	m_Lock.Lock();
	v = m_vector[_idx];
	m_Lock.UnLock();
	return v;
}

template<class Type>
int  CVector<Type>::getsize() {
	int size = 0;
	m_Lock.Lock();
	size = m_vector.size();
	m_Lock.UnLock();
	return size;
}

template<class Type>
auto CVector<Type>::getbegin() {
	std::vector<Type>::iterator itr;
	m_Lock.Lock();
	itr = m_vector.begin();
	m_Lock.UnLock();
	return itr;
}

template<class Type>
auto CVector<Type>::getend(){
	std::vector<Type>::iterator itr;
	m_Lock.Lock();
	itr = m_vector.end();
	m_Lock.UnLock();
	return itr;
}

template<class Type>
void CVector<Type>::erase_all() {
	m_Lock.Lock();
	
	for (auto& i : m_vector)
	{
		delete(i);
	}
	m_vector.clear();
	m_Lock.UnLock();

}

//extern CVector<PST_DEFECTDATA> g_CRollMapBuffer;
//extern CVector<PST_DEFECTDATA> g_CVirtualDefectBuffer;
//extern CVector<PST_DEFECTDATA> g_CMovingMapBuffer;

