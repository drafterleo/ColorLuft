#include "toolspanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QButtonGroup>

CToolsPanel::CToolsPanel(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(500);
    setMinimumWidth(100);
    setMaximumWidth(100);

    autoluftingRBtn = new QRadioButton("auto");
        autoluftingRBtn->setChecked(true);
    byHandLuftingRBtn = new QRadioButton("by hand");

    QButtonGroup *btnGrp = new QButtonGroup(this);
    btnGrp->addButton(autoluftingRBtn);
    btnGrp->addButton(byHandLuftingRBtn);

    rSlider = new QSlider(Qt::Vertical);
        rSlider->setMinimumHeight(300);
        rSlider->setMinimum(-126);
        rSlider->setMaximum(126);
        rSlider->setSliderPosition(0);
    gSlider = new QSlider(Qt::Vertical);
        gSlider->setMinimumHeight(300);
        gSlider->setMinimum(-126);
        gSlider->setMaximum(126);
        gSlider->setSliderPosition(0);
    bSlider = new QSlider(Qt::Vertical);
        bSlider->setMinimumHeight(300);
        bSlider->setMinimum(-126);
        bSlider->setMaximum(126);
        bSlider->setSliderPosition(0);
    startBtn = new QPushButton("Start");
    loadBtn = new QPushButton("Load Image");

    QVBoxLayout *vLayout = new QVBoxLayout;
        vLayout->setContentsMargins(1, 5, 1, 5);
        vLayout->setAlignment(Qt::AlignHCenter);

    settingsArea = new QWidget;
    settingsArea->setMinimumHeight(550);
    QVBoxLayout *saLayout = new QVBoxLayout;
        saLayout->setContentsMargins(0, 0, 0, 0);
        saLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    buttonsArea = new QWidget;
    QVBoxLayout *baLayout = new QVBoxLayout;
        baLayout->setContentsMargins(0, 0, 0, 0);
        baLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        baLayout->addWidget(loadBtn);
        QFrame *line = new QFrame;
            line->setMinimumWidth(90);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
        baLayout->addWidget(line);
    buttonsArea->setLayout(baLayout);
    saLayout->addWidget(buttonsArea);

    autoArea = new QWidget;
    QVBoxLayout *aaLayout = new QVBoxLayout;
        aaLayout->setContentsMargins(0, 0, 0, 0);
        //aaLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        aaLayout->addWidget(autoluftingRBtn);
        line = new QFrame;
            line->setMinimumWidth(90);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
        aaLayout->addWidget(line);
    autoArea->setLayout(aaLayout);
    saLayout->addWidget(autoArea);

    byHandArea = new QWidget;
    QVBoxLayout *bhLayout = new QVBoxLayout;
        bhLayout->setContentsMargins(0, 0, 0, 0);
        //bhLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        bhLayout->addWidget(byHandLuftingRBtn);
        line = new QFrame;
            line->setMinimumWidth(90);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
        bhLayout->addWidget(line);
    byHandArea->setLayout(bhLayout);
    saLayout->addWidget(byHandArea);

    slidersArea = new QWidget;
    QGridLayout *slidersLayout = new QGridLayout;
        slidersLayout->setAlignment(Qt::AlignCenter);
        slidersLayout->setContentsMargins(0, 0, 0, 0);
        slidersLayout->addWidget(rSlider, 0, 0, Qt::AlignCenter);
        slidersLayout->addWidget(gSlider, 0, 1, Qt::AlignCenter);
        slidersLayout->addWidget(bSlider, 0, 2, Qt::AlignCenter);
        slidersLayout->addWidget(new QLabel("R"), 1, 0, Qt::AlignCenter);
        slidersLayout->addWidget(new QLabel("G"), 1, 1, Qt::AlignCenter);
        slidersLayout->addWidget(new QLabel("B"), 1, 2, Qt::AlignCenter);
    slidersArea->setLayout(slidersLayout);
    saLayout->addWidget(slidersArea);

    settingsArea->setLayout(saLayout);
    vLayout->addWidget(settingsArea);

    vLayout->addSpacerItem(new QSpacerItem(1, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    line = new QFrame;
        line->setMinimumWidth(90);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
    vLayout->addWidget(line);
    vLayout->addWidget(startBtn);
    this->setLayout(vLayout);
}
