#pragma once
#include "Base.h"

namespace AutoLib{
class MRGBLight:public MBase
{
	DECLARE_DYNAMIC(MRGBLight)
public:
	static std::map<CString,MRGBLight*> m_Members;
	static MRGBLight *GetLightControl(CString);
	static void AllLightsLoad();
	MRGBLight(MBase *pB,CString strID,CString strName,int intMin=0,int intMax=255);
	~MRGBLight(void);
	virtual bool SetValue(int R,int G,int B);
	virtual bool IncreaseValue(int R,int G,int B);
	virtual bool DescreaseValue(int R,int G,int B);
	virtual bool GetValue(int *R,int *G,int *B){*R=m_RValue;*G=m_GValue;*B=m_BValue;return true;};
	virtual bool SetOn(bool blOn){m_isOn=blOn;return m_isOn;};
	virtual bool GetOn(){return m_isOn;};
	virtual bool Active(){return false;};
	virtual void LoadRecipeData(CADOConnection * pC);
	virtual void SaveRecipeData(CADOConnection * pC);
	virtual bool Load();
	virtual bool Save();
	virtual void SetMaxMin(int MaxValue, int MinValue){m_Max=MaxValue;m_Min=MinValue;};	// 50706
	virtual int GetMax(){return m_Max;};	// 941109
	virtual int GetMin(){return m_Min;};	// 941109
	virtual void SetDataIDIndex(CString strDataID,CString strItemIndex); //設定在Table中要存取的ID及ItemIndex
	virtual void GetDataIDIndex(CString *pstrDataID,CString *pstrItemIndex); //設定在Table中要存取的ID及ItemIndex
protected:
	CADOConnection	*m_pADOcn;
	CString			m_csDataID;		// 取出資料時的索引
	CString			m_csItemIndex;		// 取出資料時的索引
	int m_RValue,m_GValue,m_BValue,m_Min,m_Max;
	bool m_isOn;
	DECLARE_MESSAGE_MAP()
};
}//namespace AutoLib
