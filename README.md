
# INI++

INI++ is a library to easily manipulate INI files with C++14.
There is support of multiple values for one paramter and attached comments.
Library interface provides stream type conversion to simplify usage.

More examples in test/test.cpp.

Library adding guide in INSTALL.cpp.

## Usage example:
```
#include "inipp.hpp"

int main()
{
    IniPP configFile("test.txt");

    int8_t ltint8 = 42;
    string ltstr = "str ing";

    configFile.getParameterForced("Args", ltint8, ltstr);

    std::cout << ltint8 << ' ' << ltstr << std::endl;

    return 0;
}
```

## Output file example:
```
LastTermostatTime = 413.2
Boolean = 1
LastTime = 11121
Lame = over9k
Letter = L
Args = 42, mo ar

[sect name, lol]
HumanTime = Sat Jun 22 19:56:22 2019; Like and Subscribe!
```
