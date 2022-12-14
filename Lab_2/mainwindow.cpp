#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QImage>
#include <QtCore>
#include <QImageWriter>
#include <qzipreader_p.h>
#include <QStyledItemDelegate>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(count+1);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя файла"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Сжатие"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Размер"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Разрешение"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Глубина цвета"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
QString arr[]={".jpg",".gif",".bmp",".png",".jpeg"};
QString arch;
bool check = false;
QStringList fileName = QFileDialog ::getOpenFileNames(this,tr("open a file."),"D:/",tr("images(*jpg *gif *bmp *png *jpeg);;archive(*zip)"));


    if (fileName[0].contains("zip"))
    {
        QFileInfo tmp(fileName[0]);
        arch=tmp.path();
        QZipReader zip_reader(fileName[0]);
             if (zip_reader.exists())
             {
                 foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
                 {
                    for (int i=0;i<arr->size();i++)
                    {
                        if(info.filePath.contains(arr[i]))
                        {
                            check=true;
                            break;
                        }
                    }
                    if(check)
                    {
                        break;
                    }
                 }
                 qDebug()<<arch;
                 arch=arch+"/myFolder";
                 QDir dir(arch);
                 if (!dir.exists())
                     dir.mkpath(arch);
                 if(check)
                 {
                 zip_reader.extractAll(arch);
                 QStringList filters;
                    filters << "*.jpg" << "*.gif" << "*.bmp" << "*.png" << "*.jpeg";
                    dir.setNameFilters(filters);
                 QFileInfoList jpgs = dir.entryInfoList();

                 for (int i=0;i<jpgs.size();i++)
                 {
                 QFileInfo fileinfo=jpgs[i];
                 QString path = fileinfo.path();
                 QString str = fileinfo.fileName();
                 QImage myImage;
                 path = path +"/"+ str;
                 myImage.load(path);
                 int size = fileinfo.size();
                 QString res = QVariant(myImage.width()).toString()+ "x" +QVariant(myImage.height()).toString();
                 ui->tableWidget->setColumnCount(5);
                 ui->tableWidget->setRowCount(count+1);
                 ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя файла"));
                  ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Сжатие"));
                 ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Размер"));
                 ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Разрешение"));
                 ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Глубина цвета"));
                 ui->tableWidget->setItem(count,0,new QTableWidgetItem(str));
                 ui->tableWidget->setItem(count,1,new QTableWidgetItem(fileinfo.suffix()));
                 ui->tableWidget->setItem(count,2,new QTableWidgetItem(QVariant(size).toString()));
                 ui->tableWidget->setItem(count,3,new QTableWidgetItem(res));
                 ui->tableWidget->setItem(count,4,new QTableWidgetItem(QVariant(myImage.depth()).toString()));
                 count ++;
                 }
                 }
                dir.removeRecursively();
             }
    }

    else
    {
    for (int i=0;i<fileName.size();i++)
    {
    QFileInfo fileinfo(fileName[i]);
    QString path = fileinfo.path();
    QString str = fileinfo.fileName();
    QImage myImage;
    path = path +"/"+ str;
    myImage.load(path);
    int size = fileinfo.size();
    QString res = QVariant(myImage.width()).toString()+ "x" +QVariant(myImage.height()).toString();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(count+1);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя файла"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Сжатие"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Размер"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Разрешение"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Глубина цвета"));
    ui->tableWidget->setItem(count,0,new QTableWidgetItem(str));
    ui->tableWidget->setItem(count,1,new QTableWidgetItem(fileinfo.suffix()));
    ui->tableWidget->setItem(count,2,new QTableWidgetItem(QVariant(size).toString()));
    ui->tableWidget->setItem(count,3,new QTableWidgetItem(res));
    ui->tableWidget->setItem(count,4,new QTableWidgetItem(QVariant(myImage.depth()).toString()));
    count ++;
    }
    }
}
