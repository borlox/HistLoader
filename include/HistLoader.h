#ifndef HIST_LOADER_H
#define HIST_LOADER_H

#include <istream>
#include <map>
#include <stdexcept>
#include <set>
#include <string>

#include <TH1.h>

typedef std::map<std::string,TH1*> HistMap;

class HistLoader
{
public:
	ClassDef(HistLoader, 1);

	struct ParserError : public std::runtime_error
	{
		ParserError(const std::string& msg, const std::string& fn, size_t ln);

		virtual ~ParserError() throw()
		{ }
	};

	struct HistDef
	{
		struct Value 
		{
			std::string str;
			std::vector<std::string> vec;
			bool isStr;

			Value()
			: isStr(true)
			{ }

			Value(std::string s)
			: str(s), isStr(true)
			{ }

			Value(std::vector<std::string> v)
			: vec(v), isStr(false)
			{ }

			Value& operator = (const std::string& s)
			{
				str = s;
				isStr = true;
				return *this;
			}

			Value& operator = (const std::vector<std::string>& v)
			{
				vec = v;
				isStr = false;
				return *this;
			}
		};

		//typedef boost::variant<std::string,std::vector<std::string>> Value;
		typedef std::map<std::string,Value> ValueMap;

		std::string name;
		bool isTemplate;
		bool isRange;
		size_t rangeCount;

		bool resolved;

		std::string origFile;
		size_t origLine;

		std::vector<std::string> parents;
		ValueMap attributes;

		HistDef()
		: isTemplate(false), isRange(false), rangeCount(0), resolved(false)
		{ }		

		bool Has(const std::string& attr)
		{
			return attributes.find(attr) != attributes.end();
		}

		std::string Get(const std::string& attr, int idx = -1, std::string def = "", bool noVar = false)
		{
			if (!Has(attr))
				return def;

			std::string var;

			Value& val = attributes[attr];
			if (val.isStr)
				var = val.str;
			else if (idx == -1)
				var = val.vec[0];
			else
				var = val.vec[idx];

			if (noVar)
				return var;
			return ResolveAllVariables(var, idx);
		}

	private:
		std::set<std::string> currentlyResolving;

		std::string ResolveAllVariables(std::string val, int idx = -1);
		std::string ResolveVariable(const std::string& varName, int idx);		
	};	

	void LoadFile(std::string fileName);
	void LoadFromStream(std::istream& in, const std::string& name = "<input>");
	void CreateHistograms();

	//TH1* GetHistogram(std::string name);
	//template <typename T>
	//T* GetHistogram(std::string name);

	//HistMap& GetAllHistograms();

	HistDef& GetDef(const std::string& name)
	{
		if (defMap.find(name) == defMap.end())
			throw std::out_of_range(name);
		return defMap[name];
	}

	TH1& operator[](const std::string& hist)
	{
		if (histMap.find(hist) == histMap.end())
			throw std::out_of_range(hist);
		return *histMap[hist];
	}

private:
	typedef std::map<std::string,HistDef> HistDefMap;

	HistMap histMap;
	HistDefMap defMap;

	HistDef *curDef;

	std::string parserFileName;
	size_t parserLineNumber;

	std::set<std::string> currentlyResolving;

	void ParseLine(std::string line);
	void HandleDef(std::string line);
	void HandleSubDef(std::string line);

	void ResolveTemplates(HistDef& def);
	void ResolveTemplates();

	bool CheckTemplate(std::string& line);
	bool CheckRange(std::string& line);
	bool CheckParents(std::string& line);

	HistDef::Value GetValue(std::string line);

	void DebugDumpDef(HistDef* def);
};

static inline std::ostream& operator<<(std::ostream& out, const HistLoader::HistDef::Value& val)
{
	if (val.isStr) {
		return out << val.str;
	}
	else {
		out << "[";
		for (size_t i=0; i < val.vec.size(); ++i) {
		if (i > 0)
			out << ", ";
			out << val.vec[i];
		}
		return out << "]";	
	}
}

#endif //HIST_LOADER_H
