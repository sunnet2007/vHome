#include "BaseControlWnd.h"

int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	CPaintManagerUI::SetInstance(hInstance);

	CBaseControlWnd* wnd = new CBaseControlWnd;
	wnd->Create(NULL, _T("»ù´¡¿Ø¼þ"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	wnd->CenterWindow();
	wnd->ShowModal();
	delete wnd;
	wnd = NULL;
	return 0;
}