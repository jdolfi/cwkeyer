#ifndef PULSEAUDIO_H
#define PULSEAUDIO_H

#include "audiodriver.h"

#include <QMutex>
#include <pulse/simple.h>
#include <pulse/error.h>

class PulseAudio : public AudioDriver
{
public:

  ~PulseAudio();
  void create_source();
  void destroy_source();
  void write(const void * data, size_t bytes);

private:
  /* The Sample format to use */
  const pa_sample_spec ss = {
      .format = PA_SAMPLE_S16LE,
      .rate = 4000,
      .channels = 1
  };

  pa_simple *s = nullptr;
  int pa_error;

  QMutex mutex;

};

#endif // PULSEAUDIO_H
