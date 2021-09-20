#ifndef POMODORO_H
#define POMODORO_H

#include <QMainWindow>
#include <QTimer>
#include <QDialog>

#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class pomodoro; }
QT_END_NAMESPACE

class pomodoro : public QMainWindow
{
    Q_OBJECT

public:
    pomodoro(QWidget *parent = nullptr);
    ~pomodoro();

private slots:
    void on_pushButton_released();
    void handleTimeout();
    void _updateUi();
    void _hide_it();

private:
    Ui::pomodoro *ui;
        QTimer      _timer;


        //////////////
public:
//    void setVisible(bool visible) Q_DECL_OVERRIDE;
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);


private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();


    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};
#endif // POMODORO_H
