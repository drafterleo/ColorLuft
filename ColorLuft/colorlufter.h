#ifndef COLORLUFTER_H
#define COLORLUFTER_H

#include <QWidget>
#include <QImage>
#include <QTimer>

class CColorLufter : public QWidget
{
    Q_OBJECT
public:
    explicit CColorLufter(QWidget *parent = 0);

    void loadImage(const QString & fileName);
    void tuneLuft(int r, int g, int b);
    void randomLuft();

    inline int getLuftR() {return luftR;}
    inline int getLuftG() {return luftG;}
    inline int getLuftB() {return luftB;}

    inline const QImage & getProtoImage() {return protoImage;}
    inline const QImage & getAlterImage() {return alterImage;}
    inline void setFadeImage(bool fade){isImageFaded = fade; update();}

signals:

public slots:
    void startLufting(void);
    void stopLufting(void);

private slots:
    void luftTimerTick(void);
    void convergenceTimerTick(void);

protected:
    virtual void paintEvent(QPaintEvent *event);

    void resetLuftParameters();
    void updateAlterImage();

    int genSwingInterval(void);
    int genSpeed(void);

private:
    QImage protoImage;
    QImage alterImage;

    QTimer luftTimer;
    QTimer convergenceTimer;
    bool isLuftingStarted;
    bool isAlterImageUpdating;
    bool timeToConvergence;
    bool isConvergenceStarted;
    bool isImageFaded;

    int luftR;
    int luftG;
    int luftB;

    int stepR;
    int stepB;
    int stepG;

    int swingR;
    int swingG;
    int swingB;

    int speedR;
    int speedG;
    int speedB;
};

#endif // COLORLUFTER_H
