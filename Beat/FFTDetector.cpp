#include "stdafx.h"
#include "FFTDetector.h"


FFTDetector::FFTDetector()
{
	m_InstanceEnengyBuffer = nullptr;
	m_EnergyPeaks = nullptr;
	m_EnergyHistoryBuffer = nullptr;
	m_HaveBeatsBeenInitialized = false;
	m_SoundPCMLenght = 0;
	m_MillisecondsPerBeats = 0.0f;
	m_strKey = "";
}


FFTDetector::~FFTDetector()
{
	SAFE_DELETE(m_InstanceEnengyBuffer);
	SAFE_DELETE(m_EnergyHistoryBuffer);
	SAFE_DELETE(m_EnergyPeaks);
}

void FFTDetector::BeatDetectionInit(string strKey)
{
	m_InstanceEnengyBuffer = nullptr;
	m_EnergyPeaks = nullptr;
	m_EnergyHistoryBuffer = nullptr;
	m_SoundPCMLenght = 0;
	m_MillisecondsPerBeats = 0.0f;
	m_FFTSubbandAverageEnergy.clear();
	m_FFTSubbandInstanceEnergyBuffer.clear();
	m_FFTSubbandEnergyHistoryBuffer.clear();
	m_BpmEstimates.clear();
	m_BeatHistory.clear();
	m_BeatTimesForSubband.clear();

	m_SoundPCMLenght = SOUNDMANAGER->GetLength(strKey);
	m_strKey = strKey;
	m_EnergyRation = 1.5f;
	int Temp = m_SoundPCMLenght >> SAMLERATEBITS;
	m_InstanceEnengyBuffer = new float[Temp];   //전체 파일에 대해 1024개의 샘플을 반복하므로 1024로 나눔
	m_EnergyHistoryBuffer = new float[Temp];
	m_EnergyPeaks = new float[(Temp)+21];  // 21을 더하는 것은 에너지 히스토리에서 약 05초이고 첫 정점 중간에서 시작하기 위해서

	for (int i = 0; i < static_cast<int>((Temp)+21); i++)
	{
		m_EnergyPeaks[i] = 0.0f;
	}

	m_FFTSubbandAverageEnergy.reserve(NUMSUBBANDS);
	m_FFTSubbandInstanceEnergyBuffer.reserve(NUMSUBBANDS);
	m_FFTSubbandEnergyHistoryBuffer.reserve(NUMSUBBANDS);

	for (int i = 0; i < NUMSUBBANDS; i++)
	{
		m_FFTSubbandEnergyHistoryBuffer.emplace_back(vector<float>());
		m_BeatTimesForSubband.emplace_back(queue<UINT>());
		m_BpmEstimates.emplace_back(0.0f);
		m_BeatHistory.emplace_back(map<float, UINT>());
	}
}

bool FFTDetector::GetIsBeat(int Subband)
{
	float instantEnerrgy = m_FFTSubbandInstanceEnergyBuffer[Subband];
	float averageEnergy = m_FFTSubbandAverageEnergy[Subband];

	return (instantEnerrgy > (m_EnergyRation * averageEnergy));
}

float FFTDetector::GetBpmForSubband(int Subband)
{
	float BeatGeussBpm = 0.0f;
	UINT Count = 0;
	map<float, UINT>::iterator it = m_BeatHistory[Subband].begin();

	for (it; it != m_BeatHistory[Subband].end(); it++)
	{
		if (it->second > Count)
		{
			BeatGeussBpm = it->first;
			Count = it->second;
		}
	}

	return BeatGeussBpm;
}

float FFTDetector::GetEnergyRation()
{
	return m_EnergyRation;
}

void FFTDetector::incrementEnergyRatioForBeatThreshold(float Value)
{
	m_EnergyRation += Value;
	if (m_EnergyRation <= 0.0f) m_EnergyRation = 0.0f;
}

void FFTDetector::Update()
{
	float* leftData = nullptr;
	float* rightData = nullptr;
	// FMOD에서 스펙트럼 데이터를 가져온다.
	SOUNDMANAGER->GetSpectrum(m_strKey, leftData, rightData, DETECTIONSAMLERATE);
	// 스펙트럼 데이터를 계산해서 반환한다.(왼쪽 사운드 데이터, 오른쪽 사운드 데이터, 샘플 수)
	float* squaredModsOfSpectrumData = GetSquaredModsSpectrumData(leftData, rightData, DETECTIONSAMLERATE);

	// 순간 에너지를 계산한다.
	CalculateInstatEneries(squaredModsOfSpectrumData, m_FFTSubbandInstanceEnergyBuffer, NUMSUBBANDS, DETECTIONSAMLERATE);
	// 히스토리 버퍼를 업데이트 한다.
	UpdateEnergyHistoryBuffer(m_FFTSubbandInstanceEnergyBuffer, m_FFTSubbandEnergyHistoryBuffer);
	// Bpm을 계산한다/
	CaculateBpm();

	SAFE_DELETE(leftData);
	SAFE_DELETE(rightData);
	SAFE_DELETE(squaredModsOfSpectrumData);
}

void FFTDetector::RenderFFTVisual(D3DXVECTOR2 StartPos, float MaxBarNum, D3DXVECTOR2 BarScale, float Offset)
{
	vector<float> m_Spec;
	float blockGap = 4 / (MaxBarNum / 64);
	float blockWidth = static_cast<float>(BarScale.x) / static_cast<float>(MaxBarNum) - blockGap;
	float blockMaxHeight = BarScale.y;

	SOUNDMANAGER->GetSpectrum(m_strKey, m_Spec, static_cast<int>(MaxBarNum));

	if (m_Spec.size() > 0)
	{
		for (int i = 0; i < MaxBarNum - 1; i++)
		{
			IMGMANAGER->Getrect()->Color({ 1, 0, 0, 1 });
			IMGMANAGER->Getrect()->Position(((StartPos.x + (blockWidth + blockGap) * i)) + g_ptCam.x, StartPos.y + g_ptCam.y);
			IMGMANAGER->Getrect()->Scale(blockWidth, Offset + (blockMaxHeight * m_Spec[i]));
			IMGMANAGER->Getrect()->Render();
		}
	}
}

void FFTDetector::Render()
{
	RenderInstantEnergies();
}

int FFTDetector::CalculateInstanEnergy(int * data, int dataOffset, int dataWindow)
{
	float result = 0;
	float Window = 1.0f / static_cast<float>(dataWindow);
	float DataAsFloat = 0.0f;

	for (int i = dataOffset; (i < dataOffset + dataWindow) && (i < static_cast<int>(m_SoundPCMLenght)); i++)
	{
		DataAsFloat = static_cast<float>(data[i]);
		result += pow(DataAsFloat, 2) * Window;
	}

	return static_cast<int>(result);
}

int FFTDetector::CalculateInstanEnergy(int * leftdata, int * rightData, int dataOffset, int DataWindow)
{
	float result = 0.0f;
	float leftAsFloat = 0.0f;
	float rightAsFloat = 0.0f;

	for (int i = dataOffset; (i < dataOffset + DataWindow) && (i < static_cast<int>(m_SoundPCMLenght)); i++)
	{
		leftAsFloat = static_cast<float>(leftdata[i]);
		rightAsFloat = static_cast<float>(rightData[i]);

		result += pow(leftAsFloat, 2) + pow(rightAsFloat, 2);
	}

	return static_cast<int>(result);
}

float * FFTDetector::GetSquaredModsSpectrumData(float * leftdata, float * rightData, int DataWindow)
{
	float * result = new float[DataWindow];

	for (int i = 0; i < DataWindow; i++)
	{
		result[i] = leftdata[i] * leftdata[i] + rightData[i] * rightData[i];
	}

	return result;
}

void FFTDetector::CalculateInstatEneries(float * SquareModeSpectrumData, vector<float>& instanctEnergyBuffer, int numSubband, int DataWindow)
{
	instanctEnergyBuffer.clear();
	// 샘플 수와 서브 밴드 수를 나눈다 (32 / 1024)
	float constant = (static_cast<float>(numSubband)) / static_cast<float>(DataWindow);
	// 서브 밴드 수만큼 계산
	for (int i = 0; i < numSubband; i++)
	{
		float result = 0;
		for (int j = i * numSubband; j < (i + 1) * numSubband; j++)
		{
			if (j > DataWindow) break;
			result += constant * SquareModeSpectrumData[j];
		}
		instanctEnergyBuffer.emplace_back(result);
	}
}

float FFTDetector::CalculateAvergeEnergyForSubband(int Subband, vector<vector<float>>& EnergyHistoryBuffer)
{
	vector<float>& historyBufferForSubband = EnergyHistoryBuffer[Subband];
	historyBufferForSubband.emplace_back(m_FFTSubbandInstanceEnergyBuffer[Subband]);

	if (historyBufferForSubband.size() > NUMHISTORYVALUES)
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

void FFTDetector::UpdateEnergyHistoryBuffer(vector<float>& InstantEnergyBuffer, vector<vector<float>>& EnergyHistoryBuffer)
{
	m_FFTSubbandAverageEnergy.clear();

	float averageEnergy = 0.0f;
	// 순간 에너지 버퍼 사이즈 만큼 반복
	for (int i = 0; i < static_cast<int>(InstantEnergyBuffer.size()); ++i)
	{
		// 서브 밴드의 평균 에너지를 구한다.
		averageEnergy = CalculateAvergeEnergyForSubband(i, EnergyHistoryBuffer);
		m_FFTSubbandAverageEnergy.emplace_back(averageEnergy);
	}
}

void FFTDetector::RenderInstantEnergies()
{
	float blockGap = 4 / (m_FFTSubbandInstanceEnergyBuffer.size());
	float blockWidth = static_cast<float>(WINSIZEX * 0.8) / static_cast<float>(m_FFTSubbandInstanceEnergyBuffer.size()) - blockGap;
	float blockMaxHeight = 200;

	for (unsigned int index = 0; index < (unsigned int)m_FFTSubbandInstanceEnergyBuffer.size(); ++index)
	{
		if (GetIsBeat(index))
		{
			IMGMANAGER->Getrect()->Color({ 0, 1, 0, 1 });
			IMGMANAGER->Getrect()->Position(((WINSIZEX * 0.1f + (blockWidth + blockGap) * index)) + g_ptCam.x, 50 + g_ptCam.y);
			IMGMANAGER->Getrect()->Scale(blockWidth, 100 + (blockMaxHeight * m_FFTSubbandInstanceEnergyBuffer[index]));
			IMGMANAGER->Getrect()->Render();
		}
	}
}

void FFTDetector::CaculateBpm()
{
	if (!m_HaveBeatsBeenInitialized)
	{
		for (int i = 0; i < NUMSUBBANDS; i++)
		{
			m_HasBeatBeenDetectedForSubband[i] = false;
		}
		m_HaveBeatsBeenInitialized = true;
	}

	UINT currentTime = 0;

	for (int i = 0; i < NUMSUBBANDS; i++)
	{
		queue<UINT> & beatTimes = m_BeatTimesForSubband[i];

		if (GetIsBeat(i))
		{
			if (!m_HasBeatBeenDetectedForSubband[i])
			{
				m_HasBeatBeenDetectedForSubband[i] = true;
				SOUNDMANAGER->GetCurrentMSTimePos(m_strKey, currentTime);
				beatTimes.push(currentTime);

				while (currentTime - beatTimes.front() > BEATCUTOFFMS)
				{
					beatTimes.pop();
					if (beatTimes.empty()) break;
				}
			}
		}
		else
		{
			m_HasBeatBeenDetectedForSubband[i] = false;
		}

		if (beatTimes.size() >= 2)
		{
			m_MillisecondsPerBeats = static_cast<float>(beatTimes.back() - beatTimes.front()) / static_cast<float>(beatTimes.size() - 1);
			m_BpmEstimates[i] = 60000 / m_MillisecondsPerBeats;
			float roundedBpmEstimates = static_cast<float>(floor(m_BpmEstimates[i] * 100.0f) / 100.0f);

			if (m_BeatHistory[i].find(roundedBpmEstimates) != m_BeatHistory[i].end())
			{
				m_BeatHistory[i][roundedBpmEstimates]++;
			}
			else
			{
				m_BeatHistory[i].insert(make_pair(roundedBpmEstimates, 1));
			}
		}
		else
		{
			m_BpmEstimates[i] = 0.0f;
		}
	}
}
