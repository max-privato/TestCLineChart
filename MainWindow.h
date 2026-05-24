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
    void showEvent(QShowEvent *) override;
    ~MainWindow();

private slots:

    void chartValuesChanged(SXYValues values, bool hDifference, bool vDifference);
    void barPlot_clicked();
    void dataTBtn_clicked(bool checked);
    void Force0Ckb_clicked();
    void linePlot_clicked();
    void MC_clicked();
    void McD_clicked();
    void QtF_clicked();

    void plotBtn_clicked();
    void scaleTBtn_clicked();
    void swarmPlot_clicked();

    void widthThin_clicked();
    void widthThick_clicked();
    void widthAuto_clicked();
    void xDbScale_clicked();
    void xLinScale_clicked();
    void xLogScale_clicked();
    void yLogScale_clicked();
    void yLinScale_clicked();
    void yDbScale_clicked();

    void diffTBtn_clicked();
    void titleTBtn_clicked(bool checked);
    void QtPoly_clicked();

    void QTi_clicked();

private:
    bool exactMatch;
    Ui::MainWindow *ui;
    CScaleDlg *myScaleDlg;

};

#endif // MAINWINDOW_H
