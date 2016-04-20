#ifndef LUFTINFO_H
#define LUFTINFO_H

#include <QWidget>
#include <QRgb>
#include <QPushButton>
#include <QEventLoop>

class CLuftInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CLuftInfo(QWidget *parent = 0);

    inline void setLufts(int r, int g, int b) {luftR = r; luftG = g; luftB = b;}
    void setProtoPix(const QImage & img);
    void setAlterPix(const QImage & img);

signals:
    void showed();
    void hided();

public slots:
    void showModal();
    void needClose();

protected:
    inline virtual QSize sizeHint() {return QSize(500, 450);}

    virtual void paintEvent(QPaintEvent *);

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    int luftR;
    int luftG;
    int luftB;

    QPushButton *okBtn;
    QPixmap protoPix;
    QPixmap alterPix;
    QSize pixSize;

    QEventLoop eventLoop;

};

class CFader : public QWidget
{
    Q_OBJECT
public:
    explicit CFader(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent * event);

signals:

public slots:

};

#endif // LUFTINFO_H
