#pragma once

#define SAMLERATEBITS 10
#define DETECTIONSAMLERATE 1 << SAMLERATEBITS
#define NUMSUBBANDS 32
#define NUMHISTORYVALUES 43
#define BEATCUTOFFMS 1000

class FFTDetector
{
	float* m_InstanceEnengyBuffer;
	float* m_EnergyHistoryBuffer;
	float* m_EnergyPeaks;
	UINT m_SoundPCMLenght;
	float m_MillisecondsPerBeats;

	vector<float> m_BpmEstimates;
	vector<float> m_FFTSubbandInstanceEnergyBuffer;
	vector<float> m_FFTSubbandAverageEnergy;

	vector<vector<float>> m_FFTSubbandEnergyHistoryBuffer;
	vector<queue<UINT>> m_BeatTimesForSubband;
	vector<map<float, UINT>> m_BeatHistory;

	bool m_HaveBeatsBeenInitialized;
	bool m_HasBeatBeenDetectedForSubband[32];

	float m_EnergyRation;
	string m_strKey;
public:
	FFTDetector();
	~FFTDetector();
	void BeatDetectionInit(string strKey);
	bool GetIsBeat(int Subband = 0);
	float GetBpmForSubband(int Subband = 0);
	float GetEnergyRation();
	void incrementEnergyRatioForBeatThreshold(float Value);
	void Update();
	void RenderFFTVisual(D3DXVECTOR2 StartPos, float MaxBarNum, D3DXVECTOR2 BarScale, float Offset);
	void Render();
private:
	int CalculateInstanEnergy(int* data, int dataOffset, int dataWindow);
	int CalculateInstanEnergy(int* leftdata, int* rightData, int dataOffset, int DataWindow);
	float* GetSquaredModsSpectrumData(float* leftdata, float* rightData, int DataWindow = 1024);
	void CalculateInstatEneries(float* SquareModeSpectrumData, vector<float>& instanctEnergyBuffer, int numSubband, int DataWindow = 1024);
	float CalculateAvergeEnergyForSubband(int Subband, vector<vector<float>>& EnergyHistoryBuffer);
	void UpdateEnergyHistoryBuffer(vector<float>& InstantEnergyBuffer, vector<vector<float>>& EnergyHistoryBuffer);
	void RenderInstantEnergies();

	void CaculateBpm();
};

