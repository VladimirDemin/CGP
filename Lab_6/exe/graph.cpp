#include "graph.h"
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QMouseEvent>

PlotArea::PlotArea(QWidget *parent):QWidget(parent),
    AksonometricMatrix(Matrix::GetAksonometricMatrix(angleX, angleY, angleZ)), TransformationMatrix(Matrix::GetIdentityMatrix()),
    ProjectionMatrix(Matrix::GetIdentityMatrix())
{
    u = std::min(width(), height()) / 20;
    recalculateAxis();
}

void PlotArea::SetRotatable(bool newRotatable)
{
    isRotatable = newRotatable;
}
void PlotArea::recalculateAxis()
{
    axis = Matrix::DecomposeToPoints(AksonometricMatrix * Matrix::ComposeFromPoints({Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1)}));
}
QPointF PlotArea::Adjust(const Point& _p)
{
    QPointF p = axis[0].toQPoint() * _p.getParameter(0) + axis[1].toQPoint() * _p.getParameter(1) + axis[2].toQPoint() * _p.getParameter(2);

    return QPointF(zx + p.x() * u, zy - p.y() * u);
}
void PlotArea::drawBox(QPainter& p)
{
    int h = height() - 2 * box_offset;
    int w = width() - 2 * box_offset;
    QPen boxPen(boxColor);
    boxPen.setWidth(box_width);
    p.setPen(boxPen);
    p.drawRect(box_offset, box_offset, w, h);
}
void PlotArea::drawGrid(QPainter& p)
{
    QPen gridPen(Qt::black);
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
void PlotArea::drawAxis(QPainter& p)
{
    QPointF center(zx, zy);


    QPen axisPen(Qt::black);
    axisPen.setWidth(axis_width);

    p.setPen(axisPen);
    p.drawLine(Adjust(Point(-axis_length, 0, 0)), Adjust(Point(axis_length, 0, 0)));

    axisPen.setColor(Qt::black);
    p.setPen(axisPen);
    p.drawLine(Adjust(Point(0, -axis_length, 0)), Adjust(Point(0, axis_length, 0)));

    axisPen.setColor(Qt::black);
    p.setPen(axisPen);
    p.drawLine(Adjust(Point(0, 0, -axis_length)), Adjust(Point(0, 0, axis_length)));

    axisPen.setColor(Qt::black);
    p.setPen(axisPen);
    p.drawLine(center, Adjust({1, 0, 0}));
    p.drawLine(center, Adjust({0, 1, 0}));
    p.drawLine(center, Adjust({0, 0, 1}));

}

void PlotArea::drawFigure(QPainter& p)
{
    if (!figure.empty())
    {
        std::vector<Point> toDraw = Matrix::DecomposeToPoints(ProjectionMatrix * TransformationMatrix * Matrix::ComposeFromPoints(figure));
        QPainterPath ph1;
        QPainterPath ph2;
        p.setPen(QPen(Qt::black, line_width));
        p.setBrush(Qt::NoBrush);
        int shift = toDraw.size() / 2;
        ph1.moveTo(Adjust(toDraw[0]));
        ph2.moveTo(Adjust(toDraw[shift]));
        p.drawLine(Adjust(toDraw[0]), Adjust(toDraw[0 + shift]));
        for (size_t i = 1; i < toDraw.size() / 2; ++i)
        {
            ph1.lineTo(Adjust(toDraw[i]));
            ph2.lineTo(Adjust(toDraw[i + shift]));
            p.drawLine(Adjust(toDraw[i]), Adjust(toDraw[i + shift]));
        }
        p.drawPath(ph1);
        p.drawPath(ph2);
    }
    p.drawText(Adjust(Point(axis_length + 1.5, 1, 0)), "X");
    p.drawText(Adjust(Point(0, axis_length + 1.5, 0)), "Y");
     p.drawText(Adjust(Point(0, 1, axis_length + 1.5)), "Z");
}
void PlotArea::TransformFigure(Matrix const& transform)
{
    TransformationMatrix = transform * TransformationMatrix;
}
void PlotArea::ProjectFigure(Matrix::ProjectionType type)
{
    ProjectionMatrix = Matrix::GetProjectionMatrix(type);
}
void PlotArea::RevertProjection()
{
    ProjectionMatrix = Matrix::GetIdentityMatrix();
}
void PlotArea::ResetTransform()
{
    TransformationMatrix = Matrix::GetIdentityMatrix();
}
Matrix PlotArea::GetTransformationMatrix() const
{
    return ProjectionMatrix * TransformationMatrix;
}
void PlotArea::SetFigurePoints(const std::vector<Point>& data)
{
    figure = data;
}
void PlotArea::Clear()
{
    figure.clear();
}
void PlotArea::paintEvent(QPaintEvent*)
{
    zx = width() / 2;
    zy = height() / 2;
    AksonometricMatrix = Matrix::GetAksonometricMatrix(angleX, angleY, angleZ);
    recalculateAxis();
    QPainter pt(this);
    drawBox(pt);
    drawAxis(pt);
    drawFigure(pt);

}
void PlotArea::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->pos();
    mousePressed = true;
}
void PlotArea::mouseMoveEvent(QMouseEvent* event)
{
    if (mousePressed && isRotatable)
    {
        QPointF position = event->pos();
        double deltaX = position.x() - lastMousePos.x();
        double deltaY = position.y() - lastMousePos.y();
        angleY += angleShift * deltaX;
        angleX += angleShift * deltaY;
        lastMousePos = position;
        repaint();
    }
}
void PlotArea::SetRotation(double _angleX, double _angleY, double _angleZ)
{
    angleX = _angleX;
    angleY = _angleY;
    angleZ = _angleZ;
}
void PlotArea::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
void PlotArea::wheelEvent(QWheelEvent* event)
{

    SetUnit(u + delta_unit * (2 * (event->angleDelta().y() > 0) - 1));
    repaint();
}
int PlotArea::getUnit() const
{
    return u;
}
void PlotArea::SetUnit(int nu)
{
    if (nu >= min_unit && nu <= max_unit)
    {
        u = nu;
    }
}

