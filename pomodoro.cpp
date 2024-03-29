#include "pomodoro.h"
#include "ui_pomodoro.h"
#include <QTimer>
#include <QMessageBox>
#include <QApplication>
#include <QSound>

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QIcon>
#include <QShortcut>

enum state{ACT,REST,STOP}current_state;
int break_nbr=0;
int iteration=0;

pomodoro::pomodoro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::pomodoro)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint |
                   Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    ui->pushButton->setText("START");
    ui->radioButton_3->setChecked(true);
    ui->pushButton->setFocus();
    current_state=STOP;

    QShortcut *shotcut_button1 = new QShortcut(QKeySequence(Qt::Key_Enter),this);
    QShortcut *shotcut_button2 = new QShortcut(QKeySequence(Qt::Key_Return),this);
    QShortcut *shotcut_button3 = new QShortcut(QKeySequence(Qt::Key_Space),this);
    QShortcut *shotcut_close = new QShortcut(QKeySequence(Qt::Key_Escape),this);
    connect(shotcut_button1, SIGNAL(activated()), this, SLOT(on_pushButton_released()));
    connect(shotcut_button2, SIGNAL(activated()), this, SLOT(on_pushButton_released()));
    connect(shotcut_button3, SIGNAL(activated()), this, SLOT(on_pushButton_released()));
    connect(shotcut_close, SIGNAL(activated()), this, SLOT(_hide_it()));

    connect(&_timer, &QTimer::timeout, this, &pomodoro::handleTimeout);

    createActions();
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &pomodoro::iconActivated);
    trayIcon->show();
    QWidget::activateWindow();
}

pomodoro::~pomodoro()
{
    delete ui;
}

void pomodoro::handleTimeout()
{
    QWidget::showNormal();
    QWidget::setFocus();
    switch (current_state)
    {
    case ACT:
        QSound::play(":/sounds/rest.wav");
        if(iteration==break_nbr)
        {
            _timer.start(1000*60*2*(ui->spinBox_2->value()));
            iteration=1;
//            ui->label_2->setText("2");
        }
        else
        {
//            ui->label_2->setText("1");
            iteration++;
            _timer.start(1000*60*(ui->spinBox_2->value()));
        }
        current_state=REST;
        break;

    case REST:
        QSound::play(":/sounds/act.wav");
        current_state=ACT;
        _timer.start(1000*60*(ui->spinBox->value()));
        break;

    case STOP:
        QMessageBox::critical(0, qApp->tr("Error"),
                              qApp->tr("Failed, error: %2").arg(2));
        break;
    }
    _updateUi();
}

void pomodoro::_hide_it()
{
    hide();
}

void pomodoro::_updateUi()
{
    switch (current_state)
    {
    case ACT:
        ui->label->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->label_2->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        break;
    case REST:
        ui->label->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->label_2->setEnabled(true);
        ui->spinBox_2->setEnabled(false);
        break;
    case STOP:
        ui->label->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
        break;
    }
}

void pomodoro::on_pushButton_released()
{
    if(current_state==STOP)
    {
        QSound::play(":/sounds/act.wav");
        ui->pushButton->setText("STOP");
        ui->radioButton->setDisabled(true);
        ui->radioButton_2->setDisabled(true);
        ui->radioButton_3->setDisabled(true);
        if(ui->radioButton->isChecked()) break_nbr=2;
        if(ui->radioButton_2->isChecked()) break_nbr=3;
        if(ui->radioButton_3->isChecked()) break_nbr=4;
        iteration=1;
        current_state=ACT;
        _timer.start(1000*60*(ui->spinBox->value()));
        hide();
    }
    else
    {
        _timer.stop();
        ui->pushButton->setText("START");
        ui->radioButton->setDisabled(false);
        ui->radioButton_2->setDisabled(false);
        ui->radioButton_3->setDisabled(false);
        current_state=STOP;
    }
    _updateUi();
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void pomodoro::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible())
    {
        return;
    }
#endif
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void pomodoro::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        QWidget::showNormal();
        break;
    default:
        ;
    }
}

void pomodoro::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(QIcon(":/images/heart.png"));
    trayIcon->setContextMenu(trayIconMenu);
    setWindowIcon(QIcon(":/images/heart.png"));
}

void pomodoro::createActions()
{
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}
