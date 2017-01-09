#ifndef AUDIOTRAPMAINWINDOW_H
#define AUDIOTRAPMAINWINDOW_H

#include <QMainWindow>
#include "audiotraprecorder.h"
#include "audiolevelmeter.h"

namespace Ui {
class AudioTrapMainWindow;
}

class AudioTrapMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AudioTrapMainWindow(AudioTrapRecorder & recorder, QWidget *parent = 0);
    ~AudioTrapMainWindow();

public slots:
    void show();
    void addMessage(const QString &msg);

private slots:
    void update();
    void levelUpdate();
    void on_dampeningSpinBox_valueChanged(double arg1);
    void on_lowTriggerSlider_valueChanged(int value);
    void on_highTriggerSlider_valueChanged(int value);
    void on_activateButton_clicked();
    void on_activateButton_toggled(bool checked);
    void on_inputDeviceComboBox_activated(const QString &arg1);
    void on_outputDirButton_clicked();
    void on_containerFormatComboBox_activated(const QString &arg1);

private:
    Ui::AudioTrapMainWindow *ui;
    AudioTrapRecorder &m_recorder;
    AudioLevelMeter &m_levelMeter;
    void setThresholds(quint32 l, quint32 h);
};

#endif // AUDIOTRAPMAINWINDOW_H
