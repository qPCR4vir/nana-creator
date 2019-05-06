/*
 *      Nana C++ Library - Creator
 *      Author: besh81
 */
#include <iostream>

#include <nana/gui/filebox.hpp>

#include "config.h"
#include "creator.h"
#include "guimanager.h"
#include "filemanager.h"
#include "inifile.h"
#include "codegenerator.h"
#include "new_project.h"


 // toolbar
#define TB_NEW					0
#define TB_LOAD					1
#define TB_SAVE					2
#define TB_SAVE_AS				3
 //
#define TB_GENERATE				5
 //
#define TB_DELETE				7
 //
#define TB_UP					9
#define TB_DOWN					10
 //
#define TB_CUT					12
#define TB_COPY					13
#define TB_PASTE				14


guimanager*					p_gui_mgr;	// manage all the gui elements
nana::filebox::path_type	prj_name;

extern inifile				g_inifile;



void creator::enableGUI(bool state, bool new_load)
{
	_tb.enable(TB_NEW, state ? true : new_load);
	_tb.enable(TB_LOAD, state ? true : new_load);
	_tb.enable(TB_SAVE, state);
	_tb.enable(TB_SAVE_AS, state);
	_tb.enable(TB_GENERATE, state);
	_tb.enable(TB_DELETE, state);
	_tb.enable(TB_UP, state);
	_tb.enable(TB_DOWN, state);
	_tb.enable(TB_CUT, state);
	_tb.enable(TB_COPY, state);
	_tb.enable(TB_PASTE, state);
}


bool creator::load_xml(const std::string& filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());

	if(!result)
	{
		std::cout << "XML [" << filename << "] parsed with errors\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << result.offset << "]\n\n";
		return false;
	}

	// read root node
	pugi::xml_node root = doc.child(NODE_ROOT);
	if(root.empty())
	{
		std::cout << "Error missing root node: " << NODE_ROOT << "\n";
		return false;
	}

	// deserialize the XML structure
	return p_gui_mgr->deserialize(&root);
}


bool creator::save_xml(const std::string& filename)
{
	pugi::xml_document doc;

	// append root node
	pugi::xml_node root = doc.append_child(NODE_ROOT);
	root.append_attribute("version") = CREATOR_VERSION;

	p_gui_mgr->serialize(&root);

	//doc.print(std::cout); //TEMP debug
	return doc.save_file(filename.c_str());
}


bool creator::generate_cpp()
{
	codegenerator cpp;
	cpp.generate(handle(), p_gui_mgr->get_root()->child, prj_name.parent_path().string());
	cpp.print(std::cout);
	return true;
}


void creator::sb_clear()
{
	_sb.caption("");
}


void creator::sb_set(const std::string& str)
{
	_sb.caption(str);
}


void creator::_init_ctrls()
{
	caption(CREATOR_NAME " " CREATOR_VERSION);

	// toolbar
	_tb.events().selected([this](const nana::arg_toolbar & arg)
	{
		if(arg.button == TB_NEW) // new project
		{
			// confirmation message
			if(prj_name != "")
			{
				nana::msgbox m(*this, "Save project", nana::msgbox::yes_no_cancel);
				m.icon(m.icon_warning);
				m << "Do you want to save the current project?";
				auto ret = m();
				if(ret == nana::msgbox::pick_cancel)
					return;
				else if(ret == nana::msgbox::pick_yes)
					save_xml(prj_name.string());
			}

			p_gui_mgr->clear();
			prj_name = "";

			new_project dlg(*this);
			dlg.modality();

			if(dlg.return_val() == nana::msgbox::pick_ok)
			{
				prj_name = dlg.get_filename();

				p_gui_mgr->new_project(dlg.get_ctrl_type(), dlg.get_projectname());
				
				// crea file di progetto
				save_xml(prj_name.string());
			}
		}
		else if(arg.button == TB_LOAD) // load project
		{
			// confirmation message
			if(prj_name != "")
			{
				nana::msgbox m(*this, "Save project", nana::msgbox::yes_no_cancel);
				m.icon(m.icon_warning);
				m << "Do you want to save the current project?";
				auto ret = m();
				if(ret == nana::msgbox::pick_cancel)
					return;
				else if(ret == nana::msgbox::pick_yes)
					save_xml(prj_name.string());
			}

			p_gui_mgr->clear();
			prj_name = "";

			nana::filebox fb(*this, true);
			fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
			fb.add_filter("All Files (*.*)", "*.*");

		#if defined(NANA_WINDOWS)
			// solve the problem with lpstrinitialdir
			auto p = equalize_path(g_inifile.load_project_dir(), '/', '\\');
			fb.init_file(p.empty() ? "./" : p + "\\.");
		#else
			fb.init_path(equalize_path(g_inifile.load_project_dir()));
		#endif

			auto paths = fb.show();
			if(!paths.empty())
			{
				// save load_project folder
				prj_name = paths[0];
				if(prj_name.parent_path().string() != g_inifile.load_project_dir())
					g_inifile.load_project_dir(prj_name.parent_path().string(), true);

				if(!load_xml(prj_name.string()))
					prj_name = "";
			}
		}
		else if(arg.button == TB_SAVE) // save project
		{
			if(prj_name == "")
				return;

			save_xml(prj_name.string());
		}
		else if(arg.button == TB_SAVE_AS) // save project as
		{
			if(prj_name == "")
				return;

			nana::filebox fb(*this, false);
			fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
			fb.init_file(prj_name.string());

			auto paths = fb.show();
			if(!paths.empty())
			{
				prj_name = paths[0];
				save_xml(prj_name.string());
			}
		}
		else if(arg.button == TB_GENERATE) // generate code
		{
			if(prj_name == "")
				return;

			generate_cpp();
		}
		else if(arg.button == TB_DELETE) // delete current selection
		{
			p_gui_mgr->deleteselected();
		}
		else if(arg.button == TB_UP) // move up current selection
		{
			p_gui_mgr->moveupselected();
		}
		else if(arg.button == TB_DOWN) // move down current selection
		{
			p_gui_mgr->movedownselected();
		}
		else if(arg.button == TB_CUT) // cut current selection
		{
			p_gui_mgr->cutselected();
		}
		else if(arg.button == TB_COPY) // copy current selection
		{
			p_gui_mgr->copyselected();
		}
		else if(arg.button == TB_PASTE) // paste into/after current selection
		{
			p_gui_mgr->pasteselected();
		}
	});

	// assets
	_place.field("assets") << _assets;

	// objects
	_place.field("objects") << _objects;

	// properties
	_place.field("properties") << _properties;

	// canvas
	_place.field("canvas") << _canvas;

	_place.collocate();


	p_gui_mgr = new guimanager(*this);
	p_gui_mgr->init(this, &_properties, &_assets, &_objects, &_canvas);
	
	sb_set("Ready");
}


void creator::_destroy_ctrls()
{
	p_gui_mgr->clear();
	delete p_gui_mgr;
}
