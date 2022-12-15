#include "plotarea.h"
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
void PlotArea::drawAxis(QPainter& p)
{
    /*auto f = [this, &p](Point const& point, QColor color){
        double vx = point.getParameter(0);
        double vy = point.getParameter(1);
        double x = 0;
        double y = 0;
        if (vx == 0)
        {
            x = width() / 2;
            y = 0;
        }
        else
        {
            double t = -zx / vx;
            y = zy + vy * t;
            if (y < 0 || y > height())
            {
                if (vy == 0)
                {
                    y = zy;
                    x = 0;
                }
                else
                {
                    t = -zy / vy;
                    y = 0;
                    x = zx + vx * t;
                }
            }
        }
        QPen axisPen(color);
        axisPen.setWidth(axis_width);
        p.setPen(axisPen);
        p.drawLine(x, height() - y, width() - x, y);
    };*/
    //f(axis[0], Qt::blue);
    //f(axis[1], Qt::green);
    //f(axis[2], Qt::magenta);
    QPointF center(zx, zy);


    QPen axisPen(XColor);
    axisPen.setWidth(axis_width);

    p.setPen(axisPen);
    p.drawLine(Adjust(Point(-axis_length, 0, 0)), Adjust(Point(axis_length, 0, 0)));

    axisPen.setColor(YColor);
    p.setPen(axisPen);
    p.drawLine(Adjust(Point(0, -axis_length, 0)), Adjust(Point(0, axis_length, 0)));

    axisPen.setColor(ZColor);
    p.setPen(axisPen);
    p.drawLine(Adjust(Point(0, 0, -axis_length)), Adjust(Point(0, 0, axis_length)));

    axisPen.setColor(axisColor);
    p.setPen(axisPen);
    p.drawLine(center, Adjust({1, 0, 0}));
    p.drawLine(center, Adjust({0, 1, 0}));
    p.drawLine(center, Adjust({0, 0, 1}));

}
void PlotArea::drawTicks(QPainter& p)
{
    QPen ticksPen(axisColor);
    ticksPen.setWidth(axis_width);
    p.setPen(ticksPen);
    QFont font = p.font();
    font.setPixelSize(12);
    p.setFont(font);
    //ticks x
    int alignFlags = Qt::AlignRight | Qt::AlignTop;
    p.drawText(QRect{zx  - u + pixel_width, zy + pixel_width, u - pixel_width, u - pixel_width}, alignFlags, QString::number(0));
    for (int i = 1; i <= axis_length; ++i)
    {
        p.drawLine(Adjust(Point(i, 0, -tick_length / 2)), Adjust(Point(i, 0, tick_length / 2)));
        p.drawLine(Adjust(Point(-i, 0, -tick_length / 2)), Adjust(Point(-i, 0, tick_length / 2)));
    }
    //ticks y
    for (int i = 1; i <= axis_length; ++i)
    {
        p.drawLine(Adjust(Point(0, i, -tick_length / 2)), Adjust(Point(0, i, tick_length / 2)));
        p.drawLine(Adjust(Point(0, -i, -tick_length / 2)), Adjust(Point(0, -i, tick_length / 2)));
    }
    //ticks z
    for (int i = 1; i <= axis_length; ++i)
    {
        p.drawLine(Adjust(Point(-tick_length / 2, 0, i)), Adjust(Point(tick_length / 2, 0, i)));
        p.drawLine(Adjust(Point(-tick_length / 2, 0, -i)), Adjust(Point(tick_length / 2, 0, -i)));
    }
}
void PlotArea::drawArrows(QPainter& p)
{
    QPen arrowsPen(axisColor);
    arrowsPen.setWidth((axis_width));
    p.setBrush(QBrush(axisColor));
    p.setRenderHint(QPainter::RenderHint::Antialiasing);
    //arrow x
    QPainterPath px;
    px.moveTo(Adjust(Point(axis_length, 0, -tick_length / 2)));
    px.lineTo(Adjust(Point(axis_length + 1, 0, 0)));
    px.lineTo(Adjust(Point(axis_length, 0, tick_length / 2)));
    px.lineTo(Adjust(Point(axis_length, 0, -tick_length / 2)));
    p.drawPath(px);
    p.drawText(Adjust(Point(axis_length + 1.5, 1, 0)), "X");
    //arrow y
    QPainterPath py;
    py.moveTo(Adjust(Point(0, axis_length, -tick_length / 2)));
    py.lineTo(Adjust(Point(0, axis_length + 1, 0)));
    py.lineTo(Adjust(Point(0, axis_length, tick_length / 2)));
    py.lineTo(Adjust(Point(0, axis_length, -tick_length / 2)));
    p.drawPath(py);
    p.drawText(Adjust(Point(0, axis_length + 1.5, 0)), "Y");
    //arrow z
    QPainterPath pz;
    pz.moveTo(Adjust(Point(-tick_length / 2, 0, axis_length)));
    pz.lineTo(Adjust(Point(0, 0, axis_length + 1)));
    pz.lineTo(Adjust(Point(tick_length / 2, 0, axis_length)));
    pz.lineTo(Adjust(Point(-tick_length / 2, 0, axis_length)));
    p.drawPath(pz);
    p.drawText(Adjust(Point(0, 1, axis_length + 1.5)), "Z");
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
    drawTicks(pt);
    drawArrows(pt);
    drawFigure(pt);
    //drawGrid(pt);
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

