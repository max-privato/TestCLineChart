#include "CScaleDlg.h"
#include "ui_CScaleDlg.h"

extern SGlobalVars GV;

CScaleDlg::CScaleDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CScaleDlg){
    ui->setupUi(this);
    myUnitsDlg=new CUnitsDlg(this);
    exactMatch=false;
    useUserUnits=false;
    useSmartUnits=true;
}

void CScaleDlg::getInfo(SFloatRect2 dispRect_, bool twinScale){
    dispRect=dispRect_;
    isTwinScale=twinScale;
    ui->xMinEdit->setText(QString::number(dispRect.Left));
    ui->xMaxEdit->setText(QString::number(dispRect.Right));
    ui->yMinEdit->setText(QString::number(dispRect.LBottom));
    ui->yMaxEdit->setText(QString::number(dispRect.LTop));
    if(twinScale){
      ui->ryGroupBox->setEnabled(true);
      ui->ryMinEdit->setText(QString::number(dispRect.RBottom));
      ui->ryMaxEdit->setText(QString::number(dispRect.RTop));
    }else
        ui->ryGroupBox->setEnabled(false);
}

void CScaleDlg::getFullLimits(SFloatRect2 fullLimits_, bool manageFullLimits_){
  /* I fullLimits sono gli estremi delle scale orizzontale e verticali
   * come derivano dal clcolo prima di qualunque arrotondamento. Solo in casi
   * molto particolari i fullLimits per uno degli assi possono risultare esterni
   * agli assi arrotondati; in tal caso do all'utente la possibilità di fare una
   *  visualizzazione completa del grafico, rinunciando ai numeri tondi sulgli assi.
   * Solo in questo caso, caratterizzato da managefullLimits=true, attivo i radio
   * button per gestire anche questo tipo di visualizzazione speciale.
*/
    fullLimits=fullLimits_;
    managefullLimits=manageFullLimits_;
    if(manageFullLimits_){
      ui->manRadioBtn->setVisible(true);
      ui->flRadioBtn->setVisible(true);
      ui->flRadioBtn->setChecked(true);
      on_flRadioBtn_clicked();
      ui->exaMatchBox->setChecked(true);
      on_exaMatchBox_clicked(true);
    }else{
      ui->manRadioBtn->setVisible(false);
      ui->flRadioBtn->setVisible(false);
    }

}

SFloatRect2 CScaleDlg::giveDispRect(void){
    SFloatRect2 rect;
    rect.Left   =ui->xMinEdit->text().toFloat();
    rect.Right  =ui->xMaxEdit->text().toFloat();
    rect.LBottom=ui->yMinEdit->text().toFloat();
    rect.LTop   =ui->yMaxEdit->text().toFloat();
    rect.RBottom=ui->ryMinEdit->text().toFloat();
    rect.RTop   =ui->ryMaxEdit->text().toFloat();
    return rect;
}

bool CScaleDlg::giveExactMatch(){
  return exactMatch;
}


void CScaleDlg::on_exaMatchBox_clicked(bool checked){
    if(ui->exaMatchBox->isChecked()){
      ui->flRadioBtn->setEnabled(true);
      ui->manRadioBtn->setEnabled(true);
    }else{
      ui->flRadioBtn->setEnabled(false);
      ui->manRadioBtn->setEnabled(false);
    }
    exactMatch=checked;
}


void CScaleDlg::on_unitsBtn_clicked(){
//    myUnitsDlg->setXUnit(xUnit);
    myUnitsDlg->getTwinScales(isTwinScale);
    int result=myUnitsDlg->exec();
    if(result==Accepted&&!myUnitsDlg->doReset){
        xUnit=myUnitsDlg->xUnit;
        yUnit=myUnitsDlg->yUnit;
        ryUnit=myUnitsDlg->ryUnit;
        useUserUnits=true;
        useBrackets=myUnitsDlg->useBrackets;
    }else if(myUnitsDlg->doReset){
        useUserUnits=false;
    }
    useSmartUnits=myUnitsDlg->useSmartUnits;
}

void CScaleDlg::setUseBrackets(bool useBrackets_){
    useBrackets=useBrackets_;
    myUnitsDlg->setUseBrackets(useBrackets);
}

void CScaleDlg::setTwinScale(bool ts){
    isTwinScale=ts;
    ui->ryGroupBox->setEnabled(ts);
}


QString CScaleDlg::validDispRect(){
  SFloatRect2 r=giveDispRect();
  if(r.Left>=r.Right) return "Max value must be larger than Min\non the X-axis";
  if(r.LTop<=r.LBottom) return "Max value must be larger than Min\non the Y-axis";
  if(ui->ryMaxEdit->isEnabled() && r.RTop<=r.RBottom)
    return "Max value must be larger than Min\n"
                "on the right-Y-axis";
  return"";
}

CScaleDlg::~CScaleDlg(){
    delete ui;
}


void CScaleDlg::on_flRadioBtn_clicked(){
  ui->xMinEdit->setText(QString::number(fullLimits.Left));
  ui->xMaxEdit->setText(QString::number(fullLimits.Right));
  ui->yMinEdit->setText(QString::number(fullLimits.LBottom));
  ui->yMaxEdit->setText(QString::number(fullLimits.LTop));
  if(isTwinScale){
    ui->ryMinEdit->setText(QString::number(fullLimits.RBottom));
    ui->ryMaxEdit->setText(QString::number(fullLimits.RTop));
  }

  ui->xMinEdit->setEnabled(false);
  ui->xMaxEdit->setEnabled(false);
  ui->yGroupBox->setEnabled(false);
  ui->ryGroupBox->setEnabled(false);
}

void CScaleDlg::on_manRadioBtn_clicked(){
  ui->xMinEdit->setText(QString::number(dispRect.Left));
  ui->xMaxEdit->setText(QString::number(dispRect.Right));
  ui->yMinEdit->setText(QString::number(dispRect.LBottom));
  ui->yMaxEdit->setText(QString::number(dispRect.LTop));

  ui->xMinEdit->setEnabled(true);
  ui->xMaxEdit->setEnabled(true);
  ui->yGroupBox->setEnabled(true);
  if(isTwinScale){
    ui->ryGroupBox->setEnabled(true);
    ui->ryMinEdit->setText(QString::number(dispRect.RBottom));
    ui->ryMaxEdit->setText(QString::number(dispRect.RTop));
  }
}

void CScaleDlg::showEvent(QShowEvent *){
  if(managefullLimits){
    ui->flRadioBtn->setEnabled(true);
    on_flRadioBtn_clicked();
  }else{
      ui->manRadioBtn->setEnabled(false);
      on_manRadioBtn_clicked();

  }
}






