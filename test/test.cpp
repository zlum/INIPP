#ifdef IPP_TEST_BUILD
#include "inipp.hpp"

using namespace std;

int main()
{
    IniPP configFile("test.txt", IPP::Mode::cache);

    // Write default parameters (if there were such parameters into file
    // then their value will not be changed)
    if(!configFile.setDefaultParameter("LastTermostatTime", 413.2))
        cout << "LastTermostatTime parameter set failed" << endl;
    if(!configFile.setDefaultParameter("Boolean", true))
        cout << "Boolean parameter set failed" << endl;
    if(!configFile.setDefaultParameter("LastTime", 213))
        cout << "LastTime parameter set failed" << endl;
    if(!configFile.setDefaultParameter("Lame", "maximum"))
        cout << "Lame parameter set failed" << endl;

    // Read parameters
    string s;
    if(!configFile.getParameter("Lame", s))
        cout << "Lame parameter hasn`t found" << endl;
    else
        cout << "Lame " << s << endl;

    float num;
    if(!configFile.getParameter("LastTermostatTime", num))
        cout << "LastTermostatTime parameter hasn`t found" << endl;
    else
        cout << "LastTermostatTime " << num << endl;

    double dbl;
    if(!configFile.getParameter("LastTermostatTime", dbl))
        cout << "LastTermostatTime parameter hasn`t found" << endl;
    else
        cout << "LastTermostatTime " << dbl << endl;

    bool boolean;
    if(!configFile.getParameter("Boolean", boolean))
        cout << "Boolean parameter hasn`t found" << endl;
    else
        cout << "Boolean " << boolean << endl;

    // This parameter will not be found because it wasn`t set
    int intNum = 10;
    if(!configFile.getParameter("PartyTime", intNum))
        cout << "PartyTime parameter hasn`t found" << endl;
    else
        cout << "PartyTime " << intNum << endl;

    // Parameter value will be set equal to variable cause it wasn`t set
    char ch = 'L';
    if(!configFile.getParameterForced("Letter", ch))
        cout << "Letter parameter hasn`t found" << endl;
    else
        cout << "Letter " << ch << endl;

    // Parameter value will be changed cause it was set
    int integer = 47;
    if(!configFile.getParameter("LastTime", integer))
        cout << "LastTime parameter hasn`t found" << endl;
    else
        cout << "LastTime " << integer << endl;

    // Change parameters values
    if(!configFile.setParameter("LastTime", 11121))
        cout << "LastTime parameter set failed" << endl;
    if(!configFile.setParameter("Lame", "over9k"))
        cout << "Lame parameter set failed" << endl;

    // Repeat parameters read
    string s1;
    if(!configFile.getParameter("Lame", s1))
        cout << "Lame parameter hasn`t found" << endl;
    else
        cout << "Lame " << s1 << endl;

    // Parameter will not be found cause its value
    // is out of int8_t range
    int8_t integer8bit = 20;
    if(!configFile.getParameter("LastTime", integer8bit))
        cout << "LastTime parameter hasn`t found" << endl;
    else
        cout << "LastTime " << integer8bit << endl;

    // Parameter can got variable number of values
    int8_t ltint8 = 42;
    string ltstr = "string";

    if(!configFile.setParameter("Args", ltint8, "mo ar"))
        cout << "Args parameter set failed" << endl;

    if(!configFile.getParameter("Args", ltint8, ltstr))
        cout << "Args parameter hasn`t found" << endl;
    else
        cout << "Args " << ltint8 << " " << ltstr << endl;

    // Change file section name
    // Parameters from other sections will not be read or written
    // There can be parameters with same names in different sections
    configFile.setSection("sect name, lol");

    time_t stdTime = time(nullptr);
    auto stringTime = ctime(&stdTime);

    // NOTE: Extra endl at first run in cahce mode cause endls will be erased only on file writing
    if(!configFile.getParameterForced("HumanTime", stringTime))
        cout << "HumanTime parameter hasn`t found" << endl;
    else
        cout << "HumanTime " << stringTime << endl;

    // Add comment to parameter
    string comment = "Like and Subscribe!";
    if(!configFile.setComment("HumanTime", comment))
        cout << "HumanTime comment set failed" << endl;

    if(!configFile.getComment("HumanTime", comment))
        cout << "HumanTime comment hasn`t found" << endl;
    else
        cout << "HumanTime comment " << comment << endl;

    // NOTE: Comments without parameters will be erased from file

    return 0;
}
#endif // IPP_TEST_BUILD
