#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void cha();
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
//    QPushButton *push;
};

#endif // MAINWINDOW_H
