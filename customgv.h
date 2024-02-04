#ifndef CUSTOMGV_H
#define CUSTOMGV_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QString>

class CustomGv : public QGraphicsView
{
  Q_OBJECT

public:
    CustomGv(QWidget* parent = 0 );
    void clearScene();

signals:
    void paddleEventSignal(int paddleevent);

public slots:
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent * event);
  void keyReleaseEvent(QKeyEvent * event);
  void receiveString(QString aString);
  void receiveSpace(int units);

 private:
  QGraphicsScene *scene = new QGraphicsScene(this);
  int x=0;
  int y=0;

};

#endif // CUSTOMGV_H
