#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

  public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

  private:
    Ui::Dialog *ui;

    void run(const QString &command);

  public slots:

      void make();
      void insmod();
      void rmmod();
      void read();
      void clear();
};

#endif // DIALOG_H
