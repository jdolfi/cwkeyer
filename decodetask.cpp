#include <chrono>
#include "decodetask.h"

using namespace std::chrono;

DecodeTask::DecodeTask() {
  decodeTh = std::thread( &DecodeTask::decodeLoop, this);
  oneShotTh = std::thread( &DecodeTask::oneShotLoop, this);
}

void DecodeTask::setWpm(int wpm)
{
  u = 1.2/wpm ;
}

DecodeTask::~DecodeTask()
{
  processSoundEvent(DECODER_EVENT_EXIT);

  oneShotTimeout = 0;
  oneShotState = ONE_SHOT_EXIT ;
  oneShotCv.notify_all();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  decodeTh.join();
  oneShotTh.join();
}

void DecodeTask::hookProsignDecoded(std::function<void(std::string)> callback)
{
  fireProsignDecoded = callback ;
}

void DecodeTask::hookSpaceDecoded(std::function<void(int)> callback)
{
  fireSpaceDecoded = callback ;
}

void DecodeTask::processSoundEvent(int sound)
{
  decodeEvents = sound;
  decodeCv.notify_all();
}

void DecodeTask::decodeLoop() {

  std::unique_lock<std::mutex> lck(decodeMtx);
  decodeMtx.unlock();
  while ( decodeEvents != DECODER_EVENT_EXIT )
  {
    while (!decodeEvents) decodeCv.wait(lck);
    {
      if (decodeEvents == DECODER_EVENT_EXIT)
        return ;

      switch(decodeState)
      {
        case DECODE_STATE_IDLE:
          if (decodeEvents == DECODER_EVENT_DIT_RISE)
          {
              setOneShot((int)(2.5*u*to_ms));
              gapTimeStart = epoch_ms(2.0*u*to_ms);
              decodeString += '.';
              decodeState = DECODE_STATE_CHAR_WAIT ;
          }
          else if (decodeEvents == DECODER_EVENT_DAH_RISE)
          {
              setOneShot((int)(4.5*u*to_ms));
              gapTimeStart = epoch_ms(4.0*u*to_ms);
              decodeString += '_';
              decodeState = DECODE_STATE_CHAR_WAIT ;
          }
        break ;

        case DECODE_STATE_CHAR_WAIT:

          if (decodeEvents == DECODER_EVENT_DIT_RISE)
          {
              double timerVal = (2.5*u*to_ms);
              gapTimeStart = epoch_ms(2.0*u*to_ms);
              setOneShot(timerVal);
              decodeString += '.';
          }
          else if (decodeEvents == DECODER_EVENT_DAH_RISE)
          {
              setOneShot((int)(4.5*u*to_ms));
              gapTimeStart = epoch_ms(4.0*u*to_ms);
              decodeString += '_';
          }
          else if (decodeEvents == DECODER_EVENT_TIMEOUT)
          {
            setOneShot((int)(5000));
            CharReceived();
            decodeState = DECODE_STATE_LONG_WAIT ;
          }
        break ;

        case DECODE_STATE_LONG_WAIT:
          if (decodeEvents == DECODER_EVENT_DIT_RISE)
          {
              setOneShot((int)(2.5*u*to_ms));
              SpaceReceived();
              gapTimeStart = epoch_ms(2.0*u*to_ms);
              decodeString += '.';

              decodeState = DECODE_STATE_CHAR_WAIT ;
          }
          else if (decodeEvents == DECODER_EVENT_DAH_RISE)
          {
              setOneShot((int)(4.5*u*to_ms));
              SpaceReceived();
              gapTimeStart = epoch_ms(4.0*u*to_ms);
              decodeString += '_';
              decodeState = DECODE_STATE_CHAR_WAIT ;
          }
          else if (decodeEvents == DECODER_EVENT_TIMEOUT)
          {
              idleOneShot();
              SpaceReceived();

              decodeState = DECODE_STATE_IDLE ;
          }
        break ;

        default:
        break ;
      }

      decodeEvents = DECODER_EVENT_NONE ;
    }
  }
}

void DecodeTask::CharReceived()
{
  std::string prosign = codes.getProsign(decodeString);
  if (fireProsignDecoded != nullptr)
    fireProsignDecoded(prosign);
  decodeString.clear();
}

void DecodeTask::SpaceReceived()
{
  int64_t deltaT;
  double maxT = u*75.0*1000.0 ;
  double doubleT ;

  gapTimeEnd = epoch_ms(0);
  deltaT = gapTimeEnd - gapTimeStart;

  if (deltaT > maxT)
    doubleT = maxT ;
  else
    doubleT = deltaT ;

  doubleT /= 1000 ;
  doubleT /= u;

  if (fireSpaceDecoded != nullptr)
    fireSpaceDecoded(doubleT);
}

int64_t DecodeTask::epoch_ms(int64_t additional_ms)
{
return duration_cast< milliseconds >
  (system_clock::now().time_since_epoch()).count() + additional_ms ;
}

void DecodeTask::oneShotLoop() {

  std::unique_lock<std::mutex> lck(oneShotMtx);
  oneShotMtx.unlock();
  while ( oneShotState != ONE_SHOT_EXIT )
  {
    while (!oneShotState) decodeCv.wait(lck);
    {
      if (oneShotState == ONE_SHOT_EXIT)
        return ;

      int64_t nowMs;
      do
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        nowMs = epoch_ms(0);

      } while (nowMs < oneShotTimeout);

      if (oneShotTimeout != 0)
        oneShotElapsed();

      if (oneShotState != ONE_SHOT_EXIT)
        oneShotState = ONE_SHOT_IDLE;
    }
  }
}

void DecodeTask::setOneShot(long ms)
{
  oneShotTimeout = epoch_ms(ms);
  oneShotState = ONE_SHOT_RUNNING;
  oneShotCv.notify_all();
}

void DecodeTask::idleOneShot()
{
  oneShotTimeout = 0;
}

void DecodeTask::oneShotElapsed()
{
  processSoundEvent(DECODER_EVENT_TIMEOUT);
}


