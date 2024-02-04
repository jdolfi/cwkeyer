#include <QGraphicsTextItem>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  paddle.initAudio();

  ui->setupUi(this);
  paddle.setVolume(ui->volume_slider->value()/100.0);

  connect(ui->wpm_edit, &QLineEdit::textEdited, this, &MainWindow::handleWpmEdited);
  connect(ui->farn_edit, &QLineEdit::textEdited, this, &MainWindow::handleFarnEdited);
  connect(ui->tone_edit, &QLineEdit::textEdited, this, &MainWindow::handleToneEdited);
  connect(ui->send_chars_edit, &QLineEdit::textEdited, this, &MainWindow::handleSendCharsEdited);
  connect(ui->volume_slider, &QSlider::valueChanged, this, &MainWindow::handleVolumeChanged);
  connect(ui->clearButton, &QPushButton::pressed, this, &MainWindow::handleClearButton);
  connect(ui->exitButton, &QPushButton::clicked, qApp, &QCoreApplication::quit, Qt::QueuedConnection);
  connect(ui->decodeView, &CustomGv::paddleEventSignal, this, &MainWindow::handlePaddleEvent);

  connect(&paddle, &GuiInterface::autoKeyEnded, this, &MainWindow::handleAutoKeyEnded, Qt::QueuedConnection);
  connect(&paddle, &GuiInterface::prosignDecoded, ui->decodeView, &CustomGv::receiveString, Qt::QueuedConnection);
  connect(&paddle, &GuiInterface::spaceDecoded, ui->decodeView, &CustomGv::receiveSpace, Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
  paddle.closeAudio();
  delete ui;
}

void MainWindow::setParameters(int paramWpm, int paramFarn, int paramTone)
{
  ui->wpm_edit->setText(QString::number(paramWpm));
  ui->farn_edit->setText(QString::number(paramFarn));
  ui->tone_edit->setText(QString::number(paramTone));

  parametersChecked = false ;
}

void MainWindow::checkParameters()
{
  // Before checking, the text boxes are the parameters
  if (!parametersChecked)
  {
      wpm = ui->wpm_edit->text().toInt();
      farn = ui->farn_edit->text().toInt();
      tone = ui->tone_edit->text().toInt();

      if (wpm < 5)
        wpm = 5;
      else if (wpm > 50)
        wpm = 50;

      if (farn < 5)
        farn = 5;
      else if (farn > wpm)
        farn = wpm ;

      if (tone < 300)
        tone = 300;
      else if (tone > 2000)
        tone=2000;

      ui->wpm_edit->setText(QString::number(wpm));
      ui->farn_edit->setText(QString::number(farn));
      ui->tone_edit->setText(QString::number(tone));

      paddle.setFarn(farn);
      paddle.setWpm(wpm);
      paddle.setTone(tone);
      paddle.generateTones();

      parametersChecked = true ;
  }
}


void MainWindow::handleClearButton()
{
  ui->decodeView->clearScene();
}

void MainWindow::handleWpmEdited()
{
  parametersChecked = false ;
}

void MainWindow::handleFarnEdited()
{
  parametersChecked = false ;
}

void MainWindow::handleToneEdited()
{
  parametersChecked = false ;
}

void MainWindow::handleVolumeChanged()
{
  paddle.setVolume(ui->volume_slider->value()/100.0);
}

void MainWindow::handleSendCharsEdited()
{
  if (ignoreEdits)
    return ;

  checkParameters();

  ignoreEdits = true ;
  QString sendString = ui->send_chars_edit->text();
  if (sendString.length() > 0)
    paddle.autoSendKey(sendString.at(0).toLatin1());
}

void MainWindow::handleAutoKeyEnded()
{
  QString sendString = ui->send_chars_edit->text();
  if (sendString.length() > 0)
    ui->send_chars_edit->setText(sendString.remove(0,1));

  if (sendString.length() > 0)
    paddle.autoSendKey(sendString.at(0).toLatin1());
  else
    ignoreEdits = false ;
}

void MainWindow::handlePaddleEvent(int paddleEvent)
{
  checkParameters();
  paddle.processPaddleEvents(paddleEvent);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
  int scanCode = 0;

  scanCode = event->nativeScanCode();
  if (scanCode == 37)  // left ctrl
  {
    handlePaddleEvent(PADDLE_EVENT_DIT_PRESSED);
  }
  if (scanCode == 105) // right ctrl
  {
    handlePaddleEvent(PADDLE_EVENT_DAH_PRESSED);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
  int scanCode = 0;

  scanCode = event->nativeScanCode();
  if (scanCode == 37)  // left ctrl
  {
    handlePaddleEvent(PADDLE_EVENT_DIT_RELEASED);
  }
  if (scanCode == 105) // right ctrl
  {
    handlePaddleEvent(PADDLE_EVENT_DAH_RELEASED);
  }
}
