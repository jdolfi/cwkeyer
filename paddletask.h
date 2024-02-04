#ifndef PADDLETASK_H
#define PADDLETASK_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "tones.h"
#include "decodetask.h"

enum PADDLE_EVENTS
{
  PADDLE_EVENT_NONE          = 0,
  PADDLE_EVENT_DIT_PRESSED   = 1,
  PADDLE_EVENT_DIT_RELEASED  = 2,
  PADDLE_EVENT_DAH_PRESSED   = 3,
  PADDLE_EVENT_DAH_RELEASED  = 4,
};

// Not pretty.  These will be stored in a uin16_t
// The bottom 8 bits will be used to transmit a character
enum PADDLE_TASK_FLAGS
{
  PADDLE_TASK_UNLATCH_ALL     =  0b0000000000000000,
  PADDLE_TASK_LATCH_DIT       =  0b0000000100000000,
  PADDLE_TASK_UNLATCH_DIT     =  0b0000001000000000,
  PADDLE_TASK_LATCH_DAH       =  0b0000010000000000,
  PADDLE_TASK_UNLATCH_DAH     =  0b0000100000000000,
  PADDLE_TASK_SOUND_CHARACTER =  0b0001000000000000,
  PADDLE_TASK_EXIT            =  0b0010000000000000,
};

class PaddleTask
{

public:
  PaddleTask();
  ~PaddleTask();
  void setWpm(int wpm);
  void setFarn(int farn);
  void setVolume(double vol);
  void setTone(double tn);
  void initAudio();
  void generateTones();
  void closeAudio();
  void autoSendChar(char sendChar);
  void processPaddleEvents(int paddleEvent);
  void hookAutoKeyEnded(std::function<void()> callback);
  void hookDecoderEvent(std::function<void(int)> callback);
  std::function<void(int)>fireDecoderEvent = nullptr;
  std::function<void()>fireAutoKeyEnded = nullptr ;


protected:
  std::thread th;
  std::mutex mtx;
  std::condition_variable cv;
  volatile uint16_t paddleTaskFlags =0;
  void paddleLoop();

  void sendDecoderEvent(DECODER_EVENTS evt);

  Tones tones;
};

#endif // PADDLETASK_H
