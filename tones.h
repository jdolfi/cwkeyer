#ifndef TONES_H
#define TONES_H

#include <sys/types.h>
#include "audiodriver.h"

enum SOUND_COMMANDS
{
  SOUND_OFF         = 0,
  SOUND_DIT         = 1,
  SOUND_DAH         = 2,
  SOUND_CHAR_SPACE  = 3,
  SOUND_WORD_SPACE  = 4,
  SOUND_TIMEOUT     = 5,
  SOUND_EXIT        = 6,
};

class Tones
{
public:
  Tones();
  ~Tones();
  void setWpm(int wpm);
  void setFarn(int farn);
  void setVolume(double vol);
  void setTone(double tn);
  void generateTones();
  void initAudio();
  void closeAudio();
  void playSound(SOUND_COMMANDS cmd);

  int16_t *ditSignalBuffer = nullptr; // S16LE
  int ditSignalSamples =0;
  int16_t *dahSignalBuffer = nullptr; // S16LE
  int dahSignalSamples =0;
  int16_t *charSpaceSignalBuffer = nullptr; // S16LE
  int charSpaceSignalSamples =0;
  int16_t *wordSpaceSignalBuffer = nullptr; // S16LE
  int wordSpaceSignalSamples =0;
  double u;

private:  
  int wpm = 20;
  int farn = 20;
  double ta, tc, tw ;
  double volume = .7;
  const double ramp_sec = .005;
  const int sample_rate = 4000 ;
  double freq =587 ;
  AudioDriver *pAudio = nullptr ;

  void createDit();
  void createDah();
  void createCharSpace();
  void createWordSpace();

};

#endif // TONES_H
