#include "tones.h"
#include <math.h>

#include "pulseaudio.h"

Tones::Tones()
{

}

void Tones::initAudio()
{
    pAudio = new PulseAudio();
    pAudio->create_source();
}

void Tones::closeAudio()
{
    if (pAudio)
    {
      pAudio->destroy_source();
      pAudio=nullptr;
    }
}

Tones::~Tones()
{
  if (ditSignalBuffer != nullptr)
    delete[] ditSignalBuffer ;

  if (dahSignalBuffer != nullptr)
    delete[] dahSignalBuffer ;
}

void Tones::setWpm(int inWpm)
{
  wpm = inWpm ;

}

void Tones::setVolume(double vol)
{
  volume = vol;
}

void Tones::setFarn(int frn)
{
  farn = frn;
}

void Tones::setTone(double tone)
{
  freq = tone;
}

void Tones::generateTones()
{
  u = 1.2/wpm ;

  // should deal with this at the GUI level, but here for protection
  if (wpm < 5)
    wpm = 5;
  else if (wpm > 50)
    wpm = 50;

  if (farn < 5)
    farn = 5;
  else if (farn > wpm)
    farn = wpm ;

  if (freq < 300)
    freq = 300;
  else if (freq > 2000)
    freq=2000;

  ta = (60.0*wpm-37.2*farn)/(wpm*farn);
  tw = 7.0*ta/19.0;
  tc = 3.0*ta/19.0;

  createDit();
  createDah();
  createCharSpace();
  createWordSpace();
}

void Tones::createDit()
{
  double sound_sec = u ;
  double space_sec = u ;
  double total_sec = sound_sec + space_sec;

  if (ditSignalBuffer != nullptr)
    delete[] ditSignalBuffer ;

  ditSignalSamples = sample_rate*total_sec;
  ditSignalBuffer = new int16_t[ditSignalSamples];
  int soundSamples = sample_rate*sound_sec;

  int rampSamples = sample_rate*ramp_sec;
  double rampRate = volume / rampSamples ;
  double amplitude = 0;

  for(int i=0; i<soundSamples; i++)
  {
    if (i<rampSamples)
      amplitude += rampRate ;
    else if ( i>(soundSamples-rampSamples) )
      amplitude -= rampRate ;
    else
      amplitude = volume ;

    ditSignalBuffer[i] = 32766.0*amplitude*sin(2*3.14159*freq*i/sample_rate );
  }

  for(int i=soundSamples; i<ditSignalSamples; i++)
    ditSignalBuffer[i] = 0;
}

void Tones::createDah()
{
  double sound_sec = 3.0*u ;
  double space_sec = u ;
  double total_sec = sound_sec + space_sec;

  if (dahSignalBuffer != nullptr)
    delete[] dahSignalBuffer ;

  dahSignalSamples = sample_rate*total_sec;
  dahSignalBuffer = new int16_t[dahSignalSamples];
  int soundSamples = sample_rate*sound_sec;

  int rampSamples = sample_rate*ramp_sec;
  double rampRate = volume / rampSamples ;
  double amplitude=0;

  for(int i=0; i<soundSamples; i++)
  {
    if (i<rampSamples)
      amplitude += rampRate ;
    else if ( i>(soundSamples-rampSamples) )
      amplitude -= rampRate ;
    else
      amplitude = volume ;

    dahSignalBuffer[i] = 32766.0*amplitude*sin(2*3.14159*freq*i/sample_rate );
  }

  for(int i=soundSamples; i<dahSignalSamples; i++)
    dahSignalBuffer[i] = 0;
}

void Tones::createCharSpace()
{
  if (charSpaceSignalBuffer != nullptr)
    delete[] charSpaceSignalBuffer ;

  charSpaceSignalSamples = sample_rate*(tc-u);
  charSpaceSignalBuffer = new int16_t[charSpaceSignalSamples];

  for(int i=0; i<charSpaceSignalSamples; i++)
    charSpaceSignalBuffer[i] = 0;
}

void Tones::createWordSpace()
{
  if (wordSpaceSignalBuffer != nullptr)
    delete[] wordSpaceSignalBuffer ;

  wordSpaceSignalSamples = sample_rate*(tw-tc);
  wordSpaceSignalBuffer = new int16_t[wordSpaceSignalSamples];

  for(int i=0; i<wordSpaceSignalSamples; i++)
    wordSpaceSignalBuffer[i] = 0;
}

void Tones::playSound(SOUND_COMMANDS cmd)
{
  switch(cmd)
  {
    case SOUND_DIT:
      pAudio->write((const void *)ditSignalBuffer, (size_t)ditSignalSamples*2);
      break;

    case SOUND_DAH:
      pAudio->write((const void *)dahSignalBuffer, (size_t)dahSignalSamples*2);
      break;

    case SOUND_CHAR_SPACE:
      pAudio->write((const void *)charSpaceSignalBuffer,(size_t)charSpaceSignalSamples*2);
      break;

    case SOUND_WORD_SPACE:
      pAudio->write((const void *)wordSpaceSignalBuffer, (size_t)wordSpaceSignalSamples*2);
      break;

    default :
      break;
  }
}
