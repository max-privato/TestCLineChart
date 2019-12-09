#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Matrix.h"
#include "CLineChart.h"
#include "CSimOut.cpp"
#include <QString>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QElapsedTimer>


/* NOTA IMPORTANTE
 * IN DATA 1/12/2018 è STATA ATTIVA TA ANCHE LA FUNZIONE DI LETTURA DA ADFfILE
 * Putroppo si verifica che sconq uesto tipo di lettura al momento il programma è spesso imprevedibile, evidentemente per qualche problema di uso di memoriia non inizializzata / allocazione di memoria.
 *  Non è stato al momento compreso quale possa essere la causa. Però si lascia il cla funzione di lettura di testFile attiva in quanto può fornire informazioni utili per il debug di ClineChart, seppur con qualche imprevedibilità.
 * ovviamente si auspica che prima o poi la causa del problema venga individuata e risolta.
 * */

/*Per la visualizzazione del grafico verrà  utilizzata una tecnica alquanto articolata.
 * Qui si spiegano sinteticamente le varie scelte.
 * 1. La scrittura del grafico non viene fatta direttamente ma attraverso una QPainterPath
 *    in quanto i path vengono conservati per eventuali successive azioni di Cut&Paste
 *    ** E' da verificare se questo comporti effettivamente un vantaggio o meno **
 * 2. La scrittura del grafico direttamente sul fondo della finestra presenta lo svantaggio
 *    che se il numero di punti è molto elevato può essere lenta la riscrittura in seguito
 *    di una richiesta di paint. Se innvece si scrive su una QImage, essa conserva del
 *    grafico una bitmap che si rivisualizza molto più rapidamente.
 * 3. La QImage può a sua volta essere visualizzata sul fondo della finestra direttamente.
 *    Si preferisce però inserirla all'interno di una QLabel in quanto in tal modo le due
 *    dimensioni vengono gestite dal Layout. Infatti la QLabel è presente nel Designer,
 *    mentre la QImage no (in quanto non è un widget)
 * 4. Infine per la visualizzazione si fa riferimento ad una Scrooll Area, in quanto essa
 *    è in grado di fare il resize automaticamente dell'image correlata. Questo è utile
 *    durante il resize della nostra finestra: infatti in tal modo invece di dover
 *    ritracciare il plot attraverso delle lineTo di tutti i punti durante il resize,
 *    quindi molte volte al secondo, il resize si fa sulla base della pixmap contenuta
 *    nella image e solo dopo che il mouse è rilasciato verrà fatto un aggiornamento
 *    completo del grafico attravero una ritracciatura.
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(150,100);

    //    ui->lineChart->mark();
    myScaleDlg =new CScaleDlg(this);
    myScaleDlg->setWindowTitle("Plot scale setup");
    ui->plotBtn->setDefault(true);
    connect(ui->lineChart,SIGNAL(valuesChanged(SXYValues,bool,bool)),this, SLOT(chartValuesChanged(SXYValues,bool,bool)));
    ui->lineChart->linearInterpolate=false;
    ui->lineChart->addLegend=false;
    ui->interpolateBox->setVisible(false);
    ui->xValueLbl->setVisible(false);
    ui->yValueLbl->setVisible(false);


    if (ui->xDbScale->isChecked())
       ui->lineChart->setXScaleType(stDB);
    if (ui->xLogScale->isChecked())
       ui->lineChart->setXScaleType(stLog);
    if (ui->yDbScale->isChecked())
       ui->lineChart->setYScaleType(stDB);
    if (ui->yLogScale->isChecked())
       ui->lineChart->setYScaleType(stLog);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chartValuesChanged(SXYValues values, bool hDifference, bool vDifference){
    QString msg;
    msg=msg.setNum(values.X[0],'g',4);
    if(hDifference)
      msg= "*"+msg;
    ui->xValueLbl->setText(msg);
    msg=msg.setNum(values.Y[0][0],'g',4);
    if(vDifference) msg= "*"+msg;
    ui->yValueLbl->setText(msg);
}


void MainWindow::on_plotBtn_clicked()
{
  bool Error=false;
  int i, nPoints, nPlots=1; //nPlots vale normalmente 1; E' forzato a 2 solo se deve essere messa la scala destra.

  // L'allocazione statica seguente serve perché i dati devono rimanere allocati anche dsopo l'uscita dal questa funzione, per reagire agli stimioli di utente, ad es. movimento del mouse sopra il grafico
  static float **x;
  static float **y;
  float xMin, xMax, a, b;

  SXVarParam xVarParam;
  static SCurveParam curveParam[2];
  QMessageBox msgBox;

  DeleteFMatrix(x);
  nPoints=ui->pointsEdit->text().toInt();
  x=CreateFMatrix(1,nPoints);
  DeleteFMatrix(y);
  /* Con la seguente definizione creo una matrice a due elementi anche se faccio sempre grafici singoli in modo che quando posso testare l'algoritmo di generazione delle scala verticale destra generando due grafici identici, ma di cui uno sulla scala sinistra, l'altro su quella destra */
  y=CreateFMatrix(2,nPoints);

  curveParam[0].color=Qt::red;
  curveParam[0].rightScale=false;
  curveParam[0].unitS="";
  xVarParam.unitS="0";

  curveParam[1].color=Qt::red;
  curveParam[1].rightScale=true;
/*
  if(YUnitEdit->Text=="")
    curveParam[0].Unit=0;
  else{
    curveParam[0].Unit=YUnitEdit->Text[1];
    lineChart->AutoLabelXY=true;
  }
*/

  try{
      xMin=ui->xMinEdit->text().toFloat();
      xMax=ui->xMaxEdit->text().toFloat();
  }catch(...){
      msgBox.information(this,tr("TestLineChart"),"Numero errato in uno dei campi Min o Max",QMessageBox::Ok,QMessageBox::Ok);
      return;
  }
  try{
    a=ui->constAEdit->text().toFloat();
    b=ui->constBEdit->text().toFloat();
  }catch(...){
    msgBox.setText("Numero errato in uno dei campi \"a\" o \"b\"");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    return;
  }
  if(xMin>=xMax){
    msgBox.setText("Errore, xMin>=xMax");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    return;
  }

  if(ui->yDbScale->isChecked()||ui->yLogScale->isChecked()){
    switch(ui->functionCBox->currentIndex()){
      case -1:
      case 0: //a+b*sin(x)
       if(a+b<=0 || a-b<=0)Error=true;
        break;
      case 1: //a+b*x
        if(a+b*xMin<=0 || a+b*xMax<=0)Error=true;
        break;
      case 2: //a*exp(x/b)
        if(a<=0)Error=true;
        break;
      case 3: //richiesto grafico di y=a*x^b
        if(a<=0)Error=true;
        break;
    }
    if(Error){
      msgBox.information(this,"TestLineChart","Errore, Ymin*Ymax<=0 e scala Y Log o dB",QMessageBox::Ok);
      return;
   }
  }
  for(i=0; i<nPoints; i++){
    x[0][i]=xMin+(xMax-xMin)*i/float(nPoints-1);
  }
  switch(ui->functionCBox->currentIndex()){
    case -1:
    case 0: //a+b*sin(x)
      for(i=0; i<nPoints; i++){
        y[0][i]=a+b*sinf(x[0][i]);
      }
    break;
    case 1: //a+b*x
      for(i=0; i<nPoints; i++){
      y[0][i]=a+b*x[0][i];
      }
    break;
    case 2: //a*exp(x/b)
      if(b==0){
        msgBox.warning(this,"TestLineChart","Errore, b=0",QMessageBox::Ok);
        return;
      }
      for(i=0; i<nPoints; i++){
        y[0][i]=a*expf(x[0][i]/b);
      }
      break;
    case 3: //richiesto grafico di y=a*x^b
      for(i=0; i<nPoints; i++){
        y[0][i]=a*powf(x[0][i],b);
      }
      break;
    case 4: //richiesto grafico della seconda colonna di TestFile.adf rispetto alla prima
      CSimOut *simOut=new CSimOut(nullptr);
      QString ret;
      ret=simOut->loadFromAdfFile("TestFile.adf");
      if(ret!=""){
          msgBox.warning(this,"TestLineChart","Unable to find \"TestFile.adf\"",QMessageBox::Ok);
         return;
      }
      DeleteFMatrix(x);
      nPoints=simOut->numOfPoints;
      x=CreateFMatrix(1,nPoints);
      DeleteFMatrix(y);
      y=CreateFMatrix(2,nPoints);
      for(i=0; i<nPoints; i++){
        x[0][i]=simOut->y[0][i];
        y[0][i]=simOut->y[1][i];
      }
      break;
  }

  float xd[5], yd[5];
  for(i=0; i<5; i++){
    xd[i]=x[0][i];
    yd[i]=y[0][i];
  }

  ui->lineChart->getData(x[0],y,nPoints,nPlots);

  QElapsedTimer timer;
  qint64 elaps;
  float elapsMs;
  QString Msg;
  timer.start();

  ui->lineChart->plot(true);

  ui->scaleTBtn->setEnabled(true);
  ui->dataTBtn->setEnabled(true);

  elaps=timer.nsecsElapsed();
  elapsMs=float(elaps/1.e6);
  Msg=Msg.setNum(elapsMs,'g',3)+ " ms";
  ui->elapsed->setText(Msg);
}

void MainWindow::on_dataTBtn_clicked(bool checked)
{
   int numOfTotPlots=1;
    if(checked){
        ui->lineChart->setActiveDataCurs(1);
        if(numOfTotPlots>1){
            ui->xValueLbl->setVisible(false);
            ui->yValueLbl->setVisible(false);
            ui->interpolateBox->setVisible(false);
        }else{
            ui->xValueLbl->setVisible(true);
            ui->yValueLbl->setVisible(true);
            ui->interpolateBox->setVisible(true);
        }
    }else{
        ui->lineChart->setActiveDataCurs(0);
        ui->xValueLbl->setVisible(false);
        ui->yValueLbl->setVisible(false);
        ui->interpolateBox->setVisible(false);
    }
    ui->diffTBtn->setEnabled(checked);
}


void MainWindow::on_scaleTBtn_clicked()
{
    //Preparazione prima della visualizzazione della scheda:
    myScaleDlg->getInfo(ui->lineChart->giveDispRect(),ui->lineChart->twinScale);
    myScaleDlg->getFullLimits(ui->lineChart->giveFullLimits(),ui->lineChart->cutsLimits);
    myScaleDlg->xUnit=ui->lineChart->givexUnit();
    //Visualizzazione della scheda fino a che non metto dati corretti o faccio Cancel:
    QString ret;
    do{
      int result=myScaleDlg->exec();
      if (result==QDialog::Rejected)return;
      ret=myScaleDlg->validDispRect();
      if(ret!="")  QMessageBox::warning(this," ",ret);
    }while (ret!="");
    ui->lineChart->setDispRect(myScaleDlg->giveDispRect());

    exactMatch=myScaleDlg->giveExactMatch();

    if(myScaleDlg->useUserUnits){
      ui->lineChart->getUserUnits(myScaleDlg->xUnit, myScaleDlg->yUnit, myScaleDlg->ryUnit);
      ui->lineChart->useUserUnits=true;
    }else{
      ui->lineChart->useUserUnits=false;
    }
    ui->lineChart->useBrackets=myScaleDlg->useBrackets;
    ui->lineChart->exactMatch=myScaleDlg->exactMatch;
    ui->lineChart->useSmartUnits=myScaleDlg->useSmartUnits;
    ui->lineChart->plot(false);
//    ui->lineChart->markAll();
}

void MainWindow::on_widthThin_clicked()
{
    ui->lineChart->setPlotPenWidth(pwThin);
}

void MainWindow::on_widthThick_clicked()
{
    ui->lineChart->setPlotPenWidth(pwThick);
}

void MainWindow::on_widthAuto_clicked()
{
    ui->lineChart->setPlotPenWidth(pwAuto);
}


void MainWindow::on_Force0Ckb_clicked()
{
    ui->lineChart->forceYZero=ui->Force0Ckb->checkState();

}

void MainWindow::on_linePlot_clicked()
{
    ui->lineChart->plotType=ptLine;
}

void MainWindow::on_swarmPlot_clicked()
{
    ui->lineChart->plotType=ptSwarm;
}

void MainWindow::on_QtF_clicked()
{
    ui->lineChart->drawType=dtQtF;
}

void MainWindow::on_MC_clicked()
{
    ui->lineChart->drawType=dtMC;
}

void MainWindow::on_McD_clicked()
{
    ui->lineChart->drawType=dtMcD;
}


void MainWindow::on_barPlot_clicked()
{
    ui->lineChart->plotType=ptBar;
}

void MainWindow::on_QtI_clicked()
{
    ui->lineChart->drawType=dtQtI;
}

void MainWindow::on_xDbScale_clicked()
{
    ui->lineChart->setXScaleType(stDB);
}

void MainWindow::on_xLinScale_clicked()
{
    ui->lineChart->setXScaleType(stLin);
}

void MainWindow::on_yLinScale_clicked()
{
    ui->lineChart->setYScaleType(stLin);
}

void MainWindow::on_yDbScale_clicked()
{
    ui->lineChart->setYScaleType(stDB);
}

void MainWindow::on_yLogScale_clicked()
{
    ui->lineChart->setYScaleType(stLog);
}


void MainWindow::on_xLogScale_clicked()
{
    ui->lineChart->setXScaleType(stLog);
}
