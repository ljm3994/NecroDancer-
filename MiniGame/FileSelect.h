#pragma once
#include "GameNode.h"
#include <atlbase.h>
#include <ShObjIdl.h>

enum FileState
{
	SELECT,
	NOSELECT
};

class FileSelect : public GameNode
{
	string Path;
	int SelectNum;
	CComPtr<IFileDialog> pfd;
	bool m_FileOpen;
public:
	FileSelect();
	~FileSelect();
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

