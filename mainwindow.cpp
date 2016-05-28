#include "mainwindow.h"
#include "ui_mainwindow.h"
static int n=0;
qint64 current_position,old_position,total_time;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    music = new QMediaPlayer(this);
    timer =new QTimer(this);
    timer_current =new QTimer(this);
    getCurrentTime();
    ui->statusBar->addWidget(ui->lab_currentTime);
    timer_current->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(changPostion()));
    connect(timer_current,SIGNAL(timeout()),this,SLOT(getCurrentTime()));
    connect(music,SIGNAL(durationChanged(qint64)),this,SLOT(getDuration()));
    ui->pushButton->setText("暂停");
    ui->pushButton->setDisabled(true);

    /*---------------------------------------------------------*/

    QDir dir("E://Youku Files//手机音乐//My Music");
    QStringList filter;
    filter<<"*.mp3";
    dir.setNameFilters(filter);

    file_list=dir.entryInfoList();

    QStringList listwidget_name;
    for(int i =0;i<file_list.count();i++)
    {
        listwidget_name.append(file_list.at(i).baseName());
    }
    ui->listWidget->addItems(listwidget_name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDuration()
{
     music_total_time=music->duration();//music->duration()获取音乐播放总时间,单位:毫秒
     ui->horizontalSlider->setMaximum(music->duration());
}



//播放音乐
void MainWindow::playMusic(QString pm_path)
{
    music->setMedia(QUrl(pm_path));
    music->play();
    timer->start(1000);
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText("暂停");

    str_music_title="                                    ";//用于滚动字幕,实现的比较粗暴,用空格占位
    pm_path=pm_path.section('/',-1);            //从绝对路径中截取文件名
    pm_path=pm_path.section('.',-2,-2);        //从文件命中去掉后缀
    str_music_title+=pm_path;

    date.setDate(1,3,5);
    time.setDate(date);//所以上边的data初始化时,数值不为0就好
    time1.setHMS(0,0,0);
    time.setTime(time1);
    /*
     下面是给 time 这个时间对象初始化,我们目的是:"得到时分秒"为都为0的time对象,
     但是QDateTime类规定,必须先初始化合法的年月日时间后,才能初始化 时分秒都为0,
     time只初始化时分秒,年月日默认为0,但是0年0月0日是不合法的值!
    */
}

//滚动字幕
void MainWindow::rollWord()
{
    if(n>str_music_title.length())//滚动标题
        n=0;
    ui->label_2->setText(str_music_title.mid(n));
    n++;
}

//播放暂停按钮
void MainWindow::on_pushButton_clicked()
{

    if(ui->pushButton->text()=="暂停")
    {
         music->pause();
         timer->stop();
         ui->pushButton->setText("播放");     
    }
   else
    {
       music->play();
       timer->start(1000);
       ui->pushButton->setText("暂停");
    }
}

//打开按钮
void MainWindow::on_pushButton_3_clicked()
{
    str_music_path = QFileDialog::getOpenFileName(this,tr("打开音乐文件"),"E:/Youku Files/手机音乐/流行",tr("全部文件(*.*);;音乐文件(*.mp3);;歌词文件(*.lrc)"));
    if(str_music_path!=NULL)
        playMusic(str_music_path);
}

//设置音量
void MainWindow::on_verticalSlider_valueChanged(int value)
{
    //滑块设置音量
    music->setVolume(value);
}

void MainWindow::changPostion()
{
    /*本函数与定时器绑定,每一秒钟进入一次,刷新播放进度条*/

   current_position=music->position();//music->position()获取音乐当前播放位置
   if(current_position >= ui->horizontalSlider->maximum())//播放完毕
    {
       timer->stop();
       music->stop();
       ui->pushButton->setText("播放");
       ui->horizontalSlider->setSliderPosition(0);
       time1.setHMS(0,0,0);
       time.setTime(time1);//重新把时间标签改为00:00*/
       ui->lab_time->setText(time.toString("mm:ss"));

        if(ui->checkBox->isChecked())//是否是单曲循环
        {
            playMusic(str_music_path);
        }
        return;
     }
   ui->horizontalSlider->setSliderPosition(current_position);//刷新滑块位置
   time=time.addSecs(1);
   ui->lab_time->setText(time.toString("mm:ss"));
   rollWord();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    /*本函数用于快进快退*/

    //当滑块的值(是绝对值)改变一次性超多2000时,我们认为有人拖动滑块,要快进!(定时器的刷新滑块1s在1013)
    if(qAbs(value-current_position)>2000&&(ui->horizontalSlider->value()!=0))
    {
        music->stop();
        timer->stop();
        music->setPosition(value);

        /*更新时间标签*/
        music_fen=(value/1000)/60;
        music_sec=(value/1000)%60;
        time1.setHMS(0,music_fen,music_sec);
        time.setTime(time1);

        timer->start();
        music->play();
    }
}

void MainWindow::getCurrentTime()
{
    time_current =QDateTime::currentDateTime();
    ui->lab_currentTime->setText( time_current.toString("yyyy-MM-dd hh:mm:ss dddd"));
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    for(int i=0;i<file_list.count();i++)
    {
        if(item->text()==file_list.at(i).baseName())
        {
            str_music_path=file_list.at(i).absoluteFilePath();
            playMusic(str_music_path);
        }
    }
}
