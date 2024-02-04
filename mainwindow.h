#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "guiinterface.h"

#include "paddletask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setParameters(int paramWpm, int paramFarn, int paramTone);

public slots:
  void handlePaddleEvent(int paddleEvent);
  void keyPressEvent(QKeyEvent * event);
  void keyReleaseEvent(QKeyEvent * event);
  void handleVolumeChanged();
  void handleWpmEdited();
  void handleFarnEdited();
  void handleToneEdited();
  void handleClearButton();
  void handleSendCharsEdited();
  void handleAutoKeyEnded();

protected:
  GuiInterface paddle ;
  void checkParameters();
  bool parametersChecked = false ;

  Ui::MainWindow *ui;
  bool ignoreEdits = false ;

  int wpm;
  int farn;
  int tone;

  };
#endif // MAINWINDOW_H
