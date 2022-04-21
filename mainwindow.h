#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>

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
    void on_pushButton_clicked();

    void on_btnrun_clicked();

    void on_btnclr_clicked();

    void on_btndelete_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery * query;
    QSqlTableModel *model;
    int row;
    double  gr;
    double  min;
   double sec;


    double WGS84_SK42_Lat(double Bd, double Ld, double H);
    double SK42_WGS84_Lat(double Bd, double Ld, double H);
    double WGS84_SK42_Long(double Bd, double Ld, double H);
    double SK42_WGS84_Long(double Bd, double Ld, double H);
    double dB(double Bd, double Ld, double H);
    double dL(double Bd, double Ld, double H);
    double WGS84Alt(double Bd, double Ld, double H);
    void addvalue(double ck42, double wgs84);
};
#endif // MAINWINDOW_H
