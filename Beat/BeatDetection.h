#pragma once
#include "Singleton/SingletonBase.h"

#define ENERGYRATIO 1.2 // ��ũ ������ ������ ���� Energie�� TotalEnergies�� ����
#define DIMPSIZE 108 // Colvolution�� ���� �޽� ���� ũ��(1024�� 1/43��)
#define SUBBAND 32 // ��� �뿪 32
#define DETECTIONSAMPLE 1024 // 1024���� ����
#define HISTORYVALUE 43

class BeatDetection : public SingletonBase<BeatDetection>
{
	string m_SongName;

	UINT m_Lenght;              // ���̰�
	vector<vector<float>> m_EnergySubbandHistoryBuffer;       
	vector<float> m_FFTSubbandInstanceEnergyBuffer;       // ���� �������� ������ vector ����
	vector<float> m_FFTSubbandAverageEnergy;              // ��� �������� ������ ����
	float * m_Energie;         // 1024 pcm���� 1024 ������ �������� ����Ѵ�.
	float * m_TotalEnergies;   // 1024 pcm���� 44100 ������ �������� ����Ѵ�.
	float * m_EnergiesPeak;   // ��Ʈ�� �߻��� �ִ� ��
	float * m_Colvolution;   // �޽� ���� ���� ���� �̵��� ���� ���� ����, ������ ���� �����Ͽ� ���� ��
	float * m_Beat;   // ��Ʈ ��
	float m_Occurrence;
	int m_OccurrenceMax;

	int m_Tempo;; // ���� ��
	//�־��� ��ġ�� ������ ��ȣ �������� ����ϴ� �Լ�
	int Energies(int * data, int offset, int window);
	// ��ȣ ���� ���ϴ� �ִ� ������ ������ ���ִ� �Լ�
	void Normalize(float * signal, int size, float max);
	// ������ ��ġ ��ó���� �ִ� ���� �˻����ִ� �Լ�
	int MaxSearch(float * signal, int pos, int halfsize);
	// ���� ������ ��� �Լ�
	int CalculateInstanEnergy(int* data, int dataOffset, int dataWindow);
	// ������ ��� ��� �Լ�
	float CalculateAvergeEnergyForSubband(int Subband, vector<vector<float>>& EnergyHistoryBuffer);
	// �����丮 ���۸� ������Ʈ �ϴ� �Լ�
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

