#include "dialog.h"
#include "ui_dialog.h"

#include <QString>
#include <QProcess>
#include <QScrollBar>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
  // setup ui
  ui->setupUi(this);

  // connect signal/slots
  connect(ui->makeButton, SIGNAL(pressed()), this, SLOT(make()));
  connect(ui->insmodButton, SIGNAL(pressed()), this, SLOT(insmod()));
  connect(ui->rmmodButton, SIGNAL(pressed()), this, SLOT(rmmod()));
  connect(ui->readButton, SIGNAL(pressed()), this, SLOT(read()));
  connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
}

void Dialog::run(const QString &command)
{
  QProcess *process;
  QTextEdit *outputText;

  process = new QProcess();
  outputText = ui->outputText;

  if(!outputText->document()->isEmpty()) {
    outputText->insertPlainText(QString("\n"));
  }
  outputText->insertPlainText("$ " + command + "\n");

  connect(process, &QProcess::readyReadStandardOutput, [process,outputText]() {
    outputText->insertPlainText(process->readAllStandardOutput());
    QScrollBar *scrollbar = outputText->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
  });

  connect(process, &QProcess::readyReadStandardError, [process,outputText]() {
    outputText->insertPlainText(process->readAllStandardError());
    QScrollBar *scrollbar = outputText->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
  });

  connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [process, outputText](int) {
    outputText->insertPlainText(process->readAllStandardOutput());
    outputText->insertPlainText(process->readAllStandardError());

    QScrollBar *scrollbar = outputText->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());

    delete process;
  });

  process->start(command);
}

void Dialog::make()
{
  run("make all");
}

void Dialog::insmod()
{
  run("insmod falcon.ko");
}

void Dialog::rmmod()
{
  run("rmmod falcon");
}

void Dialog::read()
{
  run("cat /dev/falcon1");
}

void Dialog::clear()
{
  ui->outputText->clear();
}

Dialog::~Dialog()
{
  delete ui;
}
