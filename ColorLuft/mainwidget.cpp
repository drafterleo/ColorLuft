#include <QTime>
#include <QtDebug>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include "mainwidget.h"

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
{
    isAutoLufting = false;
    isByHandLufting = false;

    this->resize(900, 600);

    lufter = new CColorLufter(this);
    tools = new CToolsPanel(this);
    info = new CLuftInfo(this);
    info->hide();

    showRgbSliders(false);

    connect(tools->startBtn, SIGNAL(clicked()), this, SLOT(startStopLufting()));
    connect(tools->loadBtn, SIGNAL(clicked()), this, SLOT(openImage()));
    connect(tools->rSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(tools->gSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(tools->bSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(info, SIGNAL(showed()), this, SLOT(fadeLufter()));
    connect(info, SIGNAL(hided()), this, SLOT(unfadeLufter()));

    QList<QWidget *> toolWidgets = tools->findChildren<QWidget *>();
    foreach(QWidget *w, toolWidgets) {
        w->installEventFilter(this);
    }

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(1);
    hLayout->setMargin(0);
    hLayout->addWidget(lufter, 2);
    hLayout->addWidget(tools);
    this->setLayout(hLayout);

    loadRandomImage();
}

CMainWidget::~CMainWidget()
{

}

void CMainWidget::showRgbSliders(bool show)
{
    tools->slidersArea->setVisible(show);
}


void CMainWidget::loadRandomImage(void)
{
    QDir imagesDir(QCoreApplication::applicationDirPath() + "/Images");
    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg";
    QStringList files = imagesDir.entryList(imageFilters);
    qDebug() << imagesDir.path() << files.count();
    if (!files.isEmpty()) {
        qsrand(QTime::currentTime().msec());
        lufter->loadImage(imagesDir.path() + "/" + files.at(qrand() % files.count()));
    }
}

void CMainWidget::startStopLufting(void)
{
    if (isAutoLufting || isByHandLufting)
    {
        /* stop lufting */

       info->setProtoPix(lufter->getProtoImage());
       info->setAlterPix(lufter->getAlterImage());

       tools->startBtn->setText("Start");
       showRgbSliders(false);

       lufter->stopLufting();

       if (isByHandLufting) {
           qDebug() << "sliders:" << tools->rSlider->value() << tools->gSlider->value() << tools->bSlider->value();
           info->setLufts(qBound(-126, lufter->getLuftR() + tools->rSlider->value(), 126),
                          qBound(-126, lufter->getLuftG() + tools->gSlider->value(), 126),
                          qBound(-126, lufter->getLuftB() + tools->bSlider->value(), 126));
       }

       if (isAutoLufting) {
           this->releaseKeyboard();
           info->setLufts(lufter->getLuftR(),
                          lufter->getLuftG(),
                          lufter->getLuftB());
       }

       info->move(lufter->pos() +
                  QPoint((lufter->width() - info->width()) / 2,
                         (lufter->height() - info->height()) / 2));
       info->show();

       tools->settingsArea->setVisible(true);
       tools->buttonsArea->setVisible(true);
       tools->autoArea->setVisible(true);
       tools->byHandArea->setVisible(true);
       isAutoLufting = false;
       isByHandLufting = false;
    }
    else
    {
        /* start lufting */

        info->hide();

        if (tools->byHandLuftingRBtn->isChecked()) {
            tools->startBtn->setText("Results");
            tools->buttonsArea->setVisible(false);
            tools->autoArea->setVisible(false);
            startByHandLufting();
        } else {
            tools->startBtn->setText("Stop");
            tools->settingsArea->setVisible(false);
            this->grabKeyboard();
            startAutoLufting();
        }

    }
}

void CMainWidget::startAutoLufting()
{
    isAutoLufting = true;
    lufter->startLufting();
}

void CMainWidget::startByHandLufting()
{
    isByHandLufting = true;
    showRgbSliders(true);
    tools->rSlider->setSliderPosition(0);
    tools->gSlider->setSliderPosition(0);
    tools->bSlider->setSliderPosition(0);
    lufter->randomLuft();
}

void CMainWidget::sliderChanged(int)
{
    if (isByHandLufting) {
        lufter->tuneLuft(tools->rSlider->value(),
                         tools->gSlider->value(),
                         tools->bSlider->value());
    }
}

void CMainWidget::openImage(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    //QCoreApplication::applicationDirPath() + "/Images",
                                                    "",
                                                    tr("Image Files (*.png *.jpg)"));
    if (!fileName.isEmpty()) {
        lufter->loadImage(fileName);
    }
}

void CMainWidget::keyPressEvent(QKeyEvent *event)
{
    if (isAutoLufting && (event->key() == Qt::Key_Space)) {
        startStopLufting();
    }
    if (info->isVisible() && event->key() == Qt::Key_Escape) {
        info->hide();
    }
}

bool CMainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space) {
            startStopLufting();
            return true;
        }
        if (info->isVisible() && keyEvent->key() == Qt::Key_Escape) {
            info->hide();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CMainWidget::fadeLufter()
{
    lufter->setFadeImage(true);
}

void CMainWidget::unfadeLufter()
{
    lufter->setFadeImage(false);
}




