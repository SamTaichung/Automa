#pragma once

#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
// MCard
#include "Base.h"				
namespace AutoLib{
	AUTOMALIB_API class MUnit : public MBase
	{
	public:
		enum AutoRunMode {
			DryRun,		//�Ŷ]�Ҧ�
			NormalRun,	//���`�Ҧ�
		};
		AUTOMALIB_API MUnit(MUnit *pParent,CString strID,CString strName);
		AUTOMALIB_API virtual ~MUnit();
		AUTOMALIB_API virtual bool AutoStart();
		AUTOMALIB_API virtual void AutoStop();
		AUTOMALIB_API virtual void SetAutoRunMode(AutoRunMode mode);

	protected:
		bool m_bAutoEnd;	//�۰ʥͲ�����
		CArray<MUnit*> m_Units;
		std::map<void *,CString> m_mapDataName; //�Ѽƫ��й����W��
		AutoRunMode m_AutoRunMode;
	};

}//namespace AutoLib
