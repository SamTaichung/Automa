// AutomaUI.cpp : �w�q DLL ���ε{�����ץX�禡�C
//

#include "stdafx.h"
#include "AutomaUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �Ȧ����@�����ε{������

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��l�� MFC �é󥢱ѮɦC�L���~
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: �t�X�z���ݭn�ܧ���~�X
            wprintf(L"�Y�����~: MFC ��l�ƥ���\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �b�����g���ε{���欰���{���X�C
        }
    }
    else
    {
        // TODO: �t�X�z���ݭn�ܧ���~�X
        wprintf(L"�Y�����~: GetModuleHandle ����\n");
        nRetCode = 1;
    }

    return nRetCode;
}
