#include "mainwindow.h"
#include "ui_mainwindow.h"
const double pi=3.14159265358979; // Число Пи
const double ro=206264.8062; // Число угловых секунд в радиане

// Эллипсоид Красовского
const double  aP = 6378245; // Большая полуось
const double alP = 1 / 298.3; // Сжатие
const double e2P = 2 * alP - pow(alP,2); // Квадрат эксцентриситета

// Эллипсоид WGS84 (GRS80, эти два эллипсоида сходны по большинству параметров)
const double aW = 6378137; // Большая полуось
const double alW = 1 / 298.257223563; // Сжатие
const double e2W = 2 * alW - pow(alW,2); // Квадрат эксцентриситета

// Вспомогательные значения для преобразования эллипсоидов
const double a  = (aP + aW) / 2;
const double e2  = (e2P + e2W) / 2;
const double da   = aW - aP;
const double de2 = e2W - e2P;

// Линейные элементы трансформирования, в метрах
const double  dx = 23.92;
const double dy = -141.27;
const double dz = -80.9;

// Угловые элементы трансформирования, в секундах
const double wx = 0;
const double wy = 0;
const double wz = 0;

//Дифференциальное различие масштабов
const double ms = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   db=QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("./testDB.db");
   if(db.open()){

       qDebug("open");

   }
   else{
       qDebug("no open");
   }
   query=new QSqlQuery(db);

   query->exec("CREATE TABLE CK42WGS84(CK42 INT, WGS84 INT);");
   model=new QSqlTableModel(this,db);
   model-> setTable("CK42WGS84");
   model->select();

   ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//WGS84_SK42_Lat_Long
void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->insertPlainText((" *************************** "));
    gr=ui->wgsgr->text().toInt();
    min=ui->wgsmin->text().toInt();
    sec=ui->wgssec->text().toInt();
    ui->textBrowser->insertPlainText((" WGS84_SK42_Lat:"));
    ui->textBrowser->insertPlainText(QString::number(WGS84_SK42_Lat(gr,min,sec)));
   ui->textBrowser->insertPlainText(("      "));
    ui->textBrowser->insertPlainText((" WGS84_SK42_Lat: "));
    ui->textBrowser->insertPlainText(QString::number(WGS84_SK42_Long(gr,min,sec)));
    model->insertRow(model->rowCount());
}

//SK42_WGS84_Lat_Long
void MainWindow::on_btnrun_clicked()
{
    ui->textBrowser->insertPlainText((" *************************** "));
    gr=ui->ckgr->text().toInt();
    min=ui->ckmin->text().toInt();
    sec=ui->cksec->text().toInt();
ui->textBrowser->insertPlainText((" SK42_WGS84_Lat: "));
ui->textBrowser->insertPlainText(QString::number(SK42_WGS84_Lat(gr,min,sec)));
  ui->textBrowser->insertPlainText(("      "));
ui->textBrowser->insertPlainText((" SK42_WGS84_Long: "));
ui->textBrowser->insertPlainText(QString::number(SK42_WGS84_Long(gr,min,sec)));
model->insertRow(model->rowCount());

}


void MainWindow::on_btnclr_clicked()
{
    ui->ckgr->clear();
    ui->ckmin->clear();
    ui->cksec->clear();
    ui->wgsgr->clear();
    ui->wgsmin->clear();
    ui->wgssec->clear();
    ui->textBrowser->clear();
}


void MainWindow::on_btndelete_clicked()
{
    model->removeRow(row);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
     row=index.row();
}


double MainWindow::WGS84_SK42_Lat(double Bd, double Ld, double H)
{
    return Bd - dB(Bd, Ld, H) / 3600;
    qDebug("process run");
}

double MainWindow::SK42_WGS84_Lat(double Bd, double Ld, double H)
{
  return  Bd + dB(Bd, Ld, H) / 3600;
}

double MainWindow::WGS84_SK42_Long(double Bd, double Ld, double H)
{
    return Ld - dL(Bd, Ld, H) / 3600;
}

double MainWindow::SK42_WGS84_Long(double Bd, double Ld, double H)
{
    return Ld + dL(Bd, Ld, H) / 3600;
}


double MainWindow::dB(double Bd, double Ld, double H)
{
    double B, L, M, N;
       B = Bd * pi / 180;
       L = Ld * pi / 180;
       M = a * (1 - e2) /pow((1 - e2 *pow(sin(B),2)), 1.5);
       N = a * pow((1 - e2 *pow(sin(B),2)), -0.5);
      return ro / (M + H) * (N / a * e2 * sin(B) * cos(B) * da + (pow(N,2)/pow( a , 2 + 1) * N * sin(B) * cos(B) * de2 / 2 - (dx * cos(L) + dy * sin(L)) * sin(B) + dz * cos(B)) - wx * sin(L) * (1 + e2 * cos(2 * B)) + wy * cos(L) * (1 + e2 * cos(2 * B))  - ro * ms * e2 * sin(B) * cos(B));

}

double MainWindow::dL(double Bd, double Ld, double H)
{
    double B, L, N;
       B = Bd *pi / 180;
       L = Ld * pi / 180;
       N = a * pow((1 - e2 *pow(sin(B), 2)), -0.5);
       return ro / ((N + H) * cos(B)) * (-dx * sin(L) + dy * cos(L)) + tan(B) * (1 - e2) * (wx * cos(L) + wy * sin(L)) - wz;

}

double MainWindow::WGS84Alt(double Bd, double Ld, double H)
{
    double B, L, N, dH;
        B = Bd * pi / 180;
        L = Ld * pi / 180;
        N = a * pow((1 - e2 * pow(sin(B),2)),-0.5);
        dH = -a / N * da + N *pow( sin(B), 2)* de2 / 2 + (dx * cos(L) + dy * sin(L)) * cos(B) + dz * sin(B) - N * e2 * sin(B) * cos(B) * (wx / ro * sin(L) - wy / ro * cos(L)) + (pow(a,2 )/ N + H) * ms;
       return H + dH;


}
/*
void MainWindow::addvalue(double ck42, double wgs84)
{
    query->bindValue(":CK42",  ck42);
               query->bindValue(":WGS84", wgs84);
                }
*/

