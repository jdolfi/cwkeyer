#include "pulseaudio.h"

PulseAudio::~PulseAudio()
{
  if (s != nullptr)
  {
    pa_simple_free(s);
    s = nullptr;
  }
}

void PulseAudio::write(const void * data, size_t bytes)
{
  mutex.lock();
  pa_simple_write(s, data, bytes, &pa_error);
  pa_simple_drain(s,&pa_error);
  mutex.unlock();
}

void PulseAudio::create_source()
{

  /* Create a new playback stream */
  if (!(s = pa_simple_new(NULL,
                          "keyed audio",
                          PA_STREAM_PLAYBACK,
                          NULL,
                          "keyed",
                          &ss,
                          NULL,
                          NULL,
                          &pa_error)))
    {
      return ;
    }
}

void PulseAudio::destroy_source()
{
  if (s != nullptr)
  {
    pa_simple_free(s);
    s = nullptr;
  }
}
