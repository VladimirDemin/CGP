#include  "mainwindow.h"
#include  "ui_mainwindow.h"

#include <QPainter>
#include <QShortcut>
#include <QScreen>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
 QMainWindow(parent),
 ui(new Ui::MainWindow)
{
 ui->setupUi (this);
  converting_color = new Converting_Color(ui->Current_Color);
  addActions();
  addShortcuts();
  init();

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::init()
{
  QApplication::setStyle("fusion");

  ui->HSV->setType(Converting_Color::HSV);
  ui->RGB->setType(Converting_Color::RGB);
  ui->CMYK->setType(Converting_Color::CMYK);
  ui->HSL->setType(Converting_Color::HSL);
  ui->XYZ->setType(Converting_Color::XYZ);
  ui->LAB->setType(Converting_Color::LAB);


  ui->HSV->setType(Converting_Color::HSV);
  ui->RGB->setType(Converting_Color::RGB);
  ui->CMYK->setType(Converting_Color::CMYK);
  ui->HSL->setType(Converting_Color::HSL);
  ui->XYZ->setType(Converting_Color::XYZ);
  ui->LAB->setType(Converting_Color::LAB);


  ui->Current_Color->setInitH(0);
}

void MainWindow::addActions()
{
  connect( ui->Current_Color, SIGNAL(hueChanged(QColor)), converting_color, SLOT(changeHue(QColor)) );
  connect( converting_color, SIGNAL(colorChanged(QColor)), ui->Current_Color, SLOT(changeColor(QColor)) );
  connect( ui->Current_Color, SIGNAL(colorChanged(QColor)), this, SLOT(Update_Text(QColor)) );
   connect( converting_color, SIGNAL(colorChanged(QColor)), this, SLOT(Update_Text(QColor)) );

  connect( ui->HSV,  SIGNAL(textEdited(QString)), this, SLOT(Update_Color_HSV(QString)) );
  connect( ui->RGB,  SIGNAL(textEdited(QString)), this, SLOT(Update_Color_RGB(QString)) );
  connect( ui->CMYK, SIGNAL(textEdited(QString)), this, SLOT(Update_Color_CMYK(QString)) );
  connect( ui->HSL,  SIGNAL(textEdited(QString)), this, SLOT(Update_Color_HSL(QString)) );
  connect( ui->XYZ,  SIGNAL(textEdited(QString)), this, SLOT(Update_Color_XYZ(QString)) );
  connect( ui->LAB, SIGNAL(textEdited(QString)), this, SLOT(Update_Color_LAB(QString)) );
}
void MainWindow::addShortcuts()
{
     QFont f_RGB( "Arial", 10, QFont::Bold);
     ui->label_RGB->setFont(f_RGB);
     ui->label_RGB->setText("RGB");

     QFont f_HSV( "Arial", 10, QFont::Bold);
     ui->label_HSV->setFont(f_HSV);
     ui->label_HSV->setText("HSV");

     QFont f_HSL( "Arial", 10, QFont::Bold);
     ui->label_HSL->setFont(f_HSL);
     ui->label_HSL->setText("HSL");

     QFont f_LAB( "Arial", 10, QFont::Bold);
     ui->label_LAB->setFont(f_LAB);
     ui->label_LAB->setText("LAB");

     QFont f_XYZ( "Arial", 10, QFont::Bold);
     ui->label_XYZ->setFont(f_XYZ);
     ui->label_XYZ->setText("XYZ");

     QFont f_CMYK( "Arial", 10, QFont::Bold);
     ui->label_CMYK->setFont(f_CMYK);
     ui->label_CMYK->setText("CMYK");
}
void MainWindow::Update_Color_HSV(QString text)
{
//  editingField = "HSV";
  text = Converting_Color::Fix_Text(text);
  ui->HSV->updateText(text);
  converting_color->Update_HSV(text);
}

void MainWindow::Update_Color_RGB(QString text)
{
  text = Converting_Color::Fix_Text(text);
  ui->RGB->updateText(text);
  converting_color->Update_RGB(text);
}

void MainWindow::Update_Color_CMYK(QString text)
{
  text = Converting_Color::Fix_Text(text);
  ui->CMYK->updateText(text);
  converting_color->Update_CMYK(text);
}
void MainWindow::Update_Color_HSL(QString text)
{
  text = Converting_Color::Fix_Text(text);
  ui->HSL->updateText(text);
  converting_color->Update_HSL(text);
}

void MainWindow::Update_Color_XYZ(QString text)
{
  text = Converting_Color::Fix_Text(text);
  ui->XYZ->updateText(text);
  converting_color->Update_XYZ(text);
}

void MainWindow::Update_Color_LAB(QString text)
{
  text = Converting_Color::Fix_Text(text);
  ui->LAB->updateText(text);
  converting_color->Update_LAB(text);
}

void MainWindow::Update_Text(QColor color)
{
  QString HSV, RGB, CMYK, HSL, XYZ, LAB, Hex;
  HSV = converting_color->get_HSV(color);
  RGB = converting_color->get_RGB(color);
  CMYK = converting_color->get_CMYK(color);
  HSL = converting_color->get_HSL(color);
  XYZ = converting_color->get_XYZ(color);
  LAB = converting_color->get_LAB(color);
  Set_HSV(HSV);
  Set_RGB(RGB);
  Set_CMYK(CMYK);
  Set_HSL(HSL);
  Set_XYZ(XYZ);
  Set_LAB(LAB);

//    if(!Converting_Color::check_LAB(LAB))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели LAB."));
//    }

//    if(!Converting_Color::check_HSV(HSV))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели HSV."));
//    }

//    if(!Converting_Color::check_HSL(HSL))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели HSL."));
//    }

//    if(!Converting_Color::check_XYZ(XYZ))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели XYZ."));
//    }

//    if(!Converting_Color::check_RGB(RGB))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели RGB."));
//    }

//    if(!Converting_Color::check_CMYK(CMYK))
//    {
//        QMessageBox::information(this, "", tr("Произошло округление значений цветовой модели CMYK."));
//    }

//  if(!Converting_Color::checkValueCMYK(CMYK)||!Converting_Color::checkValueHSV(HSV)||!Converting_Color::checkValueHSL(HSL)||!Converting_Color::checkValueLAB(LAB)||!Converting_Color::checkValueXYZ(XYZ)||!Converting_Color::checkValueRGB(RGB))
//  {
//      QMessageBox::information(this, "", tr("Произошло округление значений."));
//  }
}

void MainWindow::Set_HSV(QString text)
{
  ui->HSV->setText(text);
}

void MainWindow::Set_RGB(QString text)
{
  ui->RGB->setText(text);
}

void MainWindow::Set_CMYK(QString text)
{
  ui->CMYK->setText(text);
}

void MainWindow::Set_HSL(QString text)
{
    ui->HSL->setText(text);
}

void MainWindow::Set_LAB(QString text)
{
    ui->LAB->setText(text);
}

void MainWindow::Set_XYZ(QString text)
{
    ui->XYZ->setText(text);
}

QString MainWindow::Get_HSV()
{
  return ui->HSV->text();
}

QString MainWindow::Get_RGB()
{
  return ui->RGB->text();
}

QString MainWindow::Get_CMYK()
{
    return ui->CMYK->text();
}

QString MainWindow::Get_HSL()
{
    return ui->HSL->text();
}

QString MainWindow::Get_LAB()
{
    return ui->LAB->text();
}

QString MainWindow::Get_XYZ()
{
    return ui->XYZ->text();
}

void MainWindow::on_Choose_Color_clicked()
{
   ui->Current_Color->Choose_Color();
}


