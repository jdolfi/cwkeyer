#include "mainwindow.h"
#include <QCommandLineParser>
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Key Cw From Mouse");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption cCharSpeed("c", "Character Speed", "...");
  QCommandLineOption fFarnSpeed("f", "Farnsword Speed", "...");
  QCommandLineOption tTone("t", "Tone", "...");

  cCharSpeed.setDefaultValue("20");
  fFarnSpeed.setDefaultValue("20");
  tTone.setDefaultValue("650");

  parser.addOption(cCharSpeed);
  parser.addOption(fFarnSpeed);
  parser.addOption(tTone);
  parser.process(a);

  MainWindow w;
  w.setParameters
  (
    parser.value(cCharSpeed).toInt(),
    parser.value(fFarnSpeed).toInt(),
    parser.value(tTone).toInt()
  );

  int deskWidth = QApplication::desktop()->screenGeometry().width();
  w.setWindowFlags(w.windowFlags() | Qt::FramelessWindowHint);
  w.setGeometry(0,0,deskWidth,w.height());
  w.show();
  return a.exec();
}
