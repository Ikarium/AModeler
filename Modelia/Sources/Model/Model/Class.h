#pragma once

#include <QString>
#include <QHash>

namespace Model
{

enum class BaseType {String, Integer, Float, Bool, Undefined};

struct Class
{
    QString name;
    QHash<QString, BaseType> properties;

    Class() {}

	Class(PropertyTree & ptree) { set(ptree); }

	bool operator ==(Class const & other_class) const
	{
		return (other_class.name == name);
	}
	bool operator !=(Class const & other_class) const
	{
		return (other_class.name != name);
	}
	operator QString () const
	{
		return name;
	}

	PropertyTree get() const
	{
		PropertyTree ptree;

		ptree.put_value("Class");
		ptree.put("Name", name.toStdString());
		for (auto i = properties.begin(); i != properties.end(); ++i)
		{
			PropertyTree property;
			property.add("Name", i.key().toStdString());
			property.add("BaseType", (uint)i.value());
			ptree.add_child("Properties.Property", property);
		}

		return ptree;
	}

	void set(PropertyTree & ptree)
	{
		if (QString::fromStdString(ptree.get_value<std::string>()) == "Class");

		name = QString::fromStdString(ptree.get<std::string>("Name"));

		for (auto property : ptree.get_child("Properties"))
			properties.insert(QString::fromStdString(property.second.get<std::string>("Name")),
								(BaseType) property.second.get<uint>("BaseType"));

	}

};

struct Type
{
    Class entryClass;
    bool vectorized;

	Type() {}

	Type(PropertyTree & ptree) { set(ptree); }

	Type(Type const & type)
		: entryClass(type.entryClass), vectorized(type.vectorized) {}

	bool operator ==(Type const & other_type) const
	{
		return (other_type.entryClass == entryClass
			&&	other_type.vectorized == vectorized)
			? true : false;
	}
	bool operator !=(Type const & other_type) const
	{
		return (other_type.entryClass == entryClass
			&&	other_type.vectorized == vectorized)
			? false : true;
	}

	operator QString () const
	{
		return (static_cast<QString>(entryClass)+(vectorized ? "(Vector)" : "(Non-Vector)"));
	}

	PropertyTree get() const
	{
		PropertyTree ptree;

		ptree.put_value("Type");
		ptree.put("Vectorized", vectorized);
		ptree.put_child("Class", entryClass.get());

		return ptree;
	}

	void set(PropertyTree & ptree)
	{
		if (QString::fromStdString(ptree.get_value<std::string>()) == "Type");

		if (ptree.count("Vectorized") == 0)
			ptree.put("Vectorized", false);

		vectorized = ptree.get<bool>("Vectorized");
		entryClass = Class(ptree.get_child("Class"));

	}
};



}
