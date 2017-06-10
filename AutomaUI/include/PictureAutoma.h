#pragma once
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif
namespace AutoUI {
	AUTOMAUI_API class PictureAutoma : public CStatic
	{
		DECLARE_DYNAMIC(PictureAutoma)

	public:
		AUTOMAUI_API PictureAutoma();
		AUTOMAUI_API virtual ~PictureAutoma();
		AUTOMAUI_API HRESULT LoadPictureFile(CString strFile);
	protected:
		DECLARE_MESSAGE_MAP()
	};
}

