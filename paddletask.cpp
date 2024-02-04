#include "paddletask.h"

PaddleTask::PaddleTask()
{
  th = std::thread( &PaddleTask::paddleLoop, this);
};

PaddleTask::~PaddleTask()
{
  paddleTaskFlags = PADDLE_TASK_EXIT ;
  cv.notify_all();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  th.join();
}

void PaddleTask::hookDecoderEvent(std::function<void(int)> callback)
{
  if (callback != nullptr)
    fireDecoderEvent=callback ;
}

void PaddleTask::hookAutoKeyEnded(std::function<void ()> callback)
{
  if (callback != nullptr)
    fireAutoKeyEnded = callback ;
}

void PaddleTask::sendDecoderEvent(DECODER_EVENTS evt)
{
  if (fireDecoderEvent != nullptr)
    fireDecoderEvent(evt);
}

void PaddleTask::setWpm(int wpm) {
  tones.setWpm(wpm);  
}

void PaddleTask::setFarn(int farn) {
  tones.setFarn(farn);
}

void PaddleTask::setVolume(double vol) {
  tones.setVolume(vol);
}

void PaddleTask::setTone(double tn) {
  tones.setTone(tn);
}

void PaddleTask::initAudio(){
 tones.initAudio();
}

void PaddleTask::generateTones(){
  tones.generateTones();
}

void PaddleTask::closeAudio(){
  tones.closeAudio();
}

void PaddleTask::autoSendChar(char sendChar)
{
  paddleTaskFlags = sendChar ;
  paddleTaskFlags |= PADDLE_TASK_SOUND_CHARACTER ;
  cv.notify_all();
}

void PaddleTask::processPaddleEvents(int paddleEvent)
{
  if (paddleEvent == PADDLE_EVENT_DIT_PRESSED)
  {
    paddleTaskFlags |= PADDLE_TASK_LATCH_DIT ;
    paddleTaskFlags &= ~PADDLE_TASK_UNLATCH_DIT ; // keep this code !
  }
  else if (paddleEvent == PADDLE_EVENT_DIT_RELEASED)
    paddleTaskFlags |= PADDLE_TASK_UNLATCH_DIT ;
  else if (paddleEvent == PADDLE_EVENT_DAH_PRESSED)
  {
    paddleTaskFlags |= PADDLE_TASK_LATCH_DAH ;
    paddleTaskFlags &= ~PADDLE_TASK_UNLATCH_DAH ; // keep this code !
  }
  else if (paddleEvent == PADDLE_EVENT_DAH_RELEASED)
    paddleTaskFlags |= PADDLE_TASK_UNLATCH_DAH ;

  cv.notify_all();
}

void PaddleTask::paddleLoop()
{
  std::unique_lock<std::mutex> lck(mtx);
  mtx.unlock();    
  while ( paddleTaskFlags != PADDLE_TASK_EXIT )
  {
    while (!paddleTaskFlags) cv.wait(lck);
    {
      if (paddleTaskFlags & PADDLE_TASK_SOUND_CHARACTER)
      {
        CwCodes codes;

        char keyChar = (char)(paddleTaskFlags & 0x00FF);
        paddleTaskFlags = PADDLE_TASK_UNLATCH_ALL;
        std::string morseString = codes.getMorse(std::string(1,keyChar));

        if ( morseString.length()==0)
          tones.playSound(SOUND_WORD_SPACE);
        else
          tones.playSound(SOUND_CHAR_SPACE);

        while (morseString.length()>0)
        {
            char nextTone = morseString[0];
            if (nextTone == '.')
            {
              sendDecoderEvent(DECODER_EVENT_DIT_RISE);
              tones.playSound(SOUND_DIT);
            }
            else if (nextTone == '_')
            {
              sendDecoderEvent(DECODER_EVENT_DAH_RISE);
              tones.playSound(SOUND_DAH);
            }
            morseString = morseString.erase(0,1);

            if ( paddleTaskFlags == PADDLE_TASK_EXIT )
              return ;
        }

        fireAutoKeyEnded();
      }

      while (paddleTaskFlags & (PADDLE_TASK_LATCH_DIT | PADDLE_TASK_LATCH_DAH))
      {
        if (paddleTaskFlags & PADDLE_TASK_LATCH_DIT)
        {
          sendDecoderEvent(DECODER_EVENT_DIT_RISE);
          tones.playSound(SOUND_DIT);
          if (paddleTaskFlags & PADDLE_TASK_UNLATCH_DIT)
            paddleTaskFlags &= ~(PADDLE_TASK_LATCH_DIT | PADDLE_TASK_UNLATCH_DIT);
        }
        if (paddleTaskFlags & PADDLE_TASK_LATCH_DAH)
        {
          sendDecoderEvent(DECODER_EVENT_DAH_RISE);
          tones.playSound(SOUND_DAH);
          if (paddleTaskFlags & PADDLE_TASK_UNLATCH_DAH)
            paddleTaskFlags &= ~(PADDLE_TASK_LATCH_DAH | PADDLE_TASK_UNLATCH_DAH);
        }

        if ( paddleTaskFlags == PADDLE_TASK_EXIT )
          return ;
      }

      if ( paddleTaskFlags == PADDLE_TASK_EXIT )
        return ;

      paddleTaskFlags = PADDLE_TASK_UNLATCH_ALL;

    } // while (!paddleTaskFlags) cv.wait(lck);
  } // while ( paddleFlags != PADDLE_TASK_EXIT)
}
