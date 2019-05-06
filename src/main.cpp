/*
 *      Nana C++ Library - Creator
 *      Author: besh81
 */
#include <nana/gui/wvl.hpp>
#ifdef NANA_WINDOWS
#include <windows.h>
#endif //NANA_WINDOWS
#include "config.h"
#include "imagemanager.h"
#include "filemanager.h"
#include "inifile.h"
#include "creator.h"


imagemanager	g_img_mgr;
filemanager		g_file_mgr;	// manage absolute and relative path
inifile			g_inifile;


#ifdef NANA_WINDOWS
	#ifdef __RELEASE
	int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
	#else
	int main()
	#endif //__RELEASE
#else
	int main()
#endif //NANA_WINDOWS
{
	// init ctrls images
	g_img_mgr.add(CTRL_FIELD, "icons/horizontal_layout.png");
	g_img_mgr.add(CTRL_GRID, "icons/grid_layout.png");
	g_img_mgr.add(CTRL_SPLITTERBAR, "icons/splitter.png");
	g_img_mgr.add(CTRL_BUTTON, "icons/button.png");
	g_img_mgr.add(CTRL_LABEL, "icons/label.png");
	g_img_mgr.add(CTRL_TEXTBOX, "icons/textbox.png");
	g_img_mgr.add(CTRL_LISTBOX, "icons/listbox.png");
	g_img_mgr.add(CTRL_PANEL, "icons/panel.png");
	g_img_mgr.add(CTRL_COMBOX, "icons/combox.png");
	g_img_mgr.add(CTRL_SPINBOX, "icons/spinbox.png");
	g_img_mgr.add(CTRL_CHECKBOX, "icons/checkbox.png");
	g_img_mgr.add(CTRL_DATECHOOSER, "icons/datechooser.png");
	g_img_mgr.add(CTRL_TOOLBAR, "icons/toolbar.png");
	g_img_mgr.add(CTRL_FORM, "icons/form.png");
	g_img_mgr.add(CTRL_CATEGORIZE, "icons/categorize.png");
	g_img_mgr.add(CTRL_GROUP, "icons/group.png");
	g_img_mgr.add(CTRL_MENUBAR, "icons/menubar.png");
	g_img_mgr.add(CTRL_PICTURE, "icons/picture.png");
	g_img_mgr.add(CTRL_PROGRESS, "icons/progress.png");
	g_img_mgr.add(CTRL_SLIDER, "icons/slider.png");
	g_img_mgr.add(CTRL_TABBAR, "icons/tabbar.png");
	g_img_mgr.add(CTRL_TREEBOX, "icons/treebox.png");
	g_img_mgr.add(CTRL_NOTEBOOK, "icons/notebook.png");
	g_img_mgr.add(CTRL_PAGE, "icons/page.png");


	creator fm(0, nana::size{ 1200, 700 });
	fm.icon(nana::paint::image("icons/creator.ico"));

	fm.show();
	nana::exec();
}
