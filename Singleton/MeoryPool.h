#pragma once
#include <iostream>
#include <windows.h>
template <class T, int ALLOC_BLOCK_SIZE = 50 >
class CMemeoryPool
{
public:
	//������ �ּҸ� ��ȯ���ִ� ���۷����� �Լ�
	static VOID* operator new(std::size_t allocLength)
	{
		//���� �ּҰ� ���ٸ�
		if (!mFreePointer)
			//�����س��� ũ�⸸ŭ Ǯ ����ֱ�
			AllocBlock();
		UCHAR* returnPointer = mFreePointer;
		//���� �ּҷ� �Ѿ��
		mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);
		return returnPointer;
	}
	//���� ���۷����� �Լ�
	static VOID operator delete(VOID* deletePointer)
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		mFreePointer = reinterpret_cast<UCHAR*>(deletePointer);
	}
private:
	//��ϸ�ŭ �ٽ� ũ�⸦ ��´�
	static VOID AllocBlock()
	{
		//������ �ּҴ� (���� ũ�⸦ �ٽ� ��´�)
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		UCHAR** current = reinterpret_cast<UCHAR**>(mFreePointer);
		UCHAR* next = mFreePointer;
		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<UCHAR**>(next);
		}
		*current = 0;
	}

private:
	static UCHAR* mFreePointer;
protected:
	~CMemeoryPool()

	{

	}

};

template <class T, int ALLOC_BLOCK_SIZE>
//������ �ּҸ� ����ִ� ������
UCHAR* CMemeoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;