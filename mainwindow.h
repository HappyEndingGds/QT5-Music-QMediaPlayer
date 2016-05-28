#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QUrl>
#include <QFileDialog>
#include <QTimer>
#include <QMediaMetaData>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QListWidget>
#include <QMediaObject>
#include <QVariant>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void playMusic(QString pm_path);

    void rollWord();

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_verticalSlider_valueChanged(int value);

    void changPostion();

    void on_horizontalSlider_valueChanged(int value);

    void getCurrentTime();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void getDuration();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *music;
    QString str_music_path,str_music_title,str_current_time;//,str_file_path;
    QTimer *timer,*timer_current;
    QDate date;
    QTime time1;
    QDateTime time,time_current;
    QFileInfo info;
    QDir dir;
    QFileInfoList file_list;
    QMediaObject *media;
    int music_fen,music_sec,music_total_time;



};

#endif // MAINWINDOW_H
