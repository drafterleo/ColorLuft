#include "luftinfo.h"
#include <QPainter>
#include <QHBoxLayout>

CLuftInfo::CLuftInfo(QWidget *parent)
    : QWidget(parent)
{
    resize(500, 450);
    luftR = 0;
    luftG = 0;
    luftB = 0;
    pixSize = QSize(250, 200);

    okBtn = new QPushButton("Ok");
    connect(okBtn, SIGNAL(clicked()), SLOT(needClose()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    layout->addWidget(okBtn);
    this->setLayout(layout);
}

void CLuftInfo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    /* draw background */
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x202020));
    painter.setPen(QPen(QBrush(QColor(Qt::gray)), 2));
    painter.drawRoundRect(this->rect().adjusted(1, 1, -2, -2), 5, 5);

    /* draw axis, grid and color lufts */
    int yCenter = this->height() / 2;
    int boxWidth = 30;
    int xBeg = 275;
    int xEnd = 490;
    QRect box = QRect(xBeg + 2, yCenter, boxWidth, 0);

    painter.setPen(QPen(QBrush(QColor(0x80, 0x80, 0x80, 100)), 1));
    for (int i = 1; i < 10; ++i) {
        painter.drawLine(xBeg, yCenter - i*20, xEnd, yCenter - i*20);
        painter.drawLine(xBeg, yCenter + i*20, xEnd, yCenter + i*20);
    }

    qreal scale = 1.8;
    //painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0xFF, 0, 0, 150));
    box.setBottom(yCenter - qRound(luftR * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0, 0xFF, 0, 150));
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound(luftG * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0, 0, 0xFF, 150));
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound(luftB * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0xFF, 0xFF, 0, 150)); //yellow
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound((luftR + luftG)/2 * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0xFF, 0x80, 0, 150)); //orange
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound((luftR + luftG * (128.0/256.0))/2 * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0, 0xBF, 0xFF, 150)); //light blue
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound((luftB + luftG * (191.0/256.0))/2 * scale));
    painter.drawRect(box);
    painter.setBrush(QColor(0x80, 0x00, 0xFF, 150)); //violet
    box.translate(boxWidth, 0);
    box.setBottom(yCenter - qRound((luftB + luftR * (180/256.0))/2 * scale));
    painter.drawRect(box);

    painter.setPen(QPen(QBrush(QColor(Qt::gray)), 2));
    painter.drawLine(xBeg, yCenter, xEnd, yCenter);

    /* draw images */

    QRect protoPixRect = QRect(QPoint(10, yCenter - pixSize.height() - 2), pixSize);
    QRect alterPixRect = QRect(QPoint(10, yCenter + 2), pixSize);

    if (!protoPix.isNull()) {
        painter.drawPixmap(protoPixRect.left() + (pixSize.width() - protoPix.width()) / 2,
                           protoPixRect.top() + (pixSize.height() - protoPix.height()) / 2,
                           protoPix);
    }
    if (!alterPix.isNull()) {
        painter.drawPixmap(alterPixRect.left() + (pixSize.width() - alterPix.width()) / 2,
                           alterPixRect.top() + (pixSize.height() - alterPix.height()) / 2,
                           alterPix);
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QBrush(QColor(Qt::gray)), 2));
    painter.drawRect(protoPixRect);
    painter.drawRect(alterPixRect);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(QRect(10, 7, 300, 10), Qt::AlignLeft | Qt::AlignVCenter, tr("Proto"));
    painter.drawText(QRect(10, alterPixRect.bottom() + 7, 300, 10), Qt::AlignLeft | Qt::AlignVCenter, tr("Alter"));

    painter.end();
}

void CLuftInfo::setProtoPix(const QImage & img)
{
    protoPix = QPixmap::fromImage(img.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void CLuftInfo::showModal()
{
//    CFader *fader = new CFader(this->parentWidget());
//    fader->show();
//    fader->raise();

    this->show();
    this->raise();

    (void) eventLoop.exec(QEventLoop::DialogExec); // <- exits with ::needClose() slot

//    delete fader;
}

void CLuftInfo::needClose()
{
    this->hide();
    eventLoop.exit(0);
}

void CLuftInfo::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit showed();
}

void CLuftInfo::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    emit hided();
}

/*********************************************************************************/

CFader::CFader(QWidget *parent)
    : QWidget(parent)
{
    if (parentWidget())
        resize(parentWidget()->size());
}

void CFader::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setBrush(QColor(100, 100, 100, 150));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
    painter.end();
}


void CLuftInfo::setAlterPix(const QImage & img)
{
    alterPix = QPixmap::fromImage(img.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

