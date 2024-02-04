
#include "guiinterface.h"

GuiInterface::GuiInterface()
{
  std::function<void(int)> decoderEventCallback = std::bind(&DecodeTask::processSoundEvent, &decodeTask, std::placeholders::_1);
  paddleTask.hookDecoderEvent(decoderEventCallback);

  std::function<void()> autoKeyCallback = std::bind(&GuiInterface::fireAutoKeyEnded, this);
  paddleTask.hookAutoKeyEnded(autoKeyCallback);

  std::function<void(std::string)> prosignCallback = std::bind(&GuiInterface::fireProsignDecoded, this, std::placeholders::_1);
  decodeTask.hookProsignDecoded(prosignCallback);

  std::function<void(int)> spaceCallback = std::bind(&GuiInterface::fireSpaceDecoded, this, std::placeholders::_1);
  decodeTask.hookSpaceDecoded(spaceCallback);
}

GuiInterface::~GuiInterface()
{
  paddleTask.processPaddleEvents(PADDLE_TASK_EXIT);
  decodeTask.processSoundEvent(SOUND_EXIT);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void GuiInterface::setWpm(int wpm) {
  paddleTask.setWpm(wpm);
  decodeTask.setWpm(wpm);
}

void GuiInterface::setFarn(int farn) {
  paddleTask.setFarn(farn);
}

void GuiInterface::setVolume(double vol) {
  paddleTask.setVolume(vol);
}

void GuiInterface::setTone(double tn) {
  paddleTask.setTone(tn);
}

void GuiInterface::initAudio(){
 paddleTask.initAudio();
}

void GuiInterface::generateTones(){
  paddleTask.generateTones();
}

void GuiInterface::closeAudio(){
  paddleTask.closeAudio();
}

void GuiInterface::autoSendKey(char sendChar)
{
  paddleTask.autoSendChar(sendChar);
}

void GuiInterface::processPaddleEvents(int paddleEvent)
{
  paddleTask.processPaddleEvents(paddleEvent);
}

void GuiInterface::fireProsignDecoded(std::string psign)
{
  prosignDecoded(QString(psign.c_str()));
}
void GuiInterface::fireSpaceDecoded(int space)
{
  spaceDecoded(space);
}

void GuiInterface::fireAutoKeyEnded()
{
  autoKeyEnded();
}
