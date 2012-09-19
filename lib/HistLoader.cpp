#include <algorithm>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

#include "HistLoader.h"

static const std::string TEMPLATE = "template";
static const std::string RANGE = "range";


void HistLoader::LoadFile(std::string fileName)
{
	std::ifstream in(fileName.c_str());
	LoadFromStream(in, fileName);
}

void HistLoader::LoadFromStream(std::istream& in, const std::string& name)
{
	parserFileName = name;
	parserLineNumber = 0;

	std::string line;
	while (parserLineNumber++, std::getline(in, line)) {
		ParseLine(line);
	}

	ResolveTemplates();
	for (HistDefMap::iterator it = defMap.begin(); it != defMap.end(); ++it) {
		DebugDumpDef(&it->second);
	}	
}

void HistLoader::CreateHistograms()
{

}

void HistLoader::ParseLine(std::string line)
{
	std::string trimmed = boost::trim_copy(line);
	if (trimmed.empty() || trimmed[0] == '#')
		return;

	if (line[0] == '\t')
		HandleSubDef(trimmed);	
	else
		HandleDef(trimmed);
}

bool StartsWithFollowedBy(const std::string& str, const std::string& withWhat, const std::string& byWhat)
{
	if (!boost::starts_with(str, withWhat))
		return false;
	if (byWhat.empty())
		return true;
	if (str.size() <= withWhat.size())
		return false;
	return byWhat.find(str[withWhat.size()]) != byWhat.npos;
}

bool HistLoader::CheckTemplate(std::string& line)
{
	if (StartsWithFollowedBy(line, TEMPLATE, " \t")) {
		curDef->isTemplate = true;
		line = boost::trim_copy(line.substr(TEMPLATE.size()+1));
	}
	return curDef->isTemplate;
}

bool HistLoader::CheckRange(std::string& line)
{
	if (StartsWithFollowedBy(line, RANGE, " \t(")) {
		curDef->isRange = true;

		line = boost::trim_copy(line.substr(RANGE.size()));
		if (line.empty() || line[0] != '(') {
			throw ParserError("range definition not followed by '('", parserFileName, parserLineNumber);
		}
		size_t end = line.find(')');
		if (end == line.npos)
			throw ParserError("range definition misses closing ')'", parserFileName, parserLineNumber);

		try {
			curDef->rangeCount = boost::lexical_cast<size_t>(boost::trim_copy(line.substr(1, end-1)));
		}
		catch (boost::bad_lexical_cast err) {
			throw ParserError("'" + std::string(err.what()) + "' in range definiton", parserFileName, parserLineNumber);
		}
		line = boost::trim_copy(line.substr(end+1));
	}
	return curDef->isRange;
}

template <char X>
static bool ischar(char c)
{
	return c == X;
}

bool HistLoader::CheckParents(std::string& line)
{
	size_t cpos = line.find(':');
	if (cpos == line.npos)
		return false;

	std::string rest = line.substr(cpos+1);
	line = boost::trim_copy(line.substr(0, cpos));

	boost::split(curDef->parents, rest, ischar<','>);
	std::for_each(curDef->parents.begin(), curDef->parents.end(), boost::bind(boost::trim<std::string>, _1, std::locale()));
}

void HistLoader::HandleDef(std::string line)
{
	HistDef newDef;
	curDef = &newDef;

	newDef.origFile = parserFileName;
	newDef.origLine = parserLineNumber;

	if (!CheckTemplate(line))
		CheckRange(line);
	else {
		std::string lineCopy = line;
		if (CheckRange(lineCopy))
			throw ParserError("template and range in one definition is not supported", parserFileName, parserLineNumber);
	}

	CheckParents(line);

	if (line.empty())
		throw ParserError("histogram definition without a name", parserFileName, parserLineNumber);
	if (line == "template")
		throw ParserError("template without a name", parserFileName, parserLineNumber);

	if (defMap.find(line) != defMap.end())
		throw ParserError("'" + line + "' redefined", parserFileName, parserLineNumber);
	curDef->name = line;
	defMap[curDef->name] = newDef;
	curDef = &defMap[line];
}

HistLoader::HistDef::Value HistLoader::GetValue(std::string line)
{
	if (!boost::starts_with(line, "[") || !boost::ends_with(line, "]")) {
		return line;
	}
	std::vector<std::string> entries;
	line = line.substr(1, line.size()-2);
	boost::split(entries, line, ischar<';'>);
	std::for_each(entries.begin(), entries.end(), boost::bind(boost::trim<std::string>, _1, std::locale()));
	return entries;
}

void HistLoader::HandleSubDef(std::string line)
{
	size_t cpos = line.find(':');

	std::string name;
	HistDef::Value value;
	if (cpos == line.npos) {
		name = line;
		value = "";
	}
	else {
		name = boost::trim_copy(line.substr(0, cpos));
		value = GetValue(boost::trim_copy(line.substr(cpos+1)));
	}

	curDef->attributes[name] = value;
}

void HistLoader::ResolveTemplates(HistLoader::HistDef& def)
{
	if (def.resolved)
		return;

	if (currentlyResolving.count(def.name))
		throw ParserError("circular dependency detected for '" + def.name + "'", def.origFile, def.origLine);
	currentlyResolving.insert(def.name);

	for (size_t i=0; i < def.parents.size(); ++i) {
		std::string p = def.parents[i];
		if (defMap.find(p) == defMap.end())
			throw ParserError("could not find template '" + p + "'", def.origFile, def.origLine);
		HistDef& tpl = defMap[p];

		if (!tpl.resolved)
			ResolveTemplates(tpl);		
		if (!tpl.isTemplate)
			throw ParserError("'" + tpl.name + "' is not a template", def.origFile, def.origLine);

		for (HistDef::ValueMap::iterator it = tpl.attributes.begin(); it != tpl.attributes.end(); ++it) {
			if (!def.Has(it->first))
				def.attributes[it->first] = it->second;
		}
	}

	currentlyResolving.erase(def.name);
	def.resolved = true;
}

void HistLoader::ResolveTemplates()
{
	for (HistDefMap::iterator it = defMap.begin(); it != defMap.end(); ++it) {
		ResolveTemplates(it->second);
	}
}

void HistLoader::DebugDumpDef(HistDef* def)
{
	std::cout << def->name << ":\n"
	          << " template:   " << def->isTemplate << "\n"
	          << " range:      " << def->isRange << "\n";
	if (def->isRange)
		std::cout << " rangeCount: " << def->rangeCount << "\n";
	std::cout << " parents:\n";
	for (size_t i=0; i < def->parents.size(); ++i) {
		std::cout << "   '" << def->parents[i] << "'\n";
	}
	std::cout << " attributes:\n";
	for (HistDef::ValueMap::iterator it = def->attributes.begin(); it != def->attributes.end(); ++it) {
		std::cout << "   " << it->first << ": " << it->second << "\n";
	}
}

std::string HistLoader::HistDef::ResolveAllVariables(std::string val, int idx)
{
	size_t pos = val.find("$");
	while (pos != val.npos) {
		std::string varName = "$";
		size_t p = pos+1;
		while (p < val.size() && std::isalpha(val[p])) {
			varName += val[p];
			++p;
		}

		std::string res = ResolveVariable(varName, idx);
		boost::replace_first(val, varName, res);

		pos = val.find("$");
	}

	return val;
}

std::string HistLoader::HistDef::ResolveVariable(const std::string& varName, int idx)
{
	if (!Has(varName))
		return varName.substr(1);

	if (currentlyResolving.count(varName))
		throw std::runtime_error("circular dependency for variable '" + varName + "' in " + name);
	currentlyResolving.insert(varName);

	std::string res = Get(varName, idx, varName.substr(1), true);

	currentlyResolving.erase(varName);
	return res;
}
