#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    area = new Graph();
    setStyleSheet("background-color: white;");

    QGridLayout *layout = new QGridLayout;

    layout -> addWidget(area, 0, 0, 12, 4);
    layout -> addWidget(ui -> pushButton, 4, 5, 1, 2);
    layout -> addWidget(ui -> pushButton_2, 5, 5, 1, 2);
    layout -> addWidget(ui -> pushButton_3, 6, 5, 1, 2);
    layout -> addWidget(ui -> pushButton_4, 7, 5, 1, 2);
    layout -> addWidget(ui -> scaleButton, 8, 5, 1, 2);
    for (int i = 0; i < 4; ++i)
    {
        layout -> setColumnStretch(i, 10);
    }
    for (int i = 0; i < 12; ++i)
    {
        layout -> setRowStretch(i, 10);
    }
    centralWidget() -> setLayout(layout);
    setMinimumSize(700, 600);
    resize(800, 700);
    setWindowTitle("Растеризация");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete area;
}

void MainWindow::on_scaleButton_clicked()
{
    int length = std::min(width(), height());
    bool ok;
    int nu = QInputDialog::getInt(centralWidget(), "Изменение параметров", "Введите новое значение единичного отрезка", area -> getScale(), 3, length / 10, 1, &ok);
    if (ok)
    {
        area -> SetUnit(nu);
        area -> repaint();
    }
}


void MainWindow::on_pushButton_clicked()
{
    int x1 = QInputDialog::getInt(this, QString::fromUtf8("Введите x1"),
                             QString::fromUtf8("x1:"));
    int y1 = QInputDialog::getInt(this, QString::fromUtf8("Введите y1"),
                             QString::fromUtf8("y1:"));

    int x2 = QInputDialog::getInt(this, QString::fromUtf8("Введите x2"),
                             QString::fromUtf8("x2:"));
    int y2 = QInputDialog::getInt(this, QString::fromUtf8("Введите y2"),
                             QString::fromUtf8("y2:"));
    auto begin = std::chrono::steady_clock::now();

    area -> Clear();
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx == 0 && dy == 0)
    {

         area->AddPixel(x1, y1);
    }
    else
    {
         if (std::abs(dx) > std::abs(dy))
         {
             for(int x = x1; x <= x2; ++x)
             {
                 qreal temp = y1 + dy * (x - x1) / (qreal)dx;
                 area->AddPixel(x, (int)temp);
             }
         }
         else
         {
             if (y1 > y2)
             {
                 std::swap(x1, x2);
                 std::swap(y1, y2);
             }
             for (int y = y1; y <= y2; ++y)
             {
                 qreal temp = dx / (qreal)dy * (y - y1) + x1;

                 area->AddPixel((int)temp, y);
             }
         }
    }
    auto end = std::chrono::steady_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    qDebug() <<"На пошаговый алгоритм потрачено: "<< QString::number(duration) << "ms";
}

void MainWindow::on_pushButton_2_clicked()
{
    int x1 = QInputDialog::getInt(this, QString::fromUtf8("Введите x1"),
                             QString::fromUtf8("x1:"));
    int y1 = QInputDialog::getInt(this, QString::fromUtf8("Введите y1"),
                             QString::fromUtf8("y1:"));

    int x2 = QInputDialog::getInt(this, QString::fromUtf8("Введите x2"),
                             QString::fromUtf8("x2:"));
    int y2 = QInputDialog::getInt(this, QString::fromUtf8("Введите y2"),
                             QString::fromUtf8("y2:"));
    auto begin = std::chrono::steady_clock::now();

    area -> Clear();
    qreal dx = (x2 - x1);
    qreal dy = (y2 - y1);
    int step;
    if (abs(dx) >= abs(dy))
      step = abs(dx);
    else
      step = abs(dy);
    dx = dx / step;
    dy = dy / step;
    qreal x = x1;
    qreal y = y1;
    int i = 0;
    while (i <= step) {
        area->AddPixel(x, y);
        x = x + dx;
        y = y + dy;
        ++i;
    }
    auto end = std::chrono::steady_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    qDebug() <<"На алгоритм ЦДА потрачено: "<< QString::number(duration) << "ms";
}

void MainWindow::on_pushButton_3_clicked()
{
    int x1 = QInputDialog::getInt(this, QString::fromUtf8("Введите x1"),
                             QString::fromUtf8("x1:"));
    int y1 = QInputDialog::getInt(this, QString::fromUtf8("Введите y1"),
                             QString::fromUtf8("y1:"));

    int x2 = QInputDialog::getInt(this, QString::fromUtf8("Введите x2"),
                             QString::fromUtf8("x2:"));
    int y2 = QInputDialog::getInt(this, QString::fromUtf8("Введите y2"),
                             QString::fromUtf8("y2:"));
    auto begin = std::chrono::steady_clock::now();
    area -> Clear();
    int dx = std::abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;

    while (true)
    {
        area->AddPixel(x1, y1);
        if (x1 == x2 && y1 == y2)
        {
            break;
        }
        int e2 = 2 * error;
        if (e2 >= dy)
        {
            if (x1 == x2)
            {
                break;
            }

            error = error + dy;
            x1 = x1 + sx;
        }
        if (e2 <= dx)
        {
            if (y1 == y2)
            {
                break;
            }

            error = error + dx;
            y1 = y1 + sy;
        }
    }
    auto end = std::chrono::steady_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    qDebug() <<"На алгоритм Брезенхема потрачено: "<< QString::number(duration) << "ms";
}

void MainWindow::on_pushButton_4_clicked()
{
    int x0 = QInputDialog::getInt(this, QString::fromUtf8("Введите x0"),
                             QString::fromUtf8("x0:"));
    int y0 = QInputDialog::getInt(this, QString::fromUtf8("Введите y0"),
                             QString::fromUtf8("y0:"));
    int radius = QInputDialog::getInt(this, QString::fromUtf8("Введите r"),
                             QString::fromUtf8("r:"));
    auto begin = std::chrono::steady_clock::now();
    area -> Clear();
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while (x >= y)
    {
        area->AddPixel(x + x0, y + y0);
        area->AddPixel(y + x0, x + y0);
        area->AddPixel(-x + x0, y + y0);
        area->AddPixel(-y + x0, x + y0);
        area->AddPixel(-x + x0, -y + y0);
        area->AddPixel(-y + x0, -x + y0);
        area->AddPixel(x + x0, -y + y0);
        area->AddPixel(y + x0, -x + y0);
        ++y;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            --x;
            radiusError += 2 * (y - x + 1);
        }
    }
    auto end = std::chrono::steady_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    qDebug() <<"На алгоритм Брезенхема(окружность) потрачено: "<< QString::number(duration) << "ms";
}
