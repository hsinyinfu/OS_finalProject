#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void reset();
    void action(int);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage *agentLabelImage;
    QImage *smkrTobaccoLabelImage;
    QImage *smkrPaperLabelImage;
    QImage *smkrMatchLabelImage;
    QImage *tobaccoLabelImage;
    QImage *paperLabelImage;
    QImage *matchLabelImage;
    QImage *tableLabelImage;
    QImage *Timage;
    QImage *Pimage;
    QImage *Mimage;
    QImage *NullImage;
    QImage *Tsurprise;
    QImage *Psurprise;
    QImage *Msurprise;
    QImage *Surprise;
    QImage *sup;

};

#endif // MAINWINDOW_H
