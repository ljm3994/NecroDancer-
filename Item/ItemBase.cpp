#include "stdafx.h"
#include "ItemBase.h"


ItemBase::ItemBase()
{
}


ItemBase::~ItemBase()
{
}

HRESULT ItemBase::Init()
{
	m_IsEffect = false;
	return S_OK;
}

void ItemBase::SetInventory(bool Set)
{
	m_SetInventory = Set;
}

