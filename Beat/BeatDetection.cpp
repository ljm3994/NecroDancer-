#include "stdafx.h"
#include "BeatDetection.h"


int BeatDetection::Energies(int * data, int offset, int window)
{
	float Energie = 0.0f;
	float Window = 1.0f / static_cast<float>(window);
	float DataAsFloat = 0.0f;

	for (UINT i = offset; (i < offset + window) && (i < m_Lenght); i++)
	{
		DataAsFloat = static_cast<float>(data[i]);
		Energie += pow(DataAsFloat, 2) * Window;
	}
	return static_cast<int>(Energie);
}

void BeatDetection::Normalize(float * signal, int size, float maxval)
{
	// 신호의 최대 값을 검색한다.
	float max = 0.0f;
	
	for (int i = 0; i < size; i++)
	{
		if (abs(signal[i]) > max)
		{
			max = abs(signal[i]);
		}
	}

	// 신호 조정
	float ratio = maxval / max;

	for (int i = 0; i < size; i++)
	{
		signal[i] = signal[i] * ratio;
	}
}

int BeatDetection::MaxSearch(float * signal, int pos, int halfsize)
{
	float max = 0.0f;
	int MaxPos = pos;

	for (int i = pos - halfsize; i <= pos + halfsize; i++)
	{
		if (signal[i] > max)
		{
			max = signal[i];
			MaxPos = i;
		}
	}
	return MaxPos;
}

int BeatDetection::CalculateInstanEnergy(int * data, int dataOffset, int dataWindow)
{
	float result = 0;
	float Window = 1.0f / static_cast<float>(dataWindow);
	float DataAsFloat = 0.0f;

	for (int i = dataOffset; (i < dataOffset + dataWindow) && (i < static_cast<int>(m_Lenght)); i++)
	{
		DataAsFloat = static_cast<float>(data[i]);
		result += pow(DataAsFloat, 2) * Window;
	}

	return static_cast<int>(result);
}

float BeatDetection::CalculateAvergeEnergyForSubband(int Subband, vector<vector<float>>& EnergyHistoryBuffer)
{
	vector<float>& historyBufferForSubband = EnergyHistoryBuffer[Subband];
	historyBufferForSubband.emplace_back(m_FFTSubbandInstanceEnergyBuffer[Subband]);

	if (historyBufferForSubband.size() > HISTORYVALUE)
	{
		historyBufferForSubband.erase(historyBufferForSubband.begin());
	}

	float result = 0;
	UINT i = 0;
	for (; i < historyBufferForSubband.size(); i++)
	{
		result += historyBufferForSubband[i];
	}
	result /= (i + 1);

	return result;
}

void BeatDetection::UpdateEnergyHistoryBuffer(vector<float>& InstantEnergyBuffer, vector<vector<float>>& EnergyHistoryBuffer)
{
	m_FFTSubbandAverageEnergy.clear();

	float averageEnergy = 0.0f;

	for (int i = 0; i < static_cast<int>(InstantEnergyBuffer.size()); ++i)
	{
		averageEnergy = CalculateAvergeEnergyForSubband(i, EnergyHistoryBuffer);
		m_FFTSubbandAverageEnergy.emplace_back(averageEnergy);
	}
}

BeatDetection::BeatDetection()
{
}


BeatDetection::~BeatDetection()
{
	SAFE_DELETE_ARRAY(m_Energie);
	SAFE_DELETE_ARRAY(m_TotalEnergies);
	SAFE_DELETE_ARRAY(m_Colvolution);
	SAFE_DELETE_ARRAY(m_Beat);
	SAFE_DELETE_ARRAY(m_EnergiesPeak);
}

HRESULT BeatDetection::Init(string Name)
{
	m_SongName = Name;

	m_Lenght = SOUNDMANAGER->GetLength(Name);
	m_FFTSubbandAverageEnergy.clear();
	m_FFTSubbandInstanceEnergyBuffer.clear();
	m_EnergySubbandHistoryBuffer.clear();
	m_Energie = new float[m_Lenght/ DETECTIONSAMPLE];
	m_TotalEnergies = new float[m_Lenght / DETECTIONSAMPLE];
	m_Colvolution = new float[m_Lenght / DETECTIONSAMPLE];
	m_Beat = new float[m_Lenght / DETECTIONSAMPLE];
	m_EnergiesPeak = new float[m_Lenght / DETECTIONSAMPLE + 21];
	m_Occurrence = 0.0f;
	m_OccurrenceMax = 0;
	for (UINT i = 0; i < m_Lenght / DETECTIONSAMPLE + 21; i++)
	{
		m_EnergiesPeak[i] = 0;
	}

	m_FFTSubbandAverageEnergy.reserve(SUBBAND);
	m_FFTSubbandInstanceEnergyBuffer.reserve(SUBBAND);
	m_EnergySubbandHistoryBuffer.reserve(SUBBAND);

	return S_OK;
}

bool BeatDetection::EnergieCalculation()
{
	// 음악 데이터를 가져온다.
	int * RightData = nullptr;
	int * LeftData = nullptr;
	int * result = new int[m_Lenght];
	if (!SOUNDMANAGER->GetPCMData(m_SongName, RightData, LeftData))
		return false;

	// 순간 에너지를 계산한다.
	for (UINT i = 0; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		m_Energie[i] = static_cast<float>(CalculateInstanEnergy(LeftData, DETECTIONSAMPLE * i, 4092));  // 값을 약간 부드럽게 만들어준다.
	}

	// 1초 동안의 평균 에너지를 계산한다.
	m_TotalEnergies[0] = 0;
	// 첫 에너지 43개와 1024는 첫번 째  totalenergies에 제공한다.
	float sum = 0.0f;

	for (int i = 0; i < 42; i++)
	{
		sum = sum + m_Energie[i];
	}

	m_TotalEnergies[0] = sum / 42;

	// 다른 모든 것에도 동일한 계산을 한다.
	for (UINT i = 1; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		if (i + 42 < m_Lenght / DETECTIONSAMPLE)
		{
			sum = sum - m_Energie[i - 1] + m_Energie[i + 42];
			m_TotalEnergies[i] = sum / 42;
		}
	}

	// 에너지의 비율을 구한다.

	for (UINT i = 21; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		// 에너지를 집중시키기 위해 21을 빼준다.
		if (m_Energie[i] > ENERGYRATIO*m_TotalEnergies[i - 21])
		{
			m_EnergiesPeak[i] = 1;
		}
	}
	SAFE_DELETE_ARRAY(RightData);
	SAFE_DELETE_ARRAY(LeftData);
	SAFE_DELETE_ARRAY(result);
	return true;
}
// bpm을 계산한다.
void BeatDetection::BpmCalculation()
{
	// 각 EnergiesPeak 사이의 시간경과를 계산한다.
	vector<int> Time;
	int prec = 0;
	for (UINT i = 1; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		if ((m_EnergiesPeak[i] == 1) && (m_EnergiesPeak[i - 1] == 0))
		{
			int Differ = i - prec;
			if (Differ > 5)
			{
				Time.push_back(Differ);
				prec = i;
			}
		}
	}

	// 통계를 통해 가장 일반적인 값을 찾는다.

	m_OccurrenceMax = 0;
	// 각 시간 경과의 발생 회수를 구한다.
	int OccurrencePack[86]; // 2초 43개의 2배 는 86
	for (int i = 0; i < 86; i++)
	{
		OccurrencePack[i] = 0;
	}

	for (UINT i = 1; i < Time.size(); i++)
	{
		if (Time[i] <= 86)
		{
			OccurrencePack[Time[i]]++;
		}
	}
	int Occmax = 0;
	m_Occurrence = 0.0f;
	for (int i = 0; i < 86; i++)
	{
		if (OccurrencePack[i] > Occmax)
		{
			m_OccurrenceMax = i;
			Occmax = OccurrencePack[i];
		}
	}

	// 최대 + 최대평균 + 정밀도
	int Maxaverage = m_OccurrenceMax - 1;
	if (OccurrencePack[m_OccurrenceMax + 1] > OccurrencePack[Maxaverage])
	{
		Maxaverage = m_OccurrenceMax + 1;
	}
	float Div = static_cast<float>(OccurrencePack[m_OccurrenceMax] + OccurrencePack[Maxaverage]);

	if (Div == 0)
	{
		m_Occurrence = 0;
	}
	else
	{
		m_Occurrence = static_cast<float>(m_OccurrenceMax*OccurrencePack[m_OccurrenceMax] + Maxaverage * OccurrencePack[Maxaverage]) / Div;
	}
	// 템포를 계산한다.
	m_Tempo = static_cast<int>(60.0f / (m_Occurrence * (DETECTIONSAMPLE / 44100.0f)));

}

bool BeatDetection::BeatCalculation()
{
	// 펄스 트레인을 생성한다.
	float Train[DIMPSIZE];
	float Space = 0.0f;
	Train[0] = 1.0f;

	for (int i = 0; i < DIMPSIZE; i++)
	{
		if (Space >= m_Occurrence)
		{
			Train[i] = 1;
			Space = Space - m_Occurrence;
		}
		else
		{
			Train[i] = 0;
		}
		Space += 1.0f;
	}

	// 음악의 순간 에너지와 Colvolution의 평균을 구한다.
	for (int i = 0; i < m_Lenght / DETECTIONSAMPLE - DIMPSIZE; i++)
	{
		for (int j = 0; j < DIMPSIZE; j++)
		{
			m_Colvolution[i] = m_Colvolution[i] + (m_Energie[i + j] * Train[j]);
		}
	}

	Normalize(m_Colvolution, m_Lenght / DETECTIONSAMPLE, 1.0f);

	//m_Colvolution의 최대를 찾는다(비트를 구하기 위해서)
	for (UINT i = 1; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		m_Beat[i] = 0;
	}

	float ColMax = 0.0f;
	int ColMaxPos = 0;

	for (UINT i = 1; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		if (m_Colvolution[i] > ColMax)
		{
			ColMax = m_Colvolution[i];
			ColMaxPos = i;
		}
	}
	m_Beat[ColMaxPos] = 1.0f;

	// 오른쪽으로 이동
	int i = ColMaxPos + m_OccurrenceMax;
	while ((i < m_Lenght / DETECTIONSAMPLE) && (m_Colvolution[i] > 0.0f))
	{
		// 최대를 찾는다
		int MaxPosLocal = MaxSearch(m_Colvolution, i, 2);
		m_Beat[MaxPosLocal] = 1.0f;
		i = MaxPosLocal + m_OccurrenceMax;
	}

	// 왼쪽으로 이동
	i = ColMaxPos - m_OccurrenceMax;
	while (i > 0)
	{
		// 최대를 찾는다
		int MaxPosLocal = MaxSearch(m_Colvolution, i, 2);
		m_Beat[MaxPosLocal] = 1.0f;
		i = MaxPosLocal - m_OccurrenceMax;
	}
	return false;
}

float * BeatDetection::GetEnergie()
{
	return m_Energie;
}

float * BeatDetection::GetTotalEnergie()
{
	return m_TotalEnergies;
}

float * BeatDetection::GetEnergyPeak()
{
	return m_EnergiesPeak;
}

float * BeatDetection::GetColvolution()
{
	return m_Colvolution;
}

float * BeatDetection::GetBeat()
{
	return m_Beat;
}

int BeatDetection::GetTempo()
{
	return m_Tempo;
}

void BeatDetection::BeatClear()
{
	SAFE_DELETE_ARRAY(m_Energie);
	SAFE_DELETE_ARRAY(m_TotalEnergies);
	SAFE_DELETE_ARRAY(m_Colvolution);
	SAFE_DELETE_ARRAY(m_Beat);
	SAFE_DELETE_ARRAY(m_EnergiesPeak);
	m_EnergySubbandHistoryBuffer.clear();
	m_EnergySubbandHistoryBuffer.shrink_to_fit();
	m_FFTSubbandInstanceEnergyBuffer.clear();
	m_FFTSubbandInstanceEnergyBuffer.shrink_to_fit();
	m_FFTSubbandAverageEnergy.clear();
	m_FFTSubbandAverageEnergy.shrink_to_fit();
}

string BeatDetection::GetKey()
{
	return m_SongName;
}
