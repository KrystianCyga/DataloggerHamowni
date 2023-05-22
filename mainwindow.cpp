#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "konfiguracja.h"
#include "wyswietlanie.h"
#include <QtSerialPort>
#include <QDebug>
#include <QList>
#include <QDateTime>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>





/**
 * @brief Konstruktor klasy MainWindow.
 * @param parent Wskaźnik na rodzica obiektu.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicjalizacja QSerialPort
    this->device = new QSerialPort(this);

    // Inicjalizacja okna Pomoc
    oknoPomocy = new Pomoc(this);

    // Inicjalizacja okna wyswietlanie
    oknoWyswietlania = new wyswietlanie(this);

    // Połączenie sygnału portConnected z odpowiednim slotem
    QObject::connect(oknoPomocy, SIGNAL(portConnected(QString)), this, SLOT(handlePortConnected(QString)));
    QCPAxis *axis2 = new QCPAxis(ui->customPlot->yAxis->axisRect(), QCPAxis::atRight);

    // Dodanie wykresu do customPlot
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();

    // Ustawienie etykiety dla osi X
    ui->customPlot->xAxis->setLabel("CZAS");

    // Ustawienie zakresu dla osi Y
    ui->customPlot->yAxis->setRange(0, oknoWyswietlania->zakresY);

    // Dodanie drugiej osi Y (axis2)

    ui->customPlot->axisRect()->addAxis(QCPAxis::atRight)->setVisible(true);

    // Dodanie legendy
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    // Ustawienie nazw dla wykresów
    ui->customPlot->graph(1)->setName("Temperatura sterownika");
    ui->customPlot->graph(0)->setName("Temperatura silnika");

    // Ustawienie zakresu dla axis2
    axis2->setRange(0, oknoWyswietlania->zakresY);

    // Inicjalizacja QcGaugeWidget i dodawanie elementów do wykresu
    mSpeedGauge = new QcGaugeWidget;
    mSpeedGauge->addBackground(99);
    QcBackgroundItem *bkg1 = mSpeedGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);
    QcBackgroundItem *bkg2 = mSpeedGauge->addBackground(88);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);
    mSpeedGauge->addArc(55);
    mSpeedGauge->addDegrees(65)->setValueRange(0,80);
    mSpeedGauge->addColorBand(50);
    mSpeedGauge->addValues(80)->setValueRange(0,80);
    mSpeedGauge->addLabel(70)->setText("Km/h");
    QcLabelItem *lab = mSpeedGauge->addLabel(40);
    lab->setText("0");
    mSpeedNeedle = mSpeedGauge->addNeedle(60);
    mSpeedNeedle->setLabel(lab);
    mSpeedNeedle->setColor(Qt::white);
    mSpeedNeedle->setValueRange(0,80);
    mSpeedGauge->addBackground(7);
    mSpeedGauge->addGlass(88);
    ui->verticalLayout->addWidget(mSpeedGauge);
}


MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Obsługa kliknięcia przycisku.
 * Ustawia wykres na podstawie wybranych opcji z okna wyswietlania.
 * Ustawia zakres osi Y na wykresie.
 * Ustawia flagi flag1 i flag2 w zależności od stanu checkboxów w oknie wyswietlania.
 */
void MainWindow::on_pushButton_clicked()
{
    oknoWyswietlania->ustaw_wykres();\

        ui->customPlot->yAxis->setRange(0, oknoWyswietlania->zakresY);



    if(oknoWyswietlania->Ctacho==true){
        flag1=1;
    }else
    {
        flag1=0;
    }

    if(oknoWyswietlania->Cbelka==true){
        flag2=1;
    }else
    {
        flag2=0;
    }

}

/**
 * @brief Obsługa podłączenia portu.
 * Ustawia nazwę portu dla obiektu device.
 * @param portName Nazwa portu.
 */
void MainWindow::handlePortConnected(QString portName) {
    this->device->setPortName(portName);
}

/**
 * @brief Obsługa akcji Informacje o programie.
 * Wyświetla okno dialogowe z informacjami o programie.
 */
void MainWindow::on_actionInformacje_o_programie_triggered()
{
    QMessageBox::information(this,"Informacje o programie","W tym oknie znajduje się wykres, który wyświetla dane pomiarowe pobierane z połączonego z komputerem urządzenia przez port szeregowy. Okno ma również wbudowany wskaźnik prędkości, który wskazuje wartość na skalowanej osi. Program próbuje nawiązać połączenie z urządzeniem, odczytuje dane z portu szeregowego i wyświetla je na wykresie. Wykres ma dwie linie: jedna pokazuje temperaturę, a druga wartość pomiaru z sensora TCRT5000. Połączenie z urządzeniem można skonfigurować za pomocą okna konfiguracji połączenia, do którego można przejść za pomocą menu w programie.");

}

/**
 * @brief Obsługa akcji Konfiguracja połączenia.
 * Wyświetla okno pomocy.
 */
void MainWindow::on_actionKonfiguracja_polaczenia_triggered()
{
    oknoPomocy->show();
}



/**
 * @brief Slot obsługujący akcję polaczenia ze skonfigurowanym portem.
 * Sprawdza zawartość pliku "devices.txt" i próbuje nawiązać połączenie z każdym urządzeniem z listy.
 */
void MainWindow::on_actionPolacz_ze_skonfigurowanym_portem_triggered()
{
    // Wczytaj zawartość pliku "devices.txt"
    QStringList deviceList;
    QFile file("devices.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            deviceList.append(line);
        }
        file.close();
    }

    // Spróbuj nawiązać połączenie z każdym urządzeniem z listy
    foreach(QString portName, deviceList) {
        this->device->setPortName(portName);
        if (device->open(QSerialPort::ReadWrite)) {
            this->device->setBaudRate(QSerialPort::Baud9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);
            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
            qDebug()<<"Połączono z urządzeniem " << portName;
                break; // Przerwij pętlę, gdy uda się połączyć z urządzeniem
        }
    }
}



/**
 * @brief Slot obsługujący odczyt danych z portu szeregowego.
 * Aktualizuje wykresy w oknie wyswietlania i przetwarza odebrane dane.
 */
void MainWindow::readFromPort() {
    oknoWyswietlania->ustaw_wykres();

    QString startMarker = "@";
    QString endMarker = "$";
    QString receivedString;

    while (this->device->canReadLine()) {
        QString line = this->device->readLine();
        receivedString += line;

        int startIndex = receivedString.indexOf(startMarker);
        int endIndex = receivedString.indexOf(endMarker);

        if (startIndex != -1 && endIndex != -1) {
                receivedString = receivedString.mid(startIndex + 1, endIndex - startIndex - 1);
                readData(receivedString);
                receivedString.clear();
        }
    }
}

float temperatureArray[1000];
int tcrtArray[1000];
int dataCounter = 0;

/**
 * @brief Przetwarza otrzymane dane i aktualizuje wykresy.
 * @param receivedString Odebrane dane w formacie QString.
 */
void MainWindow::readData(QString receivedString) {
    dataCounter = 0;
    static QDateTime programStart = QDateTime::currentDateTime();
    int elapsed = programStart.msecsTo(QDateTime::currentDateTime());

    qDebug() <<"Odebrane dane: " + receivedString; // Wyświetlenie odebranych danych w konsoli

    QStringList dataList = receivedString.split(" ");

    if (dataList.size() < 5) { // Sprawdzenie, czy otrzymano wystarczającą ilość danych
        qDebug() <<"Błąd: Nieprawidłowa ramka danych";
        return;
    }

    float temperature = dataList[0].toFloat(); // Wartość temperatury
    float temp2 = dataList[1].toFloat(); // Druga wartość temperatury
    float tacho = dataList[2].toFloat(); // Wartość sygnału TCRT5000
    int belka = dataList[3].toInt();
    int crc = dataList[4].toInt();

    int calculatedCrc = oknoPomocy->calculateCRC8(temperature, temp2, tacho, belka); // Obliczenie sumy kontrolnej dla odebranej temperatury

    qDebug() << temperature << " " << temp2 << " " << tacho << " " << belka << "\n";

    //if (std::round(crc*100)/100 == std::round(calculatedCrc*100)/100) { // Sprawdzenie poprawności sumy kontrolnej //crcTemp1 ==calculatedCrc
    if(1) {
        temperatureArray[dataCounter] = temperature;
        tcrtArray[dataCounter] = temp2;
        dataCounter++;

        if(oknoWyswietlania->Ctem1==true){

        ui->customPlot->graph(0)->addData(elapsed / 1000, temperature);
        }

        // Dodanie drugiego wykresu dla tcrt
        ui->customPlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));

        if(oknoWyswietlania->Ctem2==true){
        ui->customPlot->graph(1)->addData(elapsed / 1000, temp2);
        }


        ui->customPlot->xAxis->setRange(elapsed / 1000 - oknoWyswietlania->zakresX, elapsed / 1000);
        qDebug() <<oknoWyswietlania->zakresX<<" "<<oknoWyswietlania->zakresY;
        ui->customPlot->graph(0)->setPen(QPen(oknoWyswietlania->kolor1, 2, oknoWyswietlania->linia1));
        ui->customPlot->graph(1)->setPen(QPen(oknoWyswietlania->kolor2, 2, oknoWyswietlania->linia2));

        ui->customPlot->axisRect()->axis(QCPAxis::atRight, 0)->setLayer("axes");
        ui->customPlot->axisRect()->axis(QCPAxis::atRight, 0)->setLabel("TCRT5000");
        ui->customPlot->axisRect()->axis(QCPAxis::atRight, 0)->setRange(0, 100);

        ui->customPlot->replot();

        ui->Bar->setValue(belka * flag2);

        mSpeedNeedle->setCurrentValue(tacho * flag1);
    } else {
        qDebug() <<"Błąd sumy kontrolnej";
        qDebug() <<"Suma Obliczona:" + QString::number(calculatedCrc);
        qDebug() <<"Suma Otrzymana:" + QString::number(crc);
    }
}





void MainWindow::on_actionRoz_cz_triggered()
{
    if(this->device->isOpen()) {
        this->device->close();
        qDebug() <<"Zamknięto połączenie.";
    } else {
        qDebug() <<"Port nie jest otwarty!";
        return;
    }
}


void MainWindow::on_lineEdit_editingFinished()
{

}

/**
 * @brief Zapisuje dane do pliku CSV.
 */
void MainWindow::on_actionZapisz_dane_do_pliku_CSV_triggered()
{
    // Otwarcie pliku do zapisu
    QFile file("dane.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku do zapisu";
        return;
    }

    // Utworzenie obiektu QTextStream
    QTextStream out(&file);

    for (int i = 0; i < dataCounter; ++i) {


    // Zapisanie wartości do pliku CSV
    out << i << ","
        << temperatureArray[i] << ","
        << tcrtArray[i] << "\n";
    }

    // Zamknięcie pliku
    file.close();
    qDebug() << "Zapisano dane do pliku CSV";

}


void MainWindow::on_actionInstrukcja_u_ytkowania_triggered()
{
QMessageBox::information(this,"Instrukcja użytkowania","-Pierwszym krokiem jest upewnienie się, że wszystkie czujniki podłączone są do dataloggera. \n-Następnie należy podłączyć datalogger za pomocą kabla do komputera.\n-W następnym kroku trzeba upewnić się, że nic nie naciska na ramię hamulca mierzące siłę.\n-Po uruchomieniu programu należy skonfigurować port wchodząc do zakładki 'opcje' i 'konfiguracja połączenia'.\n-Po pomyslnym połączeniu należy zamknąć okno konfiguracji i połączyć się ze skonfigurowanym portem. \n-Port automatycznie zapisze się w pliku tekstowym. \n-Dane można zapisać do pliku CSV za pomocą przycisku. ");
}


void MainWindow::on_actionWyswietlanie_triggered()
{
    oknoWyswietlania->show();
}



