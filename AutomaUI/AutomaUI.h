// �U�C ifdef �϶��O�إߥ����H��U�q DLL �ץX���зǤ覡�C
// �o�� DLL �����Ҧ��ɮ׳��O�ϥΩR�O�C���ҩw�q AUTOMAUI_EXPORTS �Ÿ��sĶ���C
// �b�R�O�C�w�q���Ÿ��C����M�׳������w�q�o�ӲŸ�
// �o�ˤ@�ӡA��l�{���ɤ��]�t�o�ɮת������L�M��
// �|�N AUTOMAUI_API �禡�����q DLL �פJ���A�ӳo�� DLL �h�|�N�o�ǲŸ�����
// �ץX���C
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif

// �o�����O�O�q AutomaUI.dll �ץX��
class AUTOMAUI_API CAutomaUI {
public:
	CAutomaUI(void);
	// TODO:  �b���[�J�z����k�C
};

extern AUTOMAUI_API int nAutomaUI;

AUTOMAUI_API int fnAutomaUI(void);
