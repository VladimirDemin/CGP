#include "current_color.h"

#include <QPainter>
#include <QDebug>
const QColor defaultBorderColor = QColor(80,80,80,200);

CurrentColor::CurrentColor(QWidget *parent): QWidget(parent)/* : ColorWidget(parent)*/
{
  color.setHsv(0, maxSV, maxSV);
  color_border = defaultBorderColor;
  h = 0;
   s = maxSV;
   v = maxSV;

   color.setHsv(h, s, v);
}

void CurrentColor::paintEvent(QPaintEvent *event)
{
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  p.setPen(Qt::NoPen);
  p.setBrush(color);
  p.drawRect(border, border, width()-2*border, height()-2*border);

  drawBorder(p);
}


void CurrentColor::enterEvent(QEvent *e)
{
  color_border.setRgb(30,30,30,220);
  setCursor(Qt::PointingHandCursor);
  update();
}

void CurrentColor::leaveEvent(QEvent *e)
{
  color_border = defaultBorderColor;
  setCursor(Qt::ArrowCursor);
  update();
}


void CurrentColor::changeColor(QColor color)
{
  this->color = color;
  emit colorChanged(color);
  update();
}

void CurrentColor::reupdateColor_1()
{
    emit colorChanged(color);
}

 void CurrentColor::Choose_Color()
{
   color = QColorDialog::getColor(Qt::black);
   changeColor(color);
}

void CurrentColor::drawBorder(QPainter& p)
{
  QRectF rectangle(0, 0, width(), height());
}
void CurrentColor::updateColor()
{
  h = pointerY-border;
  h = qMax(0, h);
  h = qMin(maxH, h);

  color.setHsv(h, s, v);
  emit hueChanged(color);
}

void CurrentColor::reupdateColor_2()
{
  emit hueChanged(color);
}

void CurrentColor::setInitH(int h)
{
  color.setHsv(h, s, v);
  this->h = color.hue();
  emit hueChanged(color);
  update();
}

void CurrentColor::setH(int h)
{
  if (this->h == 360) this->h = 0;
  if (this->h == h) return;

  color.setHsv(h, s, v);
  setH(color);
}

void CurrentColor::setH(QColor color)
{
  int h = color.hue();
  if (this->h == 360) this->h = 0;
  if (this->h == h)
  {
    return;
  }
  this->h = color.hue();
  emit hueChanged(color);
  update();
}
