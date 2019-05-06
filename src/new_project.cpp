/*
 *		new_project Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "new_project.h"
#include <nana/gui/filebox.hpp>
#include "imagemanager.h"
#include "filemanager.h"
#include "inifile.h"


extern imagemanager		g_img_mgr;
extern inifile			g_inifile;


 //items_dialog
void new_project::init()
{
	// type list
	auto cat = type_lst.at(0);
	cat.append(CTRL_FORM).icon(nana::paint::image(g_img_mgr.path(CTRL_FORM)));
	cat.append(CTRL_PANEL).icon(nana::paint::image(g_img_mgr.path(CTRL_PANEL)));


	// folder textbox
	folder_txt.caption(equalize_path(g_inifile.new_project_dir()));


	//------------------
	// events
	//------------------

	// type list
	type_lst.events().selected([this](const nana::arg_listbox& arg)
	{
		if(arg.item.selected())
		{
			if(arg.item.text(0) == CTRL_FORM)
				type_txt.caption("Creates a nana::form widget.\n\nThe form widget represents a popup window. Overall it is a root widget which attaches the OS/Windowing system's native window.");
			else if(arg.item.text(0) == CTRL_PANEL)
				type_txt.caption("Creates a nana::panel widget.\n\nA panel widget is used for placing other widgets.");
		}
	});

	// name text
	name_txt.set_accept([](wchar_t c) -> bool
	{
		return (isalnum(c) || c == '-' || c == '_' || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
	});

	// folder button
	folder_btn.events().click([this]()
	{
		nana::folderbox folder_picker(*this, equalize_path(folder_txt.caption()));
		
		auto paths = folder_picker.show();
		if(!paths.empty())
		{
			folder_txt.caption(paths[0].string());

			// save new_project folder
			auto path = equalize_path(folder_txt.caption());
			if(path != g_inifile.new_project_dir())
				g_inifile.new_project_dir(path, true);
		}
	});

	// create button
	create_btn.events().click([this]()
	{
		// check project type
		auto sel = type_lst.selected();
		if(sel.size() == 0)
		{
			type_lst.focus();
			return;
		}

		// check project name
		if(name_txt.caption() == "")
		{
			name_txt.focus();
			return;
		}

		// check folder dir
		if(folder_txt.caption() == "")
		{
			folder_txt.focus();
			return;
		}

		// check if file already exists
		_projectname = name_txt.caption();
		_filename = equalize_path(folder_txt.caption() + "/" + _projectname + "." PROJECT_EXT);
		if(file_exists(_filename))
		{
			nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::yes_no);
			m.icon(m.icon_warning);
			m << "A project with that name already exists. Do you want to replace it?";
			if(m() == nana::msgbox::pick_no)
			{
				return;
			}
		}


		auto cat = type_lst.at(0);
		for(auto c : cat)
		{
			if(c.selected())
			{
				_ctrltype = c.text(0);
				break;
			}
		}

		_retval = nana::msgbox::pick_ok;
		nana::API::close_window(handle());
	});

	// cancel button
	canc_btn.events().click([this]()
	{
		_retval = nana::msgbox::pick_cancel;
		nana::API::close_window(handle());
	});

	// select first item
	cat.at(0).select(true, true);
}

