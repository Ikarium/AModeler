#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include "Gears/KExeption.hpp"
#include <sstream>
#include <QString>


using PropertyTree = boost::property_tree::ptree;
namespace Karma
{

struct PropertyTreeExeption : public KExeption
{
	PropertyTreeExeption(QString	const & file = "",
		int		const & line = 0,
		QString	const & message = "",
		int		const & code = 0)
		: KExeption("\nIn: " + file + "\t" + QString::number(line)
		+ "\n   Modelia exception: " + message + "\n", code)
	{

		std::string test;
		boost::property_tree::info_parser::read_info("", PropertyTree());
	}
};

inline void infoToPree(const std::string & string, PropertyTree & ptree)
{
	boost::property_tree::info_parser::read_info(string, ptree);
}

inline void ptreeToInfo(const std::string & file, PropertyTree & ptree)
{
	boost::property_tree::info_parser::write_info(file, ptree);
}

inline QString savePtree(PropertyTree & ptree)
{
	std::stringstream result;

	boost::property_tree::info_parser::write_info(result, ptree);

	return QString::fromStdString(result.str());
}

inline PropertyTree loadPtree(QString const & string)
{
	PropertyTree ptree;
	std::stringstream stream;
	stream << string.toStdString();
	boost::property_tree::info_parser::read_info(stream, ptree);
	return ptree;
}

}
