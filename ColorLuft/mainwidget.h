#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDir>
#include "colorlufter.h"
#include "toolspanel.h"
#include "luftinfo.h"

class CMainWidget : public QWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = 0);
    ~CMainWidget();

private slots:
    void openImage();
    void startStopLufting();
    void startAutoLufting();
    void startByHandLufting();
    void sliderChanged(int);
    void fadeLufter();
    void unfadeLufter();

protected:
    void loadRandomImage(void);

    inline virtual QSize sizeHint() const {return QSize(800, 600);}
    void showRgbSliders(bool show);

    virtual void keyPressEvent(QKeyEvent *);
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    CColorLufter *lufter;
    CToolsPanel  *tools;
    CLuftInfo    *info;

    bool isAutoLufting;
    bool isByHandLufting;
};

#endif // MAINWIDGET_H
