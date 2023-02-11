#ifndef __LOCATION_HPP__
#define __LOCATION_HPP__

#include <vector>
#include <string>
#include <set>

class Location{
	private:
		static const std::set<std::string>	directives;

		std::string autoindex; // no duplicate
		std::string root; // no duplicate change to pair? ex: pair(default, "path") / pair ("set", "path");
		std::


	public:
		Location();
		~Location();
		Location(const Location &ref);
		Location &operator=(const Location &ref);
		Location(const std::string &location_str);

		const std::string &getRoot() const;
		const std::string &getReturn() const;
		const std::string &getIndex() const;


};

#endif
