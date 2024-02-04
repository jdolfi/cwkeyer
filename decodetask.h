#ifndef DECODETASK_H
#define DECODETASK_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "cwcodes.h"

enum DECODER_EVENTS
{
  DECODER_EVENT_NONE        = 0,
  DECODER_EVENT_DIT_RISE    = 1,
  DECODER_EVENT_DIT_FALL    = 2,
  DECODER_EVENT_DAH_RISE    = 3,
  DECODER_EVENT_DAH_FALL    = 4,
  DECODER_EVENT_TIMEOUT     = 8,
  DECODER_EVENT_EXIT        = 9,
};

enum DECODE_STATES
{
  DECODE_STATE_IDLE       =  0,
  DECODE_STATE_CHAR_WAIT  =  1,
  DECODE_STATE_LONG_WAIT  =  2,
  DECODE_STATE_EXIT       =  3,
};

enum ONE_SHOT_STATE
{
  ONE_SHOT_IDLE     = 0,
  ONE_SHOT_RUNNING  = 1,
  ONE_SHOT_EXIT     = 2
};

class DecodeTask
{
public:
  DecodeTask();
  ~DecodeTask();
  void setWpm(int wpm);
  void processSoundEvent(int sound);
  void hookProsignDecoded(std::function<void(std::string)> callback);
  void hookSpaceDecoded(std::function<void(int)> callback);

protected:
  std::thread decodeTh;
  std::mutex decodeMtx;
  std::condition_variable decodeCv;
  volatile DECODE_STATES decodeState = DECODE_STATE_IDLE;
  volatile uint16_t decodeEvents =0;
  void decodeLoop();

  std::thread oneShotTh;
  std::mutex oneShotMtx;
  std::condition_variable oneShotCv;
  volatile ONE_SHOT_STATE oneShotState = ONE_SHOT_IDLE;
  volatile int64_t oneShotTimeout = 0;
  void oneShotLoop();
  void oneShotElapsed();
  void setOneShot(long ms);
  void idleOneShot();

  int timerId =-1 ;
  double u ;
  std::string decodeString ;
  int decodedU ;
  double to_ms = 1000.0 ;
  int64_t gapTimeStart ;
  int64_t gapTimeEnd ;
  CwCodes codes ;

  int64_t epoch_ms(int64_t additional_ms);

  std::function<void(std::string)> fireProsignDecoded = nullptr;
  std::function<void(int)> fireSpaceDecoded = nullptr;

  void CharReceived();
  void SpaceReceived();

};

#endif // DECODETASK_H
