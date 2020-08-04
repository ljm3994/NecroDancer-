#include "stdafx.h"
#include "WinUI.h"
#include "Beat/FFTDetector.h"

WinUI::WinUI()
{
}


WinUI::~WinUI()
{
}

HRESULT WinUI::init()
{
	Beat = BEATMANAGER->GetEnergyPeak();
	Heart = IMGMANAGER->GetImage("Heart");
	m_Lenght = BEATMANAGER->Getm_Lenght();
	m_Tempo = BEATMANAGER->GetTempo();
	m_isBeat = false;
	detect = new FFTDetector();
	m_isFirst = false;
	m_PlayerKeyCheck = false;
	PrevGptCamX = 0;
	CurrentFramX = 0;
	BetteweenTime = 0;
	m_NoteLostDelay = 0.0f;
	m_PerfectBeat = false;
	return S_OK;
}

void WinUI::release()
{
	SAFE_DELETE(detect);
}

void WinUI::update()
{
	UINT pos;
	UINT mspos;
	string ImgKey = "";
	strKey = BEATMANAGER->GetKey();
	if (!m_isFirst)
	{
		detect->BeatDetectionInit(strKey);
		m_isFirst = true;
	}
	m_PlayerKeyCheck = CHARTERMANAGER->GetPlayer()->Getm_PlayerKeyOn();
	m_Pan = RectMakeCenter((WINSIZEX / 2) + g_ptCam.x, 100 + g_ptCam.y, 160, 100);
	if (SOUNDMANAGER->GetCurrentTimePos(strKey, pos))
	{
		SOUNDMANAGER->GetCurrentMSTimePos(strKey, mspos);
		int CurrentTile = static_cast<int>(pos / DETECTIONSAMPLE);

		int beatTime = static_cast<int>(pos / 44100.0f);
		if (Beat[CurrentTile] > 0 && (clock() - BetteweenTime) >= 500)
		{
			MAPMANAGER->SetBeat(true);
			BetteweenTime = static_cast<float>(clock());
			Note * Temp = OBJECTPOOL(Note)->GetObObject();
			Temp->m_pos = D3DXVECTOR2(0, 100 + g_ptCam.y);
			Temp->State = NOTEALIVE;
			Temp->a = 1.0f;
			Temp->NoteImg = IMGMANAGER->GetImage("Note1");
			LeftNote.emplace_back(Temp);
			Note * Temp2 = OBJECTPOOL(Note)->GetObObject();
			Temp2->m_pos = D3DXVECTOR2(WINSIZEX, 100 + g_ptCam.y);
			Temp2->State = NOTEALIVE;
			Temp2->a = 1.0f;
			Temp2->NoteImg = IMGMANAGER->GetImage("Note1");

			RightNote.emplace_back(Temp2);

			if (CurrentFramX == 0)
			{
				CurrentFramX = 1;
			}
			else if (CurrentFramX == 1)
			{
				CurrentFramX = 0;
			}
		}
	}

	if (m_PlayerKeyCheck)
	{
		if (m_PerfectBeat)
		{
			ImgKey = "PerfectImg";
		}
		else if (!m_PerfectBeat && !m_isBeat)
		{
			ImgKey = "MissImg";
		}
	}

	if (ImgKey != "")
	{
		MissImg *img = OBJECTPOOL(MissImg)->GetObObject();
		img->Alpha = 1.0f;
		img->m_Pos = D3DXVECTOR2(WINSIZEX / 2, 150);
		img->ImgKey = ImgKey;
		m_MissImg.emplace_back(img);
	}

	PrevGptCamX = g_ptCam.x;
}

void WinUI::render()
{
	RECT WriteRect;
	WriteRect.left = (WINSIZEX / 2) - 100;
	WriteRect.right = (WINSIZEX / 2) + 100;
	WriteRect.bottom = 100;
	WriteRect.top = 30;
	wchar_t str[256];
	wsprintfW(str, L"BPM : %d", m_Tempo);
	WRITEMANAGER->RenderText(str, WriteRect, 30.0f, L"±Ã¼­Ã¼", D3DXCOLOR(0.4f, 0.4f, 0.7f, 1.0f), DWRITE_FONT_WEIGHT_EXTRA_BOLD);
	Player * player = CHARTERMANAGER->GetPlayer();
	m_PlayerKeyCheck = player->Getm_PlayerKeyOn();
	LeftNoteit = LeftNote.begin();
	m_NoteLostDelay += g_ETime;
	for (; LeftNoteit != LeftNote.end();)
	{
		(*LeftNoteit)->m_pos.x += 130.0f * g_ETime;
		(*LeftNoteit)->m_pos.y = 100 + g_ptCam.y;
		(*LeftNoteit)->NoteImg->Position((*LeftNoteit)->m_pos.x + g_ptCam.x, (*LeftNoteit)->m_pos.y);

		if ((*LeftNoteit)->State == NOTEALIVE)
		{
			if (IntersectRect(m_Pan,
				RectMakeCenter((*LeftNoteit)->m_pos.x + g_ptCam.x, (*LeftNoteit)->m_pos.y, 10, 50)))
			{
				if (!m_PerfectBeat)
				{
					player->Setm_PerfectBeat(true);
					CHARTERMANAGER->Setm_IsBeat(true);
					m_PerfectBeat = true;
				}
				else if (m_PlayerKeyCheck)
				{
					CHARTERMANAGER->Setm_IsBeat(false);
					player->Setm_PerfectBeat(false);
					player->Setm_BeatTime(false);
					(*LeftNoteit)->NoteImg = IMGMANAGER->GetImage("NoteTarget");
					(*LeftNoteit)->State = NOTEDIE;
				}
				if (IntersectRect(RectMakeCenter((WINSIZEX / 2) + g_ptCam.x, 100 + g_ptCam.y, 100, 100),
					RectMakeCenter((*LeftNoteit)->m_pos.x + g_ptCam.x, (*LeftNoteit)->m_pos.y, 10, 50)))
				{
					m_PerfectBeat = false;
					m_isBeat = true;
					player->Setm_PerfectBeat(false);
					player->Setm_BeatTime(true);
					(*LeftNoteit)->NoteImg = IMGMANAGER->GetImage("NoteTarget");
					(*LeftNoteit)->State = NOTEDIE;
				}
			}
		}
		else
		{
			if (m_NoteLostDelay > 0.02f)
			{
				(*LeftNoteit)->a -= 0.2f;
			}
			if ((*LeftNoteit)->a <= 0.0f)
			{
				m_isBeat = false;
				player->Setm_BeatTime(false);
				OBJECTPOOL(Note)->PushObject((*LeftNoteit));
				LeftNoteit = LeftNote.erase(LeftNoteit);
				continue;
			}
		}
		(*LeftNoteit)->NoteImg->Color(0, 0, 0, (*LeftNoteit)->a);
		(*LeftNoteit)->NoteImg->Scale(10, 50);
		(*LeftNoteit)->NoteImg->Render();

		LeftNoteit++;
	}

	RightNoteit = RightNote.begin();

	for (; RightNoteit != RightNote.end();)
	{
		(*RightNoteit)->m_pos.x -= 130.0f * g_ETime;
		(*RightNoteit)->m_pos.y = 100 + g_ptCam.y;
		(*RightNoteit)->NoteImg->Position(g_ptCam.x + (*RightNoteit)->m_pos.x, (*RightNoteit)->m_pos.y);

		if ((*RightNoteit)->State == NOTEALIVE)
		{
			if (IntersectRect(m_Pan,
				RectMakeCenter((*RightNoteit)->m_pos.x + g_ptCam.x, (*RightNoteit)->m_pos.y, 10, 50)))
			{
				if (!m_PerfectBeat)
				{
					m_PerfectBeat = true;
				}
				else if (m_PlayerKeyCheck && m_isBeat)
				{
					(*RightNoteit)->NoteImg = IMGMANAGER->GetImage("NoteTarget");
					(*RightNoteit)->State = NOTEDIE;
				}

				if (IntersectRect(RectMakeCenter((WINSIZEX / 2) + g_ptCam.x, 100 + g_ptCam.y, 100, 100),
					RectMakeCenter((*RightNoteit)->m_pos.x + g_ptCam.x, (*RightNoteit)->m_pos.y, 10, 50)))
				{
					m_PerfectBeat = false;
					m_isBeat = true;
					(*RightNoteit)->NoteImg = IMGMANAGER->GetImage("NoteTarget");
					(*RightNoteit)->State = NOTEDIE;
				}
			}
		}
		else
		{
			if (m_NoteLostDelay > 0.02f)
			{
				(*RightNoteit)->a -= 0.2f;
			}
			if ((*RightNoteit)->a <= 0.0f)
			{
				m_isBeat = false;
				OBJECTPOOL(Note)->PushObject((*RightNoteit));
				RightNoteit = RightNote.erase(RightNoteit);
				continue;
			}
		}

		(*RightNoteit)->NoteImg->Color(0.0f, 0.0f, 0.0f, (*RightNoteit)->a);
		(*RightNoteit)->NoteImg->Scale(10, 50);
		(*RightNoteit)->NoteImg->Render();

		RightNoteit++;
	}

	for (ImgIt = m_MissImg.begin(); ImgIt != m_MissImg.end();)
	{
		Image * img = IMGMANAGER->GetImage((*ImgIt)->ImgKey);
		img->Position((*ImgIt)->m_Pos.x + g_ptCam.x, (*ImgIt)->m_Pos.y + g_ptCam.y);
		img->Scale(40, 25);
		img->Color(0, 0, 0, (*ImgIt)->Alpha);
		img->Render();
		(*ImgIt)->m_Pos.y += 70 * g_ETime;
		(*ImgIt)->Alpha -= 0.005f;
		if ((*ImgIt)->Alpha <= 0)
		{
			OBJECTPOOL(MissImg)->PushObject((*ImgIt));
			ImgIt = m_MissImg.erase(ImgIt);
		}
		else
		{
			ImgIt++;
		}
	}

	Heart->Scale(100, 100);
	Heart->Position((WINSIZEX / 2) + g_ptCam.x, 100 + g_ptCam.y);
	Heart->GetRefCurrentFrameX() = CurrentFramX;
	Heart->Render();
	FFTWindowRender();
	if (m_NoteLostDelay > 0.5f)
	{
		m_NoteLostDelay = 0.0f;
	}
}

void WinUI::SetKey(string Key)
{
	strKey = Key;
}

void WinUI::FFTWindowRender()
{
	int MaxBar = min(64 * 2, 8192);
	detect->RenderFFTVisual({ WINSIZEX * 0.35f, WINSIZEY - 150 }, MaxBar, { WINSIZEX * 0.3, 70 }, 30);
}
