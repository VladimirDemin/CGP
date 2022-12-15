#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btnClear_clicked();

    void on_btnBackgroundColor_clicked();

    void on_btnFigureColor_clicked();

    void on_btnEdgeColor_clicked();

    void on_btnClipColor_clicked();

private:
    Ui::MainWindow *ui;
    PlotArea *area;
    QString segmentsPath = "segments.txt";
    QString polygonPath = "polygon.txt";
    QColor color_back = Qt::white;
    QColor color_seg = Qt::black;
    QColor color_clip = Qt::red;
    QColor color_figure = Qt::black;
    void ClipAfterLine(std::vector<QPointF>& polygon, int cord, int bit);
    int getCode(qreal x, qreal y) const;
    int getCode(const QPointF& p) const;
};
#endif // MAINWINDOW_H
