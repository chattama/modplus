#if !defined(UTILITY_H)
#define UTILITY_H

extern bool FileExists(const char* pcFileName);
extern void Basename(char* pcStripped, int nBufSize, 
		const char* pcFileName);

#endif // !defined(UTILITY_H)
