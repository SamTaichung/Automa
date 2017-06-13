// PictureAutoma.cpp : 實作檔
//

#include "stdafx.h"
#include "PictureAutoma.h"


// PictureAutoma
using namespace AutoUI;
IMPLEMENT_DYNAMIC(PictureAutoma, CStatic)

PictureAutoma::PictureAutoma()
{

}

PictureAutoma::~PictureAutoma()
{
}
HRESULT PictureAutoma::LoadPictureFile(CString strFile)
{
	HDC hDC_Temp = ::GetDC(m_hWnd);
	CRect rect;
	this->GetWindowRect(rect);
	IPicture *pPic;
	IStream *pStm;

	BOOL bResult;

	HANDLE hFile = NULL;
	DWORD dwFileSize, dwByteRead;

	//打开硬盘中的图形文件
	//    hFile=CreateFile(lpstrFile,GENERIC_READ,
	//    FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	hFile = CreateFile(strFile, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hFile, NULL);//获取文件字节数

		if (dwFileSize == 0xFFFFFFFF)
			return E_FAIL;
	}
	else
	{
		return E_FAIL;
	}

	//記憶體分配
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	LPVOID pvData = NULL;

	if (hGlobal == NULL)
	{
		AfxMessageBox(_T("記憶體分配錯誤！"));
		return E_FAIL;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)
	{
		AfxMessageBox(_T("記憶體鎖定錯誤！"));
		return E_FAIL;
	}
	//Read to buffer
	ReadFile(hFile, pvData, dwFileSize, &dwByteRead, NULL);

	GlobalUnlock(hGlobal);

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStm) != S_OK)
	{
		AfxMessageBox(_T("資訊流建立失敗！"));
		return E_FAIL;
	}

	//装入图形文件
	bResult = OleLoadPicture(pStm, dwFileSize, TRUE, IID_IPicture, (LPVOID*)&pPic);

	if (FAILED(bResult))
	{
		AfxMessageBox(_T("圖檔文件載入錯誤！"));
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度
	OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	//輸出圖檔
	bResult = pPic->Render(hDC_Temp, 0, 0, rect.Width(), rect.Height(),
		0, hmHeight, hmWidth, -hmHeight, NULL);

	CloseHandle(hFile);//关闭打开的文件

	pPic->Release();
	// Free memory.
	GlobalFree(hGlobal);

	if (SUCCEEDED(bResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(_T("圖檔文件輸出錯誤！"));
		return E_FAIL;
	}


}

BEGIN_MESSAGE_MAP(PictureAutoma, CStatic)
END_MESSAGE_MAP()



// PictureAutoma 訊息處理常式


