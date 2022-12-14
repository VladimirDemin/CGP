#include "graph.h"
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>

Graph::Graph(QWidget *parent):QWidget(parent)
{
    u = std::min(width(), height()) / 20;
}

void Graph::drawBox(QPainter& p)
{
    int h = height() - 2 * box_offset;
    int w = width() - 2 * box_offset;
    QPen boxPen(boxColor);
    boxPen.setWidth(box_width);
    p.setPen(boxPen);
    p.drawRect(box_offset, box_offset, w, h);
}
void Graph::drawGrid(QPainter& p)
{
    QPen gridPen(gridColor);
    gridPen.setWidth(grid_line_width);
    p.setPen(gridPen);
    int i = 0;
    while(zx + i * u <= width() - box_offset)
    {
        i++;
        p.drawLine(zx + i * u, box_offset, zx + i * u, height() - box_offset);
        p.drawLine(zx - i * u, box_offset, zx - i * u, height() - box_offset);
    }
    i = 0;
    while(zy + i * u < height())
    {
        i++;
        p.drawLine(box_offset, zy + i * u, width() - box_offset, zy + i * u);
        p.drawLine(box_offset, zy - i * u, width() - box_offset, zy - i * u);
    }
}
void Graph::drawAxis(QPainter& p)
{
    QPen axisPen(axisColor);
    axisPen.setWidth(axis_width);
    p.setPen(axisPen);
    p.drawLine(box_offset, zy, width() - box_offset, zy);
    p.drawLine(zx, box_offset, zx, height() - box_offset);
}
void Graph::drawTicks(QPainter& p)
{
    QPen ticksPen(axisColor);
    ticksPen.setWidth(axis_width);
    p.setPen(ticksPen);
    QFont font = p.font();
    font.setPixelSize(12);
    p.setFont(font);
    int i = 0;
    p.drawText(QRect{zx + pixel_width, zy - u + pixel_width, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(0));
    while(zx + (i + 2) * u < width())
    {
        i++;
        p.drawLine(zx + i * u, zy + tick_length, zx + i * u, zy - tick_length);
        p.drawLine(zx - i * u, zy + tick_length, zx - i * u, zy - tick_length);
        if (zx + (i + 2) * u < width())
            p.drawText(QRect{zx + i * u + pixel_width, zy - u + pixel_width, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(i));
        p.drawText(QRect{zx - (i) * u + pixel_width, zy - u + pixel_width, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(-i));
    }
    i = 0;
    while(zy + (i + 2) * u < height())
    {
        i++;
        p.drawLine(zx - tick_length, zy + i * u, zx + tick_length, zy + i * u);
        p.drawLine(zx - tick_length, zy - i * u, zx + tick_length, zy - i * u);
        if (zy - (i + 2) * u > 0)
            p.drawText(QRect{zx + pixel_width, zy - (i + 1) * u +  pixel_width, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(i));
        p.drawText(QRect{zx + pixel_width, zy + (i - 1) * u + pixel_width, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(-i));
    }
}
void Graph::drawArrows(QPainter& p)
{
    QPen arrowsPen(axisColor);
    arrowsPen.setWidth((axis_width));
    p.setBrush(QBrush(axisColor));
    p.setRenderHint(QPainter::RenderHint::Antialiasing);
    //arrow x
    QPainterPath px;
    px.moveTo(width() - u - 1, zy + 2 * tick_length);
    px.lineTo(width() - u - 1, zy - 2 * tick_length);
    px.lineTo(width() - 1, zy);
    px.lineTo(width() - u - 1, zy + 2 * tick_length);
    p.drawPath(px);
    p.drawText(QRect{width() - u / 2 - 1, zy + u / 2, u, u}, "X");
    //arrow y
    QPainterPath py;
    py.moveTo(zx + 2 * tick_length, u + 1);
    py.lineTo(zx - 2 * tick_length, u + 1);
    py.lineTo(zx, 1);
    py.lineTo(zx + 2 * tick_length, u + 1);
    p.drawPath(py);
    p.drawText(QRect{zx + u / 2, u / 2, u, u}, "Y");
}
void Graph::drawPixels(QPainter& p)
{
    for (const auto& data: pixels)
    {
        auto pixel = data.first;
        int greyval =  max_grey_value * (1 - data.second);
        QColor pixelColor(greyval, greyval, greyval);
        QPen pixelPen(pixelColor, pixel_width);
        p.setPen(pixelPen);
        p.setBrush(QBrush(pixelColor));
        int xpos = zx + (pixel.first) * u + pixel_width;
        int ypos = zy - (pixel.second + 1) * u + pixel_width;
        p.drawRect(xpos, ypos, u - pixel_width, u - pixel_width);
        if (greyval < 3 * max_grey_value / 4)
        {
            p.setPen(Qt::white);
        }
        else
        {
            p.setPen(Qt::black);
        }
        if (pixel.second == 0)
        {
            p.drawText(QRect{xpos, ypos, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(pixel.first));
        }
        else if (pixel.first == 0)
        {
            p.drawText(QRect{xpos, ypos, u - pixel_width, u - pixel_width}, Qt::AlignLeft | Qt::AlignBottom, QString::number(pixel.second));
        }
    }
}
void Graph::AddPixel(int x, int y, qreal percent)
{
    pixels.push_back({{x, y}, percent});
    repaint();
}
void Graph::Clear()
{
    pixels.clear();
    repaint();
}
void Graph::SetUnit(int nu)
{
      u = nu;
}
void Graph::paintEvent(QPaintEvent*)
{
    zx = width() / 2;
    zy = height() / 2;
    QPainter pt(this);
    drawBox(pt);
    drawGrid(pt);
    drawAxis(pt);
    drawTicks(pt);
    drawArrows(pt);
    drawPixels(pt);
}
int Graph::getScale() const
{
    return u;
}
