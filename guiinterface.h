#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>

#include "paddletask.h"
#include "decodetask.h"

class GuiInterface : public QObject
{
  Q_OBJECT

public:
  GuiInterface();
  ~GuiInterface();
  void setWpm(int wpm);
  void setFarn(int farn);
  void setVolume(double vol);
  void setTone(double tn);
  void initAudio();
  void generateTones();
  void closeAudio();
  void autoSendKey(char sendChar);
  void processPaddleEvents(int paddleEvent);

protected:
  DecodeTask decodeTask ;
  PaddleTask paddleTask ;

  void fireProsignDecoded(std::string psign);
  void fireSpaceDecoded(int space);
  void fireAutoKeyEnded();

signals:
    void prosignDecoded(QString psign);
    void spaceDecoded(int space);
    void autoKeyEnded();
};

#endif // GUIINTERFACE_H
