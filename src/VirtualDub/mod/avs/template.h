#if !defined(TEMPLATE_H)
#define TEMPLATE_H

#include <vector>
#include <string>

struct TemplateInfo {
	std::string sFileName_;
	std::string sDescription_;
};

typedef std::vector<TemplateInfo> TemplateInfoList;
typedef TemplateInfoList::iterator TILIterator;
typedef TemplateInfoList::const_iterator TILConstIterator;
extern TemplateInfoList gTemplateList;

extern bool GetTemplateInfo();

#endif // !defined(TEMPLATE_H)
