#include "aspectratiolabel.h"

AspectRatioLabel::AspectRatioLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f)
{
}

AspectRatioLabel::~AspectRatioLabel()
{
}

void AspectRatioLabel::setPixmap(const QPixmap& pm)
{
    pix = pm;
    pixmapWidth = pm.width();
    pixmapHeight = pm.height();

    updateMargins();
    QLabel::setPixmap(pm);
}

int AspectRatioLabel::heightForWidth( int width ) const
{
    return pix.isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

QSize AspectRatioLabel::sizeHint() const
{
    int w = this->width();
    return QSize( w, heightForWidth(w) );
}

QPixmap AspectRatioLabel::scaledPixmap() const
{
    return pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void AspectRatioLabel::resizeEvent(QResizeEvent* event)
{
    updateMargins();

    if(!pix.isNull())
        QLabel::setPixmap(scaledPixmap());

    QLabel::resizeEvent(event);
}

void AspectRatioLabel::updateMargins()
{
    if (pixmapWidth <= 0 || pixmapHeight <= 0)
        return;

    int w = this->width();
    int h = this->height();

    if (w <= 0 || h <= 0)
        return;

    setContentsMargins(0, 0, 0, 0);

    /*if (w * pixmapHeight > h * pixmapWidth)
    {
        int m = (w - (pixmapWidth * h / pixmapHeight)) / 2;
        setContentsMargins(m, 0, m, 0);
    }
    else
    {
        int m = (h - (pixmapHeight * w / pixmapWidth)) / 2;
        setContentsMargins(0, m, 0, m);
    }*/
}
