#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QPainter>
#include <QWidget>
#include <vector>

class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);
    void AddPixel(int x, int y, qreal percent = 1);
    void Clear();
    void SetUnit(int nu);
    int getScale() const;
private:
    int u; // unit size
    int tick_length = 4;
    int grid_line_width = 1;
    int axis_width = 2;
    int box_offset = 1;
    int box_width = 1;
    int pixel_width = 1;
    int zx = 0;
    int zy = 0;
    QColor axisColor = Qt::black;
    QColor gridColor = Qt::gray;
    QColor boxColor = Qt::gray;
    int max_grey_value = 200;
    void inline drawBox(QPainter(&p));
    void inline drawGrid(QPainter& p);
    void inline drawAxis(QPainter& p);
    void inline drawTicks(QPainter& p);
    void inline drawArrows(QPainter& p);
    void inline drawPixels(QPainter& p);
    std::vector<std::pair<std::pair<int,int>, qreal>> pixels;
    void paintEvent(QPaintEvent* event) override;
};

#endif // PLOTAREA_H
