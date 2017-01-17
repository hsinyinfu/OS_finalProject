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

    Timage = new QImage("/Users/fu-hsin-ying/OS/pic/T.png");
    tobaccoLabelImage = Timage;
    ui->Tobacco->setPixmap(QPixmap::fromImage(*tobaccoLabelImage));

    Pimage = new QImage("/Users/fu-hsin-ying/OS/pic/P.png");
    paperLabelImage = Pimage;
    ui->Paper->setPixmap(QPixmap::fromImage(*paperLabelImage));

    Mimage = new QImage("/Users/fu-hsin-ying/OS/pic/M.png");
    matchLabelImage = Mimage;
    ui->Match->setPixmap(QPixmap::fromImage(*matchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Table.png");
    tableLabelImage = Image;
    ui->Table->setPixmap(QPixmap::fromImage(*tableLabelImage));

    NullImage = new QImage("/Users/fu-hsin-ying/OS/pic/null.png");

    Surprise = new QImage("/Users/fu-hsin-ying/OS/pic/surprise.png");
    Tsurprise = NullImage;
    ui->Tsurprise->setPixmap(QPixmap::fromImage(*Tsurprise));
    Psurprise = NullImage;
    ui->Psurprise->setPixmap(QPixmap::fromImage(*Psurprise));
    Msurprise = NullImage;
    ui->Msurprise->setPixmap(QPixmap::fromImage(*Msurprise));

    sup = NullImage;
    ui->sup->setPixmap(QPixmap::fromImage(*sup));
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

    tobaccoLabelImage = NullImage;
    ui->Tobacco->setPixmap(QPixmap::fromImage(*tobaccoLabelImage));
    paperLabelImage = NullImage;
    ui->Paper->setPixmap(QPixmap::fromImage(*paperLabelImage));
    matchLabelImage = NullImage;
    ui->Match->setPixmap(QPixmap::fromImage(*matchLabelImage));

    Image = new QImage("/Users/fu-hsin-ying/OS/pic/Table.png");
    tableLabelImage = Image;
    ui->Table->setPixmap(QPixmap::fromImage(*tableLabelImage));

    Tsurprise = NullImage;
    ui->Tsurprise->setPixmap(QPixmap::fromImage(*Tsurprise));
    Psurprise = NullImage;
    ui->Psurprise->setPixmap(QPixmap::fromImage(*Psurprise));
    Msurprise = NullImage;
    ui->Msurprise->setPixmap(QPixmap::fromImage(*Msurprise));

    sup = NullImage;

}

void MainWindow::action(int Case){
    switch(Case){
        case 1: //TOBACCO be supplied
            tobaccoLabelImage = Timage;
            sup = Timage;
            ui->Tobacco->setPixmap(QPixmap::fromImage(*tobaccoLabelImage));
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            break;
        case 3: //PAPER be supplied
            paperLabelImage = Pimage;
            sup = Pimage;
            ui->Paper->setPixmap(QPixmap::fromImage(*paperLabelImage));
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            break;
        case 5: //MATCH be supplied
            matchLabelImage = Mimage;
            sup = Mimage;
            ui->Match->setPixmap(QPixmap::fromImage(*matchLabelImage));
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            break;
        case 8: //smkrTobacco is coming
            Tsurprise = Surprise;
            sup = NullImage;
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            ui->Tsurprise->setPixmap(QPixmap::fromImage(*Tsurprise));
            break;
        case 4: //smkrMatch is coming
            Msurprise = Surprise;
            sup = NullImage;
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            ui->Msurprise->setPixmap(QPixmap::fromImage(*Msurprise));
            break;
        case 6: //smkrPaper is coming
            Psurprise = Surprise;
            sup = NullImage;
            ui->sup->setPixmap(QPixmap::fromImage(*sup));
            ui->Psurprise->setPixmap(QPixmap::fromImage(*Psurprise));
            break;
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}
