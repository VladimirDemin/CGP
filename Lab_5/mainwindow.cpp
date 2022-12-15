#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <fstream>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QValidator>
#include <chrono>
#include <cmath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString qss = QString("background-color: %1").arg(color_back.name());
    setStyleSheet(qss);
    QGridLayout *g = new QGridLayout(centralWidget());
    area = new PlotArea();
    g -> addWidget(area, 0, 0, 10, 4);
    g -> addWidget(ui -> pushButton,  0, 4, 1, 1);
    g -> addWidget(ui -> pushButton_2,  1, 4, 1, 1);
    centralWidget()->setLayout(g);
    setMinimumSize({600, 600});
    setWindowTitle("Отсечения");
}

int MainWindow::getCode(qreal x, qreal y) const
{
    int ans = 0;
    QPoint p1 = area -> getClippingWindowP1();
    QPoint p2 = area -> getClippingWindowP2();
    if (y > p1.y())
    {
        ans |= 8;
    }
    if (y < p2.y())
    {
        ans |= 4;
    }
    if (x > p2.x())
    {
        ans |= 2;
    }
    if (x < p1.x())
    {
        ans |= 1;
    }
    return ans;
}

int MainWindow::getCode(const QPointF& p) const
{
    return getCode(p.x(), p.y());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ClipAfterLine(std::vector<QPointF>& points, int coord, int bit)
{
    auto f = [&bit](int code)
    {
        return (code & (1 << bit)) == 0;
    };
    auto intersect = [coord, bit](const QPointF& p1, const QPointF& p2)
    {
        qreal x1 = p1.x();
        qreal y1 = p1.y();
        qreal x2 = p2.x();
        qreal y2 = p2.y();
        qreal A = y2 - y1;
        qreal B = x1 - x2;
        qreal C = -A * x1 - B * y1;
        if (bit < 2)
        {
            //intersect with x = coord


            qreal y = (-C - A * coord) / B;
            return QPointF(coord, y);
        }
        else
        {
            qreal x = (-C - B * coord) / A;
            return QPointF(x, coord);
        }
    };
    size_t i = 1;
    std::vector<QPointF> ans;
    QPointF lastPoint = points[0];
    int lastCode = getCode(lastPoint);
    while(i <= points.size())
    {
        int newCode = getCode(points[i % points.size()]);
        if (f(lastCode) && f(newCode))
        {
            ans.push_back(lastPoint);
            //++i;
        }
        else if (f(lastCode) && !f(newCode))
        {
            ans.push_back(lastPoint);
            ans.push_back(intersect(lastPoint, points[i % points.size()]));
            //++i;
        }
        else if (!f(lastCode) && f(newCode))
        {
            ans.push_back(intersect(lastPoint, points[i % points.size()]));
            //++i;
        }
        lastCode = newCode;
        lastPoint = points[i];
        ++i;
    }
    points.swap(ans);
}



void MainWindow::on_pushButton_clicked()
{
    area -> Clear();
    area -> ChangeMode(PlotMode::Segments);
    std::ifstream fin(segmentsPath.toStdString());
    if (!fin)
    {
        QMessageBox::warning(nullptr, "Ошибка", "Файл input_1.txt в папке исполняемого файла не найден");
        return;
    }
    int n;
    fin >> n;
    qreal x1, x2, y1, y2;
    std::vector<std::pair<QPointF, QPointF>> data;
    for (int i = 0; i < n; ++i)
    {
        fin >> x1 >> y1 >> x2 >> y2;
        data.push_back({QPointF(x1, y1), QPointF(x2, y2)});
    }
    fin >> x1 >> y1 >> x2 >> y2;
    area -> SetClippingWindow(QPoint(x1, y1), QPoint(x2, y2));
    fin.close();

    for (int i = 0; i < n; ++i)
    {
        x1 = data[i].first.x();
        y1 = data[i].first.y();
        x2 = data[i].second.x();
        y2 = data[i].second.y();
        int code1 = getCode(x1, y1);
        int code2 = getCode(x2, y2);
        if (code1 == 0 && code2 == 0)
        {
            //inside
            area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(x2, y2), color_clip});
        }
        else if ((code1 & code2) != 0)
        {
            //outside
            area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(x2, y2), color_seg});
        }
        else
        {
            qreal A = y2 - y1;
            qreal B = x1 - x2;
            qreal C = -A * x1 - B * y1;
            qreal xmin = area -> getClippingWindowP1().x();
            qreal ymax = area -> getClippingWindowP1().y();
            qreal xmax = area -> getClippingWindowP2().x();
            qreal ymin = area -> getClippingWindowP2().y();
            while(code1 != 0 || code2 != 0)
            {
                if (code1 == 0)
                {
                    std::swap(x1, x2);
                    std::swap(y1, y2);
                    std::swap(code1, code2);
                }
                if (code1 & 1)
                {
                    //leftside xmin
                    qreal y = (-C - A * xmin) / B;
                    area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(xmin, y), color_seg});
                    x1 = xmin;
                    y1 = y;
                }
                else if (code1 & 2)
                {
                    //rightside xmax
                    qreal y = (-C - A * xmax) / B;
                    area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(xmax, y), color_seg});
                    x1 = xmax;
                    y1 = y;

                }
                else if (code1 & 4)
                {
                    //bottomside ymin
                    qreal x = (-C - B * ymin) / A;
                    area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(x, ymin), color_seg});
                    x1 = x;
                    y1 = ymin;
                }
                else
                {
                    //topside ymax
                    qreal x = (-C - B * ymax) / A;
                    area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(x, ymax), color_seg});
                    x1 = x;
                    y1 = ymax;
                }
                code1 = getCode(x1, y1);
            }
            area -> AddLineSegment(LineSegmentData{QPointF(x1, y1), QPointF(x2, y2), color_clip});
        }
    }
    area -> repaint();
}

void MainWindow::on_pushButton_2_clicked()
{

    area -> Clear();
    area -> ChangeMode(PlotMode::Polygons);

    std::ifstream fin(polygonPath.toStdString());
    if (!fin)
    {
        QMessageBox::warning(nullptr, "Ошибка", "Файл input_2.txt в папке исполняемого файла не найден");
        return;
    }
    int n;
    fin >> n;
    int x1, y1;
    std::vector<QPointF> points;
    for (int i = 0; i < n; ++i)
    {
        fin >> x1 >> y1;
        points.push_back(QPointF(x1, y1));
    }
    int x2, y2;
    fin >> x1 >> y1 >> x2 >> y2;
    area -> AddPolygon(points, color_figure);
    area -> SetClippingWindow({x1, y1}, {x2, y2});
    qreal xmin = area -> getClippingWindowP1().x();
    qreal ymax = area -> getClippingWindowP1().y();
    qreal xmax = area -> getClippingWindowP2().x();
    qreal ymin = area -> getClippingWindowP2().y();
    ClipAfterLine(points, ymin, 2);
    ClipAfterLine(points, ymax, 3);
    ClipAfterLine(points, xmin, 0);
    ClipAfterLine(points, xmax, 1);
    area -> AddPolygon(points, color_clip);

    fin.close();
    area -> repaint();
}

void MainWindow::on_btnClear_clicked()
{
    area -> Clear();
     area -> ChangeMode(PlotMode::None);
     area -> repaint();
}


void MainWindow::on_btnBackgroundColor_clicked()
{
  color_back = QColorDialog::getColor(Qt::green, this);
  QString qss = QString("background-color: %1").arg(color_back.name());
  setStyleSheet(qss);
}

void MainWindow::on_btnFigureColor_clicked()
{
    color_figure = QColorDialog::getColor(Qt::green, this);
    area -> repaint();
}

void MainWindow::on_btnEdgeColor_clicked()
{
  color_seg = QColorDialog::getColor(Qt::green, this);
  area -> repaint();
}

void MainWindow::on_btnClipColor_clicked()
{
  color_clip = QColorDialog::getColor(Qt::green, this);
  area -> repaint();
}
