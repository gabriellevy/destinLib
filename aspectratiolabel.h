#ifndef ASPECTRATIOPIXMAPLABEL_H
#define ASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AspectRatioLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AspectRatioLabel();
    QPixmap scaledPixmap() const;
    virtual int heightForWidth( int width ) const override;
    virtual QSize sizeHint() const override;

public slots:
    void setPixmap(const QPixmap& pm);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap pix;
    void updateMargins();

    int pixmapWidth = 0;
    int pixmapHeight = 0;
};

#endif // ASPECTRATIOPIXMAPLABEL_H
