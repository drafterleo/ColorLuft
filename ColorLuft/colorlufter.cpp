#include <QPainter>
#include <QtDebug>
#include <QRgb>
#include <QTime>
#include <QCoreApplication>
#include "colorlufter.h"

CColorLufter::CColorLufter(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 600);

    isAlterImageUpdating = false;
    isLuftingStarted = false;
    isImageFaded = false;
    timeToConvergence = false;
    connect(&luftTimer, SIGNAL(timeout()), this, SLOT(luftTimerTick()));
    connect(&convergenceTimer,SIGNAL(timeout()), this, SLOT(convergenceTimerTick()));
}

void CColorLufter::loadImage(const QString & fileName)
{
    protoImage.load(fileName);
    alterImage = protoImage;
    this->update();
}

void CColorLufter::resizeEvent(QResizeEvent *)
{

}

void CColorLufter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);
    if (!alterImage.isNull()) {
        QSize imgSize = alterImage.size();
        imgSize.scale(this->rect().size(), Qt::KeepAspectRatio);
        QRect targetRect = QRect(QPoint((this->width() - imgSize.width()) / 2,
                                        (this->height() - imgSize.height()) / 2),
                                 imgSize);
        painter.drawImage(targetRect, alterImage, alterImage.rect());
    }
    if (isImageFaded) {
        painter.fillRect(this->rect(), QColor(0x10, 0x10, 0x10, 200));
    }
    painter.end();
}

void CColorLufter::tuneLuft(int r, int g, int b)
{
    int tmpR = luftR;
    int tmpG = luftG;
    int tmpB = luftB;

    luftR = qBound(-126, luftR + r, 126);
    luftG = qBound(-126, luftG + g, 126);
    luftB = qBound(-126, luftB + b, 126);
    updateAlterImage();

    luftR = tmpR;
    luftG = tmpG;
    luftB = tmpB;
}

void CColorLufter::randomLuft()
{
    luftR = qrand()%255 - 127;
    luftG = qrand()%255 - 127;
    luftB = qrand()%255 - 127;
    updateAlterImage();
}

int CColorLufter::genSwingInterval(void)
{
    return qrand()%200 + 100;
}

int CColorLufter::genSpeed(void)
{
    return qrand()%8 + 2;
}

void CColorLufter::resetLuftParameters()
{
    luftR = qrand()%255 - 127;
    luftG = qrand()%255 - 127; //luftR * ((qrand()%2 > 0) ? -1 : 1);
    luftB = qrand()%255 - 127; //luftR * ((qrand()%2 > 0) ? -1 : 1);
    stepR = (luftR > 0) ? -1 : 1;
    stepG = (luftG > 0) ? -1 : 1;
    stepB = (luftB > 0) ? -1 : 1;
    swingR = genSwingInterval();
    swingG = genSwingInterval();
    swingB = genSwingInterval();
    speedR = genSpeed();
    speedG = genSpeed();
    speedB = genSpeed();
}

void CColorLufter::startLufting(void)
{
    isLuftingStarted = true;
    qsrand(QTime::currentTime().msec());
    resetLuftParameters();
    luftTimer.start(100);
    convergenceTimer.start(10000 + qrand()%30000);
    isConvergenceStarted = false;
    updateAlterImage();
}

void CColorLufter::stopLufting(void)
{
    luftTimer.stop();
    convergenceTimer.stop();
    alterImage = protoImage;
    this->update();
    isLuftingStarted = false;
    isConvergenceStarted = false;

#ifdef QT_DEBUG
    qDebug() << luftR << luftG << luftB;
#endif
}

void CColorLufter::convergenceTimerTick(void)
{
    timeToConvergence = true;
    convergenceTimer.setInterval(10000 + qrand()%30000);
}

int sign (int val)
{
    if (val == 0)
        return 0;
    if (val > 0)
        return 1;
    return -1;
}

void CColorLufter::luftTimerTick(void)
{
    if (isConvergenceStarted) {
        if (qAbs(luftR) > speedR)
            luftR += stepR * speedR;
        if (qAbs(luftG) > speedG)
            luftG += stepG * speedG;
        if (qAbs(luftB) > speedB)
            luftB += stepB * speedB;

        if (qAbs(luftR) <= speedR &&
            qAbs(luftG) <= speedG &&
            qAbs(luftB) <= speedB)
        {
            isConvergenceStarted = false;
            speedR = 1;
            speedB = 1;
            speedG = 1;
            swingR = 1;
            swingG = 1;
            swingB = 1;
#ifdef QT_DEBUG
            qDebug() << luftR << luftG << luftB;
#endif
        }

    } else {
        luftR += stepR * speedR;
        luftG += stepG * speedG;
        luftB += stepB * speedB;

        if (luftR < -126 || luftR > 126)
            stepR *= -1;
        if (luftG < -126 || luftG > 126)
            stepG *= -1;
        if (luftB < -126 || luftB > 126)
            stepB *= -1;

        swingR -= speedR;
        swingG -= speedG;
        swingB -= speedB;

        if (swingR < 0) {
            swingR = genSwingInterval();
            speedR = genSpeed();
            stepR *= -1;
        }
        if (swingG < 0) {
            swingG = genSwingInterval();
            speedG = genSpeed();
            stepG *= -1;
        }
        if (swingB < 0) {
            swingB = genSwingInterval();
            speedB = genSpeed();
            stepB *= -1;
        }

        if (timeToConvergence && luftR != 0 && luftG !=0 && luftB != 0) {
            int luftMin = static_cast<qreal>(qMin(qAbs(luftR), qMin(qAbs(luftG), qAbs(luftB))));
            stepR = -1 * sign(luftR);
            stepG = -1 * sign(luftG);
            stepB = -1 * sign(luftB);
            swingR = luftR + 2;
            swingG = luftG + 2;
            swingB = luftB + 2;
            speedR = qRound(qAbs(static_cast<qreal>(luftR) / luftMin));
            speedG = qRound(qAbs(static_cast<qreal>(luftG) / luftMin));
            speedB = qRound(qAbs(static_cast<qreal>(luftB) / luftMin));
#ifdef QT_DEBUG
            qDebug() << "convergence!" << luftR << luftG << luftB
                     << ":" << speedR << speedG << speedB;
#endif
            isConvergenceStarted = true;
            timeToConvergence = false;
        }
    }

    updateAlterImage();
}

void CColorLufter::updateAlterImage()
{
    if (isAlterImageUpdating)
        return;

    isAlterImageUpdating = true;

    //qDebug() << QTime::currentTime().toString("mm:ss.zzz");
    //alterImage = baseImage;
    memcpy(alterImage.bits(), protoImage.bits(), alterImage.byteCount());
    for (qint32 y = 0; y < alterImage.height(); ++y) {
        QRgb *scanLine = (QRgb *) alterImage.scanLine(y);
        for (qint32 x = 0; x < alterImage.width(); ++x){
            int r = qBound(0, qRed(scanLine[x]) + luftR, 255);
            int g = qBound(0, qGreen(scanLine[x]) + luftG, 255);
            int b = qBound(0, qBlue(scanLine[x]) + luftB, 255);
            scanLine[x] = qRgb(r, g, b);
        }
        if (!isLuftingStarted)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    //qDebug() << QTime::currentTime().toString("mm:ss.zzz");

    isAlterImageUpdating = false;
    this->update();
}


