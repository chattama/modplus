#if !defined(CMDLINE_H)
#define CMDLINE_H

#include <vector>
#include <string>

typedef std::vector<std::string> StringList;
typedef StringList::iterator SLIterator;
typedef StringList::const_iterator SLConstIterator;

// *******************************************************************
// *** Modification	for inlusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************

//extern bool ParseCommandLine();
//extern bool GetTemplateFileNames(StringList& TemplateList);

// END ***************************************************************

extern StringList gFileList;


#endif // !defined(CMDLINE_H)