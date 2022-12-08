#include "text.h"

#include <QDebug>

#include "check_values.h"

ColorText::ColorText(QWidget *parent) : QLineEdit(parent)
{
  colorType = Converting_Color::HSV;
  separator = " ";
}

void ColorText::setType(Converting_Color::Type_of_Color colorType)
{
  this->colorType = colorType;
}


void ColorText::updateText(QString text)
{
  int pos = cursorPosition();
  setText(text);
  setCursorPosition(pos);
}




