
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QObject>
#include "konfiguracja.h"
#include "ui_mainwindow.h"
#include "wyswietlanie.h"
#include "qcgaugewidget.h"
#include "qcustomplot.h"
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Klasa MainWindow reprezentuje główne okno aplikacji.
 *
 * Obsługuje różne interakcje z interfejsem użytkownika oraz komunikację z urządzeniami poprzez port szeregowy.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy MainWindow.
     * @param parent Wskaźnik na obiekt rodzica (domyślnie: nullptr)
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor klasy MainWindow.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Slot wywoływany po wciśnięciu akcji "Informacje o programie".
     */
    void on_actionInformacje_o_programie_triggered();

    /**
     * @brief Slot wywoływany po wciśnięciu akcji "Konfiguracja połączenia".
     */
    void on_actionKonfiguracja_polaczenia_triggered();

    /**
     * @brief Slot wywoływany po połączeniu z portem.
     * @param portName Nazwa połączonego portu
     */
    void handlePortConnected(QString portName);

    /**
     * @brief Slot wywoływany po wciśnięciu akcji "Połącz ze skonfigurowanym portem".
     */
    void on_actionPolacz_ze_skonfigurowanym_portem_triggered();

    /**
     * @brief Slot wywoływany po otrzymaniu danych z portu.
     */
    void readFromPort();

    /**
     * @brief Slot wywoływany po wciśnięciu przycisku.
     */
    void on_pushButton_clicked();
    /**
    * @brief Slot wywoływany po zakończeniu edycji pola tekstowego.
    */
    void on_lineEdit_editingFinished();

    /**
    * @brief Slot wywoływany po wciśnięciu akcji "Rozłącz".
    */
    void on_actionRoz_cz_triggered();

    /**
    * @brief Slot wywoływany po wciśnięciu akcji "Zapisz dane do pliku CSV".
    */
    void on_actionZapisz_dane_do_pliku_CSV_triggered();

    /**
    * @brief Slot wywoływany po wciśnięciu akcji "Instrukcja użytkowania".
    */
    void on_actionInstrukcja_u_ytkowania_triggered();

    /**
    * @brief Slot wywoływany po wciśnięciu akcji "Wyswietlanie".
    */
    void on_actionWyswietlanie_triggered();



public:
    int flag1=1;
    int flag2=1;


private:
    Ui::MainWindow *ui;
    Pomoc *oknoPomocy;
    wyswietlanie *oknoWyswietlania;
    QSerialPort *device;
    void readData(QString receivedString);
    QCPBars *gauge;
    QcGaugeWidget * mSpeedGauge;
    QcNeedleItem *mSpeedNeedle;
    QBarSet *set1;
    QBarSeries *series;
    QChart *chart;
    QChartView *chartView;

    QCPAxis *axis2;



};

#endif // MAINWINDOW_H
