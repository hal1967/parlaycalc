#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QAction>
#include <QDoubleValidator>
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnClear_clicked();
    void on_edOdd_textChanged(const QString &arg1);
    void on_edwagger_returnPressed();
    void on_edOdd_returnPressed();
    void setdecimalvalidator();
    void on_actionAmerican_triggered();

private:
    Ui::MainWindow *ui;
    QActionGroup *alignmentGroup;
    QDoubleValidator *betvalidator; // For bet
    QDoubleValidator *decmodevalidator; // For each event in decimal mode
    QDoubleValidator *amermodevalidator; // For each event in american mode

    QLabel *statusLabel;


    float calcfactorodd(float wagger, float odd);
};
#endif // MAINWINDOW_H
