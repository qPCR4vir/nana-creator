/*
 *		guimanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_GUIMANAGER_H
#define NANA_CREATOR_GUIMANAGER_H

#include "config.h"
#include "pugixml/pugixml.hpp"
#include "tree.h"
#include "ctrls/ctrl.h"
#include "namemanager.h"
#include "creator.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"


typedef std::shared_ptr<ctrls::ctrl>	control_obj;
typedef std::weak_ptr<ctrls::ctrl>		control_obj_ptr;


enum class cursor_action
{
	select,
	add
};

struct cursor_state
{
	cursor_action	action;
	std::string		type;
};

enum class insert_mode
{
	before,
	into,
	after
};



class guimanager
{
public:
	guimanager(nana::window wd);

	void init(creator* ct, propertiespanel* pp, assetspanel* ap, objectspanel* op, resizablecanvas* main_wd);
	void clear();

	void enableGUI(bool state, bool new_load);

	void cursor(cursor_state state);
	cursor_state cursor() { return _cursor_state; }

	void new_project(const std::string& type, const std::string& name);

	tree_node<control_obj>* addmainctrl(const std::string& type, const std::string& name = "");
	tree_node<control_obj>* addcommonctrl(tree_node<control_obj>* node, const std::string& type, insert_mode mode, const std::string& name = "");

	void deleteselected();
	void moveupselected();
	void movedownselected();

	void cutselected() { copyselected(true); }
	void copyselected(bool cut = false);
	void pasteselected();


	tree_node<control_obj>* get_root()
	{
		return _ctrls.get_root();
	}


	void updateselected()
	{
		_updatectrl(_selected);
	}


	bool click_ctrl(control_obj ctrl, const nana::arg_mouse& arg);
	void left_click_ctrl(control_obj ctrl);
	void click_objectspanel(const std::string& name);


	void serialize(pugi::xml_node* xml_parent);
	bool deserialize(pugi::xml_node* xml_parent);
	

private:
	bool _check_relationship(control_obj parent, const std::string& child_type);

	control_obj _create_ctrl(control_obj parent, const std::string& type, const std::string& name);

	tree_node<control_obj>* _registerobject(control_obj ctrl, tree_node<control_obj>* node, insert_mode mode);

	void _serialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, bool children_only = false);
	bool _deserialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, bool paste = false);

	bool _updatectrlname(tree_node<control_obj>* node, const std::string& new_name);
	void _updatectrl(tree_node<control_obj>* node, bool update_owner = true, bool update_children = true);
	void _updatechildrenctrls(tree_node<control_obj>* node);

	void _update_op();

	void _select_ctrl(tree_node<control_obj>* to_select);


	nana::window			_root_wd;
	nana::menu				_ctxmenu;

	tree<control_obj>		_ctrls;
	tree_node<control_obj>*	_selected{ 0 };

	cursor_state			_cursor_state{ cursor_action::select };

	creator*				_ct{ 0 };
	propertiespanel*		_pp{ 0 };
	assetspanel*			_ap{ 0 };
	objectspanel*			_op{ 0 };
	resizablecanvas*		_main_wd{ 0 };

	namemanager				_name_mgr;	// manage the controls name used in the creator

	bool					_deserializing{ false };

	pugi::xml_document		_cut_copy_doc;
	bool					_copied{ false };
};

#endif //NANA_CREATOR_GUIMANAGER_H
