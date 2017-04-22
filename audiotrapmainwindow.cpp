#include <QFileDialog>
#include "audiotrapmainwindow.h"
#include "ui_audiotrapmainwindow.h"

AudioTrapMainWindow::AudioTrapMainWindow(AudioTrapRecorder &recorder, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AudioTrapMainWindow),
    m_recorder(recorder),
    m_levelMeter(m_recorder.getAudioLevelMeter())
{
    ui->setupUi(this);
    connect(&m_recorder, SIGNAL(activated()), this, SLOT(update()), Qt::UniqueConnection);
    connect(&m_recorder, SIGNAL(deactivated()), this, SLOT(update()), Qt::UniqueConnection);
    connect(&m_recorder, SIGNAL(recording(QString)), this, SLOT(update()), Qt::UniqueConnection);
    connect(&m_recorder, SIGNAL(stopped(QString)), this, SLOT(update()), Qt::UniqueConnection);
    connect(&m_levelMeter, SIGNAL(update(int)), this, SLOT(levelUpdate()), Qt::UniqueConnection);
    update();
}

void AudioTrapMainWindow::show() {
    QMainWindow::show();
    QDialog
}

void AudioTrapMainWindow::update()
{
    // input device
    ui->inputDeviceComboBox->clear();
    ui->inputDeviceComboBox->addItems(m_recorder.audioInputs());
    QString deviceName = m_recorder.audioInput();
    int deviceIndex = ui->inputDeviceComboBox->findText(deviceName);
    ui->inputDeviceComboBox->setCurrentIndex(deviceIndex);
    // containers
    ui->containerFormatComboBox->clear();
    ui->containerFormatComboBox->addItems(m_recorder.supportedContainers());
    ui->containerFormatComboBox->setCurrentIndex(ui->containerFormatComboBox->findText(m_recorder.getContainerFormat()));
    // outputDir
    ui->outputDirLabel->setText(m_recorder.getOutputDir());
    // thold settings
    ui->lowTriggerSlider->setValue(m_levelMeter.getLowThreshold()*1000);
    ui->highTriggerSlider->setValue(m_levelMeter.getHighThreshold()*1000);
    // dampening
    ui->dampeningSpinBox->setValue(m_levelMeter.getDampening());
    // active/inactive
    if(m_recorder.isActive()) {
        ui->inputDeviceComboBox->setDisabled(true);
        ui->activateButton->setText("Deactivate");
        if(m_recorder.isRecording()) {
            ui->recordingStatusLabel->setText("Recording");
        }
        else {
            ui->recordingStatusLabel->setText("Stopped");
        }
    }
    else {
        ui->inputDeviceComboBox->setEnabled(true);
        ui->activateButton->setText("Activate");
        ui->recordingStatusLabel->setText("Deactivated");
    }
}

void AudioTrapMainWindow::levelUpdate()
{
    qreal exp = m_levelMeter.channelExpAmplitude();
    QList<qreal> channels = m_levelMeter.avgAmplitude();
    if(channels.length() > 0) {
        ui->channel0ProgressBar->setValue(quint32(channels[0]*100));
    }
    else {
        ui->channel0ProgressBar->setValue(0);
    }
    if(channels.length() > 1) {
        ui->channel1ProgressBar->setValue(quint32(channels[1]*100));
    }
    else {
        ui->channel1ProgressBar->setValue(0);
    }
    ui->triggerLevelProgressBar->setValue(quint32(exp*100));
}

AudioTrapMainWindow::~AudioTrapMainWindow()
{
    delete ui;
}

void AudioTrapMainWindow::addMessage(const QString &msg)
{
    ui->statusTextEdit->append(msg);
}

void AudioTrapMainWindow::on_dampeningSpinBox_valueChanged(double arg1)
{
    (void)(arg1);
    m_levelMeter.setDampening(ui->dampeningSpinBox->value());
}

void AudioTrapMainWindow::on_lowTriggerSlider_valueChanged(int value)
{
    (void)(value);
    quint32 v = ui->lowTriggerSlider->value();
    qreal rv = m_levelMeter.setLowThreshold(v/1000.0);
    ui->lowTriggerSlider->setValue(quint32(rv*1000));
    ui->highTriggerSlider->setValue(m_levelMeter.getHighThreshold()*1000);
}

void AudioTrapMainWindow::on_highTriggerSlider_valueChanged(int value)
{
    (void)(value);
    quint32 v = ui->highTriggerSlider->value();
    qreal rv = m_levelMeter.setHighThreshold(v/1000.0);
    ui->highTriggerSlider->setValue(quint32(rv*1000));
    ui->lowTriggerSlider->setValue(m_levelMeter.getLowThreshold()*1000);
}

/**
 * Recorder may adjust passed in tholds so need to get return value and reset them.
 */
void AudioTrapMainWindow::setThresholds(quint32 l, quint32 h) {


}

void AudioTrapMainWindow::on_activateButton_clicked()
{
    if(m_recorder.isActive()) {
        m_recorder.deactivate();
        ui->activateButton->setText("Activate");
    }
    else {
        m_recorder.activate();
        ui->activateButton->setText("Deactivate");
    }
}

void AudioTrapMainWindow::on_activateButton_toggled(bool checked)
{
    if(checked) {
        m_recorder.activate();
    }
    else {
        m_recorder.deactivate();
    }
}

void AudioTrapMainWindow::on_inputDeviceComboBox_activated(const QString &arg1)
{
    if(m_recorder.audioInput() != arg1) {
        qDebug() << "Setting audio device to " << arg1;
        m_recorder.setAudioInput(arg1);
    }
}

void AudioTrapMainWindow::on_outputDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
        tr("Select Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks
    );
    m_recorder.setOutputDir(dir);
    ui->outputDirLabel->setText(m_recorder.getOutputDir());
}

void AudioTrapMainWindow::on_containerFormatComboBox_activated(const QString &arg1)
{
    m_recorder.setContainerFormat(arg1);
}
