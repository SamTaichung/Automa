#pragma once
using namespace std;
#include <math.h>					
#include "MotionLib.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib {
	AUTOMALIB_API class MMotionLibVirtual :
		public MMotionLib
	{
		enum Step {
			stpDone,
			stpAcc,
			stpDec,
			stpHiSpeed,
		};
		struct VirtualMotorData {
			double m_dblLastDist, m_dblTargetPos, m_dblCalSpeed;
			double m_dblMaxSpeed, m_dblStartSpeed;
			double m_dblAccTime,m_dblDesTime;
			double m_dblPos;
			Step m_mvStep;
		};
	public:
		AUTOMALIB_API MMotionLibVirtual(MBase *pB, CString MotorID, CString strName, int AxisCount);
		AUTOMALIB_API ~MMotionLibVirtual();
		AUTOMALIB_API bool Init(MMotor* pMotor);
		AUTOMALIB_API void Cycle(const double dblTime);
		AUTOMALIB_API bool isMotion(MMotor* pMotor);
		AUTOMALIB_API bool GetSpeed(MMotor* pMotor,double *pSpeed);
		AUTOMALIB_API bool GetPosition(MMotor* pMotor,double *pPosition);
	protected:
		bool AMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblDesTime,double dblPos);
		bool RMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblDesTime, double dblMove);
		bool Stop(MMotor* pMotor, double dblDesTime);
		bool EStop(MMotor* pMotor);
		bool Home(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblOffset);
		bool VMove(MMotor* pMotor, bool isP, double sp = 0) { return true; };
		bool GotoLimit(MMotor* pMotor, bool isP, bool bSlow = true) { return true; };
		int GetMotorIndex(MMotor* pMotor);
		int m_intMotorInitCount;
		CArray<VirtualMotorData*> m_AxisArray;
		map<MMotor*, int> m_mapMotorIndex;
	};
}
