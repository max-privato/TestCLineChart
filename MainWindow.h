#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CScaleDlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void chartValuesChanged(SXYValues values, bool hDifference, bool vDifference);
    void on_barPlot_clicked();
    void on_dataTBtn_clicked(bool checked);
    void on_Force0Ckb_clicked();
    void on_linePlot_clicked();
    void on_MC_clicked();
    void on_McD_clicked();
    void on_QtF_clicked();
    void on_QtI_clicked();
    void on_plotBtn_clicked();
    void on_scaleTBtn_clicked();
    void on_swarmPlot_clicked();

    void on_widthThin_clicked();
    void on_widthThick_clicked();
    void on_widthAuto_clicked();
    void on_xDbScale_clicked();
    void on_xLinScale_clicked();
    void on_xLogScale_clicked();
    void on_yLogScale_clicked();
    void on_yLinScale_clicked();
    void on_yDbScale_clicked();

    void on_diffTBtn_clicked();
    void on_titleTBtn_clicked(bool checked);

private:
    bool exactMatch;
    Ui::MainWindow *ui;
    CScaleDlg *myScaleDlg;

};

#endif // MAINWINDOW_H
