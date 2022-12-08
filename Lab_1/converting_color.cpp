#include "converting_color.h"

#include <QApplication>
#include <QtCore>
const int maxH_1 = 359;
const int maxS_1 = 255;
const int maxV_1 = 255;

const int maxRGB = 255;

const int maxCMYK = 255;

const int maxL = 100;
const int maxA = 128;
const int maxB = 128;

const int maxH_2 = 360;
const int maxS_2 = 100;
const int maxL_2 = 100;

const int maxX = 95;
const int maxY = 100;
const int maxZ = 109;


bool Converting_Color::checkColorText(QString text, Converting_Color::Type_of_Color colorType)
{
  int size = getSizeByType(colorType);

  QStringList list;
  list = text.split(" ");
  if (list.size()!=size) return false;

  foreach(QString comp, list)
  {
    if (!comp.length())
      return false;
  }

  foreach(QString comp, list)
  {
    bool ok;
    comp.toInt(&ok);
    if (!ok) return false;
  }

  return true;
}

bool Converting_Color::check_by_type(QString text, Converting_Color::Type_of_Color colorType) {
  switch (colorType)
  {
    case Converting_Color::HSV:
      return check_HSV(text);
    case Converting_Color::RGB:
      return check_RGB(text);
    case Converting_Color::CMYK:
      return check_CMYK(text);
  }
  return false;
}

bool Converting_Color::check_HSV(QString text)
{
  QStringList list;
  list = text.split(" ");

  int h = list[0].toInt();
  int s = list[1].toInt();
  int v = list[2].toInt();

  if (h<0 || h>maxH_1) return false;
  if (s<0 || s>maxH_1) return false;
  if (v<0 || v>maxH_1) return false;

  return true;
}

bool Converting_Color::check_RGB(QString text)
{
  QStringList list;
  list = text.split(" ");

  int r = list[0].toInt();
  int g = list[1].toInt();
  int b = list[2].toInt();

  if (r<0 || r>maxRGB) return false;
  if (g<0 || g>maxRGB) return false;
  if (b<0 || b>maxRGB) return false;

  return true;
}

bool Converting_Color::check_CMYK(QString text)
{
  QStringList list;
  list = text.split(" ");

  int c = list[0].toInt();
  int m = list[1].toInt();
  int y = list[2].toInt();
  int k = list[3].toInt();

  if (c<0 || c>maxCMYK) return false;
  if (m<0 || m>maxCMYK) return false;
  if (y<0 || y>maxCMYK) return false;
  if (k<0 || k>maxCMYK) return false;

  return true;
}



bool Converting_Color::check_HSL(QString text)
{
    QStringList list;
    list = text.split(" ");

    int h = list[0].toInt();
    int s = list[1].toInt();
    int l = list[2].toInt();

    if (h<0 || h>maxH_2) return false;
    if (s<0 || s>maxS_2) return false;
    if (l<0 || l>maxL_2) return false;

    return true;
}

bool Converting_Color::check_LAB(QString text)
{
    QStringList list;
    list = text.split(" ");

    int l = list[0].toInt();
    int a = list[1].toInt();
    int b = list[2].toInt();

    if (l<0 || l>maxL)
    {
        return false;
    }
    if (a<0 || a>maxA)
    {

        return false;
    }
    if (b<0 || b>maxB) return false;

    return true;
}

bool Converting_Color::check_XYZ(QString text)
{
    QStringList list;
    list = text.split(" ");

    int x = list[0].toInt();
    int y = list[1].toInt();
    int z = list[2].toInt();

    if (x<0 || x>maxX) return false;
    if (y<0 || y>maxY) return false;
    if (z<0 || z>maxZ) return false;

    return true;
}

bool Converting_Color::check_comp_value(int val, Converting_Color::Type_of_Color colorType, int group, int hexLen) {
  if (colorType == Converting_Color::HSV)
  {
    switch (group)
    {
    case 0:
      return (val >= 0 && val <= maxH_1);
    case 1:
      return (val >= 0 && val <= maxS_1);
    case 2:
      return (val >= 0 && val <= maxV_1);
    }
  }

  if (colorType == Converting_Color::RGB)
  {
    return (val >= 0 && val <= maxRGB);
  }

  if (colorType == Converting_Color::CMYK)
  {
    switch (group)
    {
    case 0:
      return (val >= 0 && val <= maxCMYK);
    case 1:
      return (val >= 0 && val <= maxCMYK);
    case 2:
      return (val >= 0 && val <= maxCMYK);
    case 3:
      return (val >= 0 && val <= maxCMYK);
    }
  }

  if (colorType == Converting_Color::HSL)
  {
    switch (group)
    {
    case 0:
      return (val >= 0 && val <= maxH_2);
    case 1:
      return (val >= 0 && val <= maxS_2);
    case 2:
      return (val >= 0 && val <= maxL_2);
    }
  }

  if (colorType == Converting_Color::LAB)
  {
    switch (group)
    {
    case 0:
      return (val >= 0 && val <= maxL);
    case 1:
      return (val >= 0 && val <= maxA);
    case 2:
      return (val >= 0 && val <= maxB);
    }
  }

  if (colorType == Converting_Color::XYZ)
  {
    switch (group)
    {
    case 0:
      return (val >= 0 && val <= maxX);
    case 1:
      return (val >= 0 && val <= maxY);
    case 2:
      return (val >= 0 && val <= maxZ);
    }
  }

  return false;
}


void Converting_Color::Fix_HSV(QColor& color)
{
  int h = color.hue();
  int s = color.saturation();
  int v = color.value();

  h = qMin(maxH_1, qMax(0, h));
  s = qMin(maxS_1, qMax(0, s));
  v = qMin(maxV_1, qMax(0, v));

  color.setHsv(h, s, v);
}

void Converting_Color::Fix_RGB(QColor& color)
{
  int r = color.red();
  int g = color.green();
  int b = color.blue();

  r = qMin(maxRGB, qMax(0, r));
  g = qMin(maxRGB, qMax(0, g));
  b = qMin(maxRGB, qMax(0, b));

  color.setRgb(r, g, b);
}

void Converting_Color::Fix_CMYK(QColor& color)
{
  int c = color.cyan();
  int m = color.magenta();
  int y = color.yellow();
  int k = color.black();

  c = qMin(maxCMYK, qMax(0, c));
  m = qMin(maxCMYK, qMax(0, m));
  y = qMin(maxCMYK, qMax(0, y));
  k = qMin(maxCMYK, qMax(0, k));

  color.setCmyk(c, m, y, k);
}

void Converting_Color::Fix_HSL(QColor &color)
{
    int h = color.hue();
    int s = color.saturation();
    int l = color.lightness();

    h = qMin(maxH_1, qMax(0, h));
    s = qMin(maxS_1, qMax(0, s));
    l = qMin(maxV_1, qMax(0, l));

    color.setHsl(h, s, l);
}

void Converting_Color::Fix_LAB(QColor &color)
{
    int l = color.lightness();
    int a = color.red();
    int b = color.blue();

    l = qMin(maxL, qMax(0, l));
    a = qMin(maxA, qMax(0, a));
    b = qMin(maxB, qMax(0, b));

    color.setHsl(l, a, b);
}

void Converting_Color::Fix_XYZ(QColor &color)
{
    int x = color.red();
    int y = color.green();
    int z = color.blue();

    x = qMin(maxX, qMax(0, x));
    y = qMin(maxY, qMax(0, y));
    z = qMin(maxZ, qMax(0, z));

    color.setHsl(x, y, z);
}


QString Converting_Color::Fix_Text(QString text)
{
  if (!text.length()) return text;
  return text.trimmed().replace(",", " ").replace(QRegExp("\\s+"), " ");
}

int Converting_Color::getSizeByType(Converting_Color::Type_of_Color colorType)
{
  switch (colorType)
  {
    case Converting_Color::HSV:
      return 3;
    case Converting_Color::RGB:
      return 3;
    case Converting_Color::CMYK:
      return 4;
  case Converting_Color::HSL:
    return 3;
  case Converting_Color::LAB:
    return 3;
  case Converting_Color::XYZ:
    return 3;
    default:
      return 0;
  }
}


Converting_Color::Converting_Color(CurrentColor* Current_color, QWidget* parent) : QWidget(parent)
{
  this->current_color = Current_color;
  clip = QApplication::clipboard();
}

QString Converting_Color::get_HSV(QColor color)
{
  QString res;
  Converting_Color::Fix_HSV(color);

  int h = color.hue();
  int s = color.saturation();
  int v = color.value();

  res = QString("%1 %2 %3").arg(h).arg(s).arg(v);
  return res;
}

QString Converting_Color::get_RGB(QColor color)
{
  QString res;

  Converting_Color::Fix_RGB(color);

  int r = color.red();
  int g = color.green();
  int b = color.blue();

  res = QString("%1 %2 %3").arg(r).arg(g).arg(b);
  return res;
}

QString Converting_Color::get_CMYK(QColor color)
{
  QString res;
  Converting_Color::Fix_CMYK(color);

  int c = color.cyan();
  int m = color.magenta();
  int y = color.yellow();
  int k = color.black();

  res = QString("%1 %2 %3 %4").arg(c).arg(m).arg(y).arg(k);
  return res;
}

QString Converting_Color::get_HSL(QColor color)
{
    QString res;
    Converting_Color::Fix_HSL(color);

    int h = color.hue();
    int s = color.saturation();
    int l = color.lightness();

    res = QString("%1 %2 %3").arg(h).arg(s).arg(l);
    return res;
}

QString Converting_Color::get_LAB(QColor color)
{
    QString res;
    Converting_Color::Fix_LAB(color);

    int l = color.lightness();
    int a = color.red();
    int b = color.blue();

    res = QString("%1 %2 %3").arg(l).arg(a).arg(b);
    return res;
}

QString Converting_Color::get_XYZ(QColor color)
{
    QString res;
    Converting_Color::Fix_HSV(color);

    int x = color.red();
    int y = color.green();
    int z = color.blue();

    res = QString("%1 %2 %3").arg(x).arg(y).arg(z);
    return res;
}

QColor Converting_Color::Get_Color_HSV(QString HSV_Text)
{
  QStringList list;
  QColor color;

  list = HSV_Text.split(" ");
  int h = list[0].toInt();
  int s = list[1].toInt();
  int v = list[2].toInt();

  color.setHsv(h, s, v);

  return color;
}

QColor Converting_Color::Get_Color_RGB(QString RGB_Text)
{
  QStringList list;
  QColor color;

  list = RGB_Text.split(" ");
  int r = list[0].toInt();
  int g = list[1].toInt();
  int b = list[2].toInt();

  color.setRgb(r, g, b);

  return color;
}

QColor Converting_Color::Get_Color_CMYK(QString CMYK_Text)
{
  QStringList list;
  QColor color;

  list = CMYK_Text.split(" ");
  int c = list[0].toInt();
  int m = list[1].toInt();
  int y = list[2].toInt();
  int k = list[3].toInt();

  color.setCmyk(c, m, y, k);

  return color;
}

QColor Converting_Color::Get_Color_HSL(QString HSL_Text)
{
    QStringList list;
    QColor color;

    list = HSL_Text.split(" ");
    int h = list[0].toInt();
    int s = list[1].toInt();
    int l = list[2].toInt();

    color.setHsv(h, s, l);

    return color;
}

QColor Converting_Color::Get_Color_LAB(QString LAB_Text)
{
    QStringList list;
    QColor color;

    list = LAB_Text.split(" ");
    int l = list[0].toInt();
    int a = list[1].toInt();
    int b = list[2].toInt();

    color.setHsv(l, a, b);

    return color;
}

QColor Converting_Color::Get_Color_XYZ(QString XYZ_Text)
{
    QStringList list;
    QColor color;

    list = XYZ_Text.split(" ");
    int x = list[0].toInt();
    int y = list[1].toInt();
    int z = list[2].toInt();

    color.setHsv(x, y, z);

    return color;
}

void Converting_Color::Update_Color()
{
  color.setHsv(h, s, v);

  emit colorChanged(color);
  emit saturationChanged(color);
  emit valueChanged(color);
}

void Converting_Color::reupdateColor()
{
  emit saturationChanged(color);
  emit valueChanged(color);
}

void Converting_Color::changeHue(QColor color)
{
  h = color.hue();

  QColor newColor;
  newColor.setHsv(h, s, v);
  emit colorChanged(newColor);

  update();
}

void Converting_Color::setS(QColor color)
{
  s = color.saturation();

  update();

  Update_Color();
}

void Converting_Color::setV(QColor color)
{
  v = color.value();

  update();

  Update_Color();
}

void Converting_Color::setSV(int s, int v)
{
  this->s = s;
  this->v = v;

  update();

  Update_Color();
}

void Converting_Color::Update_HSV(QString HSV_Text)
{
  if ( !Converting_Color::checkColorText(HSV_Text, Converting_Color::HSV) ) return;
  if ( !Converting_Color::check_HSV(HSV_Text) ) return;

  QColor color = Get_Color_HSV(HSV_Text);
  Update_Color(color);
}

void Converting_Color::Update_RGB(QString RGB_Text)
{
  if ( !Converting_Color::checkColorText(RGB_Text, Converting_Color::RGB) ) return;
  if ( !Converting_Color::check_RGB(RGB_Text) ) return;

  QColor color = Get_Color_RGB(RGB_Text);
  Update_Color(color);
}

void Converting_Color::Update_CMYK(QString CMYK_Text)
{
  if ( !Converting_Color::checkColorText(CMYK_Text, Converting_Color::CMYK) ) return;
  if ( !Converting_Color::check_CMYK(CMYK_Text) ) return;

  QColor color = Get_Color_CMYK(CMYK_Text);
  Update_Color(color);
}


void Converting_Color::Update_HSL(QString HSL_Text)
{
    if ( !Converting_Color::checkColorText(HSL_Text, Converting_Color::HSL) ) return;
    if ( !Converting_Color::check_HSV(HSL_Text) ) return;

    QColor color = Get_Color_HSL(HSL_Text);
    Update_Color(color);
}

void Converting_Color::Update_LAB(QString LAB_Text)
{
    if ( !Converting_Color::checkColorText(LAB_Text, Converting_Color::LAB) ) return;
    if ( !Converting_Color::check_LAB(LAB_Text) ) return;

    QColor color = Get_Color_LAB(LAB_Text);
    Update_Color(color);
}

void Converting_Color::Update_XYZ(QString XYZ_Text)
{
    if ( !Converting_Color::checkColorText(XYZ_Text, Converting_Color::XYZ) ) return;
    if ( !Converting_Color::check_XYZ(XYZ_Text) ) return;

    QColor color = Get_Color_XYZ(XYZ_Text);
    Update_Color(color);
}

void Converting_Color::Update_Color(QColor color)
{
  int h = color.hue();
  int s = color.saturation();
  int v = color.value();

  current_color->setH(h);
  setSV(s, v);

  emit updateFinished();
}
