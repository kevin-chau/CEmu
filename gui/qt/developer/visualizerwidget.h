#ifndef MEMORYVISUALIZER_H
#define MEMORYVISUALIZER_H

#include "widgets/visualizerlcdwidget.h"

#include <QtWidgets/QWidget>
QT_BEGIN_NAMESPACE
class QLineEdit;
class QGroupBox;
class QToolButton;
class QRadioButton;
QT_END_NAMESPACE

class VisualizerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizerWidget(const QString &config, QWidget *parent = nullptr);
    QString getConfig();
    void translate();
    void forceUpdate();

private slots:
    void resetView();
    void showConfig();
    void showPresets();

signals:
    void configChanged();

private:
    void stringToView();
    void viewToString();

    QStringList mSetup;

    int mScale = 100;
    int mFps = 30;

    QLineEdit *mConfigStr;
    QGroupBox *mGroup;

    QToolButton *mBtnLcd;
    QToolButton *mBtnRefresh;
    QToolButton *mBtnConfig;

    VisualizerLcdWidget *mLcd;
    VisualizerLcdWidgetConfig mLcdConfig;
};

#endif
