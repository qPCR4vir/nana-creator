/*
 *		nana::pg_items Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PG_ITEMS_H
#define NANA_CREATOR_PG_ITEMS_H

#include "nana_extra/propgrid_items.h"


using pgitem = nana::drawerbase::propertygrid::pgitem;



/// class pg_filename
class pg_filename
	: public nana::pg_string_button
{
public:
	pg_filename() = default;

	pg_filename(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

	virtual void value(const std::string& value) override;

	void add_filter(const std::string& description, const std::string& filetype);

protected:
	virtual void create(nana::window wd) override;

	std::vector<std::pair<std::string, std::string>> filters_;
};



/// class pg_folder
class pg_folder
	: public nana::pg_string_button
{
public:
	pg_folder() = default;

	pg_folder(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

	virtual void value(const std::string& value) override;

protected:
	virtual void create(nana::window wd) override;
};



/// class pg_collection
class pg_collection
	: public nana:: pg_string_button
{
public:
	pg_collection() = default;

	pg_collection(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

protected:
	virtual void create(nana::window wd) override;
};

#endif //NANA_CREATOR_PG_ITEMS_H