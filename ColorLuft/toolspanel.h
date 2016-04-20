#ifndef TOOLSPANEL_H
#define TOOLSPANEL_H

#include <QFrame>
#include <QRadioButton>
#include <QSlider>
#include <QPushButton>

class CToolsPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CToolsPanel(QWidget *parent = 0);

    QWidget *settingsArea;
    QWidget *autoArea;
    QRadioButton *autoluftingRBtn;
    QWidget *byHandArea;
    QRadioButton *byHandLuftingRBtn;
    QWidget *slidersArea;
    QSlider *rSlider;
    QSlider *gSlider;
    QSlider *bSlider;
    QWidget *buttonsArea;
    QPushButton *loadBtn;
    QPushButton *startBtn;

signals:

public slots:

protected:
    virtual QSize sizeHint () const {return QSize(100, 600);}

};

#endif // TOOLSPANEL_H
