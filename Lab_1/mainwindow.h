#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>

#include <QLineEdit>

#include "converting_color.h"
#include <QColorDialog>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  QSignalMapper *mapper;

  Ui::MainWindow *ui;

  Converting_Color *converting_color;


public:

  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void Set_HSV(QString text);
  void Set_RGB(QString text);
  void Set_CMYK(QString text);
  void Set_HSL(QString text);
  void Set_LAB(QString text);
  void Set_XYZ(QString text);


  QString Get_HSV();
  QString Get_RGB();
  QString Get_CMYK();
  QString Get_HSL();
  QString Get_LAB();
  QString Get_XYZ();


private:
  void addActions();
  void addShortcuts();
  void init();



public slots:
  void on_Choose_Color_clicked();
  void Update_Color_HSV(QString HSV);
  void Update_Color_RGB(QString RGB);
  void Update_Color_CMYK(QString CMYK);
  void Update_Color_HSL(QString HSL);
  void Update_Color_XYZ(QString XYZ);
  void Update_Color_LAB(QString LAB);

private slots:

  void Update_Text(QColor color);

};

#endif // MAINWINDOW_H
