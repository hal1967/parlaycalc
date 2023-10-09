#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need an alignmentGroup in order to transfort Menu's checkbox
    // into radio button. This is strange for me, but "this is the way".

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(ui->actionAmerican);
    alignmentGroup->addAction(ui->actionDecimal);
    alignmentGroup->addAction(ui->actionFractional);

    // Default en american format -120... +400. .. etc
    ui->actionAmerican->setChecked(true);

    // showMessage, does the work, bet could be deleted by any other widget, like menu...
    // statusLabel is more specific and it has some persistence
    //ui->statusbar->showMessage("American notation");

    statusLabel = new QLabel("American notation", this);   // Creating a label to be indluded inside the statusbar
    ui->statusbar->addPermanentWidget(statusLabel); // Now belongs to the statusbar

    betvalidator = new QDoubleValidator(0.0, 100000.0, 2); // with 2 decimals of precision
    ui->edwagger->setValidator(betvalidator);

    // Created in memory but unused until select decimal mode
    decmodevalidator = new QDoubleValidator(1.0, 100000.0, 2); // Minimal 1, with 2 decimals of precision

    amermodevalidator= new QDoubleValidator(); // No range, but decimals
    amermodevalidator->setDecimals(2);
    ui->edOdd->setValidator(amermodevalidator);


    // setdecimalvalidator() we need code the "connect"
    // on_setdecimalvalidator()  with this name connect is automatic (https://doc.qt.io/qt-5/qobject.html#auto-connection)
    connect(ui->actionDecimal, SIGNAL(triggered()), this, SLOT(setdecimalvalidator()));

}

MainWindow::~MainWindow()
{
    // alignmentGroup, dv, decmode and statusLabel belong to MainWindow

    delete decmodevalidator;  // This validator has not parent, so we need to dispose here
    delete betvalidator;       // idem decmode
    delete ui;
}

// TODO
// We need to validated a lot of diferents circunsatances.
// for example:
//      0 or negativa wagger
//      0 or empty coeficient could't be accepted to the list
float MainWindow::calcfactorodd(float wagger, float odd)
{
    if (ui->actionAmerican->isChecked()) {

       // The american, way supposed positive and negative values.
       // +180 (american) is 2.8 decimal.  For this; american value is divide by 100 + adding 1. 180/100 + 1 -> 2.8.
       // but, negative values like -120 are diferent  1 + 100/(American Value), so   1 - 100/(-120) ->  1.833

        if (odd>=0) {
            return wagger*(1+odd/100);
        } else {
            return wagger*(1-100/odd);
        }
    } else if (ui->actionDecimal->isChecked()) {
        return wagger*odd;
    }

}

// on_btnAdd_clicked must call something like DoCalculation as on_edOdd_returnPressed() must
// but is interesting than call one slot from other one.

void MainWindow::on_btnAdd_clicked()
{
    QString sodd, swagger, srst;
    float odd, wagger, rst;

    // this could be
    // ui->edOdd->text().ToFloat()

    sodd = ui->edOdd->text();
    odd = sodd.toFloat();

    swagger = ui->edwagger->text();
    wagger = swagger.toFloat();

    rst = calcfactorodd(wagger, odd); // 1er edwagger

    QListWidgetItem* item;
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        item = ui->listWidget->item(i);
        rst = calcfactorodd(rst, item->text().toFloat());
    }

    srst.setNum(rst);
    ui->listWidget->addItem( sodd );

    ui->lblResult->setText(srst);

    ui->edOdd->clear();
}


void MainWindow::on_btnClear_clicked()
{
    ui->listWidget->clear();
    ui->edOdd->clear();
    ui->edwagger->clear();
    ui->lblResult->clear();
}


void MainWindow::on_edOdd_textChanged(const QString &arg1)
{
    ui->btnAdd->setEnabled(! arg1.isEmpty());
    ui->btnClear->setEnabled((ui->listWidget->count()>0) ||
                             (!arg1.isEmpty()) );
}


void MainWindow::on_edwagger_returnPressed()
{
    ui->edOdd->setFocus(); // doesn't work ????
}


void MainWindow::on_edOdd_returnPressed()
{
    on_btnAdd_clicked();
}


void MainWindow::setdecimalvalidator()
{
    ui->edOdd->setValidator(decmodevalidator);
    ui->edOdd->setPlaceholderText("like 1.2 or 0.4");
    // ui->statusbar->showMessage("Decimal notation");
    statusLabel->setText("Decimal notation");
}


void MainWindow::on_actionAmerican_triggered()
{
    ui->edOdd->setValidator(amermodevalidator);
    ui->edOdd->setPlaceholderText("like -120");
    // ui->statusbar->showMessage("American notation");
    statusLabel->setText("American notation");

}

