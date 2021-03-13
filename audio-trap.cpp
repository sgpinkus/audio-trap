/**
 * Program that listens to an audio input device and records audio to file if a threshold is exceeded.
 * Program has an in built UI that allow monitoring and configuring. use --ui option to launch it.
 */
#include <QApplication>
#include <QCommandLineParser>
#include <QtGlobal>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include "audiotraprecorder.h"
#include "audiotrapmainwindow.h"
#include "handle_signals.h"

void appMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void printMsg(QString msg);
int sig_clean_up(int signum);
static QPointer<AudioTrapMainWindow> windowPtr = nullptr;

/**
 * Setup application meta then run AudioTrapRecorder.
 */
int main(int argc, char *argv[])
{
    handle_signals(sig_clean_up);
    qInstallMessageHandler(appMsgHandler);
    QApplication::setOrganizationName("audio-trap");
    QApplication::setApplicationName("audio-trap");
    QApplication a(argc, argv);
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    parser.setApplicationDescription("Audio Trap");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption uiModeOption("ui", QCoreApplication::translate("main", "Launch UI, write messages to UI instead of CLI"));
    parser.addOption(uiModeOption);
    parser.process(a);
    bool uiMode = parser.isSet(uiModeOption);
    AudioTrapRecorder recorder;
    AudioTrapMainWindow window(recorder);
    if(uiMode) {
        windowPtr = QPointer<AudioTrapMainWindow>(&window);
        window.show();
    }
    else {
        qDebug() << "Activating Audio Trap";
        recorder.activate();
    }
    return a.exec();
}

/**
 * Terminal signal clean up callback.
 * Only called with signals handle_signals() determines are terminal.
 * For some call qApp->quit() before quitting to allow Qt app to clean itself up the Qt way.
 * Returning 1 from this function will make handle_signals() return to program flow to allow Qt quit.
 * @see handle_signals.c
 */
int sig_clean_up(int signum)
{
  int cont = 0;
  static int continued = 0;

  if(signum == SIGBUS || signum == SIGFPE || signum == SIGILL || signum == SIGSEGV) {
    write(STDERR_FILENO, "Abnormal Termination.\n", 22);
  }
  else if(qApp) {
    qApp->quit();
    cont = 1;
  }
  if(continued) {
    cont = 0;
  }
  continued = 1;
  return cont;
}

/**
 * Simple pipe to syslog. Default Qt fatal generates a core dump and terminates.
 * Termination is handy, maintain that. If you want a core dump use SIGABRT.
 * @todo qFatal() still terminates app abruptly so qApp->quit() is a null op!
 */
void appMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    (void)(context);
    switch (type) {
        case QtDebugMsg:
            printMsg(QString("%1").arg(msg)); // .arg(context.file).arg(context.line).arg(context.function));
            break; 
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
        case QtInfoMsg:
            printMsg(QString("INFO:     %1").arg(msg));
            break;
        #endif
        case QtWarningMsg:
            printMsg(QString("WARNING:  %1").arg(msg));
            break;
        case QtCriticalMsg:
            printMsg(QString("CRITICAL: %1").arg(msg));
            break;
        case QtFatalMsg:
            printMsg(QString("FATAL:    %1").arg(msg));
            abort();
    }
}

void printMsg(QString msg) {
    if(windowPtr.isNull()) {
        fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
    }
    else if(windowPtr->isVisible()) {
        windowPtr->addMessage(msg);
    }
}
