#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED

extern void displayHistory(const char*);

extern void updateHistory(const char*, char**, int);

extern std::string historyLookup(const char*, char*);

#endif
