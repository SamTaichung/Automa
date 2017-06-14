#pragma once
#include "Coordinate.h"
using namespace AutoLib;
struct ComponentData {
public:
	ComponentData()
	{
		m_strID = _T("");
		m_strName = _T("");
		m_dblHight = 0;
	}; 
	CString m_strID, m_strName;
	double m_dblHight;		//元件高度
	double m_dblPickAngle;	//取料角度
};
struct PluginPoint
{
public:
	PluginPoint() {
		m_strComponentID = _T("");
		m_bPlugFinish = false;
		m_intArrangeArm = -1;
		m_pComponent = NULL;
	};
	CString m_strComponentID;
	ComponentData *m_pComponent;
	DCoordinate m_cdPlugPosition;
	DCoordinate m_cdPlugOffset;
	bool m_bPlugFinish; 
	int m_intArrangeArm; //安排的Arm
};
struct PCBRecipe
{
public:
	PCBRecipe() {
		m_intNumOfX = 1;
		m_intNumOfY = 1;
		m_dblXPitch = 0;
		m_dblYPitch = 0;
		m_intXIndex = 0;
		m_intYIndex = 0;
		m_intComponentIndex = 0;
	};
	~PCBRecipe() {
		RemoveAllPlugPoint();
	};
	DCoordinate m_cdBase;				//X,Y,Z方向的基準點座標
	int m_intNumOfX, m_intNumOfY;		//X,Y方向的板數
	double m_dblXPitch, m_dblYPitch;	//X,Y方向的Pitch
	int m_intXIndex, m_intYIndex;		//目前插件中的PCB板X,Y向計數
	int m_intComponentIndex;			//目前插件中的Component序號
	DCoordinate m_cdPCBMark[4];			//PCB Mark基準位
	void ResetData() {
		ResetPreArrangeData();
		ResetFinishedData();
		m_intXIndex = 0;
		m_intYIndex = 0;
	}
	void ResetFinishedData() {
		for (int i = 0; i < m_PointAry.GetCount(); i++)
		{
			m_PointAry[i]->m_bPlugFinish = false;
		};
	}
	void ResetPreArrangeData() {
		for (int i = 0; i < m_PointAry.GetCount(); i++)
		{
			m_PointAry[i]->m_intArrangeArm = -1;
		};
	}
	void NextPCB() {
		ResetFinishedData();	//清除完成資料
		ResetPreArrangeData();	//清除預安排資料
		if ((m_intXIndex + 1) < m_intNumOfX)
		{
			m_intXIndex++;
		}
		else {
			m_intXIndex = 0;
			if ((m_intYIndex + 1) < m_intNumOfY)
			{
				m_intYIndex++;
			}
			else {
				m_intYIndex = 0;
			}
		}
	}
	void SetNumberOfX(int NX) {
		if (NX<1)
		{
			m_intNumOfX = 1;
		}
		else {
			m_intNumOfX = NX;
		}
		m_intXIndex = 0;
	};
	void SetNumberOfY(int NY) {
		if (NY<1)
		{
			m_intNumOfY = 1;
		}
		else {
			m_intNumOfY = NY;
		}
		m_intYIndex = 0;
	};
	DCoordinate GetPlugPos(int index) {
		DCoordinate cdRet,cdPCB;
		PluginPoint *pPoint = GetPlugPoint(index);
		cdPCB = m_cdBase;
		cdPCB.x = m_intXIndex*m_dblXPitch;
		cdPCB.y = m_intYIndex*m_dblYPitch;
		cdRet =cdPCB+pPoint->m_cdPlugPosition + pPoint->m_cdPlugOffset;
		return cdRet;
	}
	bool IsLastPCB() {
		bool bRet;
		if ((m_intXIndex + 1) == m_intNumOfX && (m_intYIndex + 1) == m_intNumOfY)
		{
			bRet = true;
		}
		else {
			bRet = false;
		}
		return bRet;
	}
	int GetPlugPointCount() {
		return m_PointAry.GetCount();
	}
	void RemoveAllPlugPoint() {
		for (int i = 0; i < m_PointAry.GetCount(); i++)
		{
			delete m_PointAry[i];
		};
		m_PointAry.RemoveAll();
	};
	void AddPlugPoint(PluginPoint* pPoint)
	{
		m_PointAry.Add(pPoint);
	};
	void RemovePlugPoint(int index)
	{
		m_PointAry.RemoveAt(index, 1);
	}
	PluginPoint *GetPlugPoint(int index)
	{
		return m_PointAry[index];
	}
private:
	CArray<PluginPoint*> m_PointAry;
};