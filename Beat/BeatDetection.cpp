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
	// ��ȣ�� �ִ� ���� �˻��Ѵ�.
	float max = 0.0f;
	
	for (int i = 0; i < size; i++)
	{
		if (abs(signal[i]) > max)
		{
			max = abs(signal[i]);
		}
	}

	// ��ȣ ����
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
	// ���� �����͸� �����´�.
	int * RightData = nullptr;
	int * LeftData = nullptr;
	int * result = new int[m_Lenght];
	if (!SOUNDMANAGER->GetPCMData(m_SongName, RightData, LeftData))
		return false;

	// ���� �������� ����Ѵ�.
	for (UINT i = 0; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		m_Energie[i] = static_cast<float>(CalculateInstanEnergy(LeftData, DETECTIONSAMPLE * i, 4092));  // ���� �ణ �ε巴�� ������ش�.
	}

	// 1�� ������ ��� �������� ����Ѵ�.
	m_TotalEnergies[0] = 0;
	// ù ������ 43���� 1024�� ù�� °  totalenergies�� �����Ѵ�.
	float sum = 0.0f;

	for (int i = 0; i < 42; i++)
	{
		sum = sum + m_Energie[i];
	}

	m_TotalEnergies[0] = sum / 42;

	// �ٸ� ��� �Ϳ��� ������ ����� �Ѵ�.
	for (UINT i = 1; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		if (i + 42 < m_Lenght / DETECTIONSAMPLE)
		{
			sum = sum - m_Energie[i - 1] + m_Energie[i + 42];
			m_TotalEnergies[i] = sum / 42;
		}
	}

	// �������� ������ ���Ѵ�.

	for (UINT i = 21; i < m_Lenght / DETECTIONSAMPLE; i++)
	{
		// �������� ���߽�Ű�� ���� 21�� ���ش�.
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
// bpm�� ����Ѵ�.
void BeatDetection::BpmCalculation()
{
	// �� EnergiesPeak ������ �ð������ ����Ѵ�.
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

	// ��踦 ���� ���� �Ϲ����� ���� ã�´�.

	m_OccurrenceMax = 0;
	// �� �ð� ����� �߻� ȸ���� ���Ѵ�.
	int OccurrencePack[86]; // 2�� 43���� 2�� �� 86
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

	// �ִ� + �ִ���� + ���е�
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
	// ������ ����Ѵ�.
	m_Tempo = static_cast<int>(60.0f / (m_Occurrence * (DETECTIONSAMPLE / 44100.0f)));

}

bool BeatDetection::BeatCalculation()
{
	// �޽� Ʈ������ �����Ѵ�.
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

	// ������ ���� �������� Colvolution�� ����� ���Ѵ�.
	for (int i = 0; i < m_Lenght / DETECTIONSAMPLE - DIMPSIZE; i++)
	{
		for (int j = 0; j < DIMPSIZE; j++)
		{
			m_Colvolution[i] = m_Colvolution[i] + (m_Energie[i + j] * Train[j]);
		}
	}

	Normalize(m_Colvolution, m_Lenght / DETECTIONSAMPLE, 1.0f);

	//m_Colvolution�� �ִ븦 ã�´�(��Ʈ�� ���ϱ� ���ؼ�)
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

	// ���������� �̵�
	int i = ColMaxPos + m_OccurrenceMax;
	while ((i < m_Lenght / DETECTIONSAMPLE) && (m_Colvolution[i] > 0.0f))
	{
		// �ִ븦 ã�´�
		int MaxPosLocal = MaxSearch(m_Colvolution, i, 2);
		m_Beat[MaxPosLocal] = 1.0f;
		i = MaxPosLocal + m_OccurrenceMax;
	}

	// �������� �̵�
	i = ColMaxPos - m_OccurrenceMax;
	while (i > 0)
	{
		// �ִ븦 ã�´�
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
