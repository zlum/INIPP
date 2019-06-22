#ifndef INIPP_FILE_H
#define INIPP_FILE_H

#include "chainlock.h"
#include "src/types/ippcache.h"
#include "src/types/ipptypes.hpp"

#include <fstream>
#include <iostream>
#include <vector>

class File
{
public:
    explicit File(const std::string& fileName = "example.txt");
    virtual ~File() = default;

protected:
    IPPCache* readFile();
    bool appendFile(IPP_Types::Parameter* parameter,
                    const std::string& section);
    bool writeFile(IPPCache* parameters);

protected:
    std::string fileName;

private:
    std::string _currentSection;
};

#endif // INIPP_FILE_H
