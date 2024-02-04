#include "customgv.h"
#include <QGraphicsTextItem>
#include "paddletask.h"

CustomGv::CustomGv(QWidget* parent )
    : QGraphicsView( parent )
{
  scene = new QGraphicsScene(this);  
}

void CustomGv::clearScene()
{
  delete scene ;
  scene = new QGraphicsScene(this);

  x=0;
  y=0;
}

void CustomGv::receiveString(QString aString)
{

  if (aString == "<CLR>")
  {
      clearScene();
      return ;
  }

  QFont f;
  f.setPointSize(15);

  QGraphicsTextItem *text = new QGraphicsTextItem();
  text->setPlainText(aString);
  text->setFont(f);
  text->setPos(x,y);
  x = x + text->boundingRect().width() - 8 ;

  // Add this space to keep the right side of the decode line visible
  QGraphicsRectItem *space = new QGraphicsRectItem(0,0,200,1);
  space->setPen(QPen(Qt::GlobalColor::white));
  space->setPos(x,y);

  this->ensureVisible(text);
  scene->addItem(text);
  this->setScene(scene);

  this->ensureVisible(space);
  scene->addItem(space);
  this->setScene(scene);

}

void CustomGv::receiveSpace(int units)
{

  int width = units*2 ;
  x = x + width ;

}

void CustomGv::mousePressEvent(QMouseEvent *e)
{
  if(e->button()&Qt::LeftButton)
  {
    paddleEventSignal(PADDLE_EVENT_DIT_PRESSED);
  }
  if(e->button()&Qt::RightButton)
  {
    paddleEventSignal(PADDLE_EVENT_DAH_PRESSED);
  }
}

void CustomGv::mouseReleaseEvent(QMouseEvent *e)
{
  if(e->button()&Qt::LeftButton)
  {
    paddleEventSignal(PADDLE_EVENT_DIT_RELEASED);
  }
  if(e->button()&Qt::RightButton)
  {
    paddleEventSignal(PADDLE_EVENT_DAH_RELEASED);
  }
}

void CustomGv::keyPressEvent(QKeyEvent * event)
{
  int scanCode = 0;

  scanCode = event->nativeScanCode();
  if (scanCode == 37)
  {
    paddleEventSignal(PADDLE_EVENT_DIT_PRESSED);
  }
  if (scanCode == 105)
  {
    paddleEventSignal(PADDLE_EVENT_DAH_PRESSED);
  }
}

void CustomGv::keyReleaseEvent(QKeyEvent * event)
{
  int scanCode = 0;

  scanCode = event->nativeScanCode();
  if (scanCode == 37)
  {
    paddleEventSignal(PADDLE_EVENT_DIT_RELEASED);
  }
  if (scanCode == 105)
  {
    paddleEventSignal(PADDLE_EVENT_DAH_RELEASED);
  }
}
