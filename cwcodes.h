#ifndef CWCODES_H
#define CWCODES_H

#include <string>
#include <algorithm>
#define NUM_STR_CODES 48

class CwCodes
{
public:

  const std::string morseCodes[NUM_STR_CODES][2] = {
    {"_____","0"},
    {".____","1"},
    {"..___","2"},
    {"...__","3"},
    {"...._","4"},
    {".....","5"},
    {"_....","6"},
    {"__...","7"},
    {"___..","8"},
    {"____.","9"},

    {"._",  "A"},
    {"_...","B"},
    {"_._.","C"},
    {"_..", "D"},
    {".",   "E"},
    {".._.","F"},
    {"__.", "G"},
    {"....","H"},
    {"..",  "I"},
    {".___","J"},
    {"_._", "K"},
    {"._..","L"},
    {"__",  "M"},
    {"_.",  "N"},
    {"___", "O"},
    {".__.","P"},
    {"__._","Q"},
    {"._.", "R"},
    {"...", "S"},
    {"_",   "T"},
    {".._", "U"},
    {"..._","V"},
    {".__", "W"},
    {"_.._","X"},
    {"_.__","Y"},
    {"__..","Z"},

    {"__..__",","},
    {"._._._","."},
    {"_.._.", "/"},
    {"_..._", "="},
    {"..__..","?"},
    {"._._.","+"},
    {"_...._","-"},
    {"_._.__","!"},

    {"._...","<AS>"},
    {"_.__.","<KN>"},
    {"..._._","<SK>"},
    {"_._._._","<CLR>"},
  };

  std::string getMorse(std::string prosign)
  {
    std::transform(prosign.begin(), prosign.end(), prosign.begin(), ::toupper);

    for (int i=0; i<NUM_STR_CODES; i++)
    {
        if (morseCodes[i][1] == prosign)
          return morseCodes[i][0];
    }

    return "" ;
  }

  std::string getProsign(std::string inMorse)
  {
      for (int i=0; i<NUM_STR_CODES; i++)
      {
          if (morseCodes[i][0] == inMorse)
            return morseCodes[i][1];
      }

      return "~";
  }

};

#endif // CWCODES_H
