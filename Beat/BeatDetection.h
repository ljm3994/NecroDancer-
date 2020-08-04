#pragma once
#include "Singleton/SingletonBase.h"

#define ENERGYRATIO 1.2 // 피크 에너지 감지를 위한 Energie와 TotalEnergies의 비율
#define DIMPSIZE 108 // Colvolution을 위한 펄스 열의 크기(1024는 1/43초)
#define SUBBAND 32 // 밴드 대역 32
#define DETECTIONSAMPLE 1024 // 1024개의 샘플
#define HISTORYVALUE 43

class BeatDetection : public SingletonBase<BeatDetection>
{
	string m_SongName;

	UINT m_Lenght;              // 길이값
	vector<vector<float>> m_EnergySubbandHistoryBuffer;       
	vector<float> m_FFTSubbandInstanceEnergyBuffer;       // 순간 에너지를 저장할 vector 변수
	vector<float> m_FFTSubbandAverageEnergy;              // 평균 에너지를 저장할 변수
	float * m_Energie;         // 1024 pcm마다 1024 샘플의 에너지를 계산한다.
	float * m_TotalEnergies;   // 1024 pcm마다 44100 샘플의 에너지를 계산한다.
	float * m_EnergiesPeak;   // 비트가 발생할 최대 값
	float * m_Colvolution;   // 펄스 값에 대해 반전 이동한 값을 곱한 다음, 구간에 대해 적분하여 구한 값
	float * m_Beat;   // 비트 값
	float m_Occurrence;
	int m_OccurrenceMax;

	int m_Tempo;; // 템포 값
	//주어진 위치와 폭에서 신호 에너지를 계산하는 함수
	int Energies(int * data, int offset, int window);
	// 신호 값을 원하는 최대 값으로 재조정 해주는 함수
	void Normalize(float * signal, int size, float max);
	// 지정한 위치 근처에서 최대 값을 검색해주는 함수
	int MaxSearch(float * signal, int pos, int halfsize);
	// 순간 에너지 계산 함수
	int CalculateInstanEnergy(int* data, int dataOffset, int dataWindow);
	// 에너지 평균 계산 함수
	float CalculateAvergeEnergyForSubband(int Subband, vector<vector<float>>& EnergyHistoryBuffer);
	// 히스토리 버퍼를 업데이트 하는 함수
	void UpdateEnergyHistoryBuffer(vector<float>& InstantEnergyBuffer, vector<vector<float>>& EnergyHistoryBuffer);
public:
	BeatDetection();
	~BeatDetection();
	HRESULT Init(string Name);
	bool EnergieCalculation();
	void BpmCalculation();
	bool BeatCalculation();
	float * GetEnergie();
	float * GetTotalEnergie();
	float * GetEnergyPeak();
	float * GetColvolution();
	float * GetBeat();
	int GetTempo();
	void BeatClear();
	string GetKey();
	MAKEGET(UINT, m_Lenght);
};

