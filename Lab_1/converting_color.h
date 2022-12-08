#ifndef COLORPROCESSOR_H
#define COLORPROCESSOR_H

#include <QtCore>
#include <QColor>

#include <QObject>
#include <QClipboard>

#include "current_color.h"

class Converting_Color : public QWidget
{
  Q_OBJECT

public:
  explicit Converting_Color(CurrentColor* current_color, QWidget* parent=0);
    enum Type_of_Color
    {
      HSV, RGB, CMYK, Hex, HSL, XYZ, LAB
    };
     QColor color;

  static bool checkColorText(QString text, Type_of_Color colorType=HSV);

  static bool check_HSV(QString text);
  static bool check_RGB(QString text);
  static bool check_CMYK(QString text);
  static bool check_HSL(QString text);
  static bool check_LAB(QString text);
  static bool check_XYZ(QString text);

  static void Fix_HSV(QColor& color);
  static void Fix_RGB(QColor& color);
  static void Fix_CMYK(QColor& color);
  static void Fix_HSL(QColor& color);
  static void Fix_LAB(QColor& color);
  static void Fix_XYZ(QColor& color);

  static QString Fix_Text(QString text);

  static bool check_comp_value(int val, Type_of_Color colorType=HSV, int group=0, int hexLen=0);
  static bool check_by_type(QString text, Type_of_Color colorType=HSV);

  QString get_HSV(QColor color);
  QString get_RGB(QColor color);
  QString get_CMYK(QColor color);
  QString get_HSL(QColor color);
  QString get_LAB(QColor color);
  QString get_XYZ(QColor color);


  QColor Get_Color_HSV(QString HSV_Text);
  QColor Get_Color_RGB(QString RGB_Text);
  QColor Get_Color_CMYK(QString CMYK_Text);
  QColor Get_Color_HSL(QString HSL_Text);
  QColor Get_Color_LAB(QString LAB_Text);
  QColor Get_Color_XYZ(QString XYZ_Text);


int h,s,v;

  void Update_HSV(QString HSV);
  void Update_RGB(QString RGB);
  void Update_CMYK(QString CMYK);
  void Update_HSL(QString HSL);
  void Update_LAB(QString LAB);
  void Update_XYZ(QString XYZ);

  void Update_Color(QColor color);
  void Update_Color();

  void setS(QColor color);
  void setV(QColor color);
  void setSV(int s, int v);

  void reupdateColor();

signals:
  void updateFinished();
  void colorChanged(QColor);
  void saturationChanged(QColor);
  void valueChanged(QColor);

public slots:
  void changeHue(QColor color);

private:
  CurrentColor* current_color;
  QClipboard* clip;
  static int getSizeByType(Type_of_Color colorType);

};

#endif // COLORPROCESSOR_H
