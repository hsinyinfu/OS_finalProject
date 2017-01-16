#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage *Image = new QImage( "/Users/fu-hsin-ying/OS/pic/Agent.png" ) ;
    agentLabelImage = Image;
    ui->Agent->setPixmap(QPixmap::fromImage(*agentLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Tobacco.png");
    smkrTobaccoLabelImage = Image;
    ui->smkrTobacco->setPixmap(QPixmap::fromImage(*smkrTobaccoLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Paper.png");
    smkrPaperLabelImage = Image;
    ui->smkrPaper->setPixmap(QPixmap::fromImage(*smkrPaperLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Match.png");
    smkrMatchLabelImage = Image;
    ui->smkrMatch->setPixmap(QPixmap::fromImage(*smkrMatchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/T.png");
    tobaccoLabelImage = Image;
    ui->Tobacco->setPixmap(QPixmap::fromImage(*tobaccoLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/P.png");
    paperLabelImage = Image;
    ui->Paper->setPixmap(QPixmap::fromImage(*paperLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/M.png");
    matchLabelImage = Image;
    ui->Match->setPixmap(QPixmap::fromImage(*matchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Table.png");
    tableLabelImage = Image;
    ui->Table->setPixmap(QPixmap::fromImage(*tableLabelImage));
//    QPushButton *bu = new QPushButton(tr("初始"),this);
//    push = bu;
}

void MainWindow::reset(){
    QImage *Image = new QImage( "/Users/fu-hsin-ying/OS/pic/Agent.png" ) ;
    agentLabelImage = Image;
    ui->Agent->setPixmap(QPixmap::fromImage(*agentLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Tobacco.png");
    smkrTobaccoLabelImage = Image;
    ui->smkrTobacco->setPixmap(QPixmap::fromImage(*smkrTobaccoLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Paper.png");
    smkrPaperLabelImage = Image;
    ui->smkrPaper->setPixmap(QPixmap::fromImage(*smkrPaperLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Match.png");
    smkrMatchLabelImage = Image;
    ui->smkrMatch->setPixmap(QPixmap::fromImage(*smkrMatchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/null.png");
    tobaccoLabelImage = Image;
    ui->Tobacco->setPixmap(QPixmap::fromImage(*tobaccoLabelImage));
    paperLabelImage = Image;
    ui->Paper->setPixmap(QPixmap::fromImage(*paperLabelImage));
    matchLabelImage = Image;
    ui->Match->setPixmap(QPixmap::fromImage(*matchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Table.png");
    tableLabelImage = Image;
    ui->Table->setPixmap(QPixmap::fromImage(*tableLabelImage));

}

MainWindow::~MainWindow()
{
    delete ui;
}
