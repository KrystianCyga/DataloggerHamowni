#include "konfiguracja.h"
#include "ui_pomoc.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QList>
#include <QDateTime>
#include <cmath>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>


/**
 * @brief Konstruktor klasy Pomoc.
 * @param parent Wskaźnik do rodzica okna.
 */
Pomoc::Pomoc(QWidget *parent) :
    QDialog(parent),
    ui2(new Ui::Pomoc)
{
    ui2->setupUi(this);
    this->device = new QSerialPort(this);


}
/**
 * @brief Destruktor klasy Pomoc.
 */
Pomoc::~Pomoc()
{
    delete ui2;
}

/**
 * @brief Obsługuje zdarzenie kliknięcia przycisku ROZLACZ.
 */
void Pomoc::on_ROZLACZ_clicked()
{
    if(this->device->isOpen()) {
    this->device->close();
    this->addToLogs("Zamknięto połączenie.");
    } else {
    this->addToLogs("Port nie jest otwarty!");
    return;
    }
}

/**
 * @brief Obsługuje zdarzenie kliknięcia przycisku POLACZ.
 */
void Pomoc::on_POLACZ_clicked()
{
    if(ui2->COMBOX->count() == 0) {
      this->addToLogs("Nie wykryto żadnych urządzeń!");
      return;
    }

    QString portName = ui2->COMBOX->currentText().split("\t").first();

    // Sprawdź, czy port już istnieje na liście
    QStringList portList;
    QFile file("devices.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            portList.append(line);
        }
        file.close();
    }
    if (portList.contains(portName)) {
        this->addToLogs("Port " + portName + " już istnieje na liście.");

    }

    this->device->setPortName(portName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(device->open(QSerialPort::ReadWrite)) {
      this->device->setBaudRate(QSerialPort::Baud9600);
      this->device->setDataBits(QSerialPort::Data8);
      this->device->setParity(QSerialPort::NoParity);
      this->device->setStopBits(QSerialPort::OneStop);
      this->device->setFlowControl(QSerialPort::NoFlowControl);

      // Otwórz plik i dopisz informacje o urządzeniu
      if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
          out << portName << Qt::endl;
        file.close();
      }

      this->addToLogs("Otwarto port szeregowy.");
      connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
      emit portConnected(portName);
    } else {
      this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
    }
}



/**
 * @brief Obsługuje zdarzenie kliknięcia przycisku SZUKAJ.
 */
void Pomoc::on_SZUKAJ_clicked()
{
    this->addToLogs("Szukam urządzeń...");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();

    for(int i = 0; i < devices.count(); i++) {
      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
      ui2->COMBOX->addItem(devices.at(i).portName() + "\t" + devices.at(i).description());

    }
}


void Pomoc::on_pushButton_clicked()
{
    if(this->device->isOpen()) {
      this->device->close();
      this->addToLogs("Zamknięto połączenie.");
    } else {
      this->addToLogs("Port nie jest otwarty!");
      close();
      return;
    }
    close();
}
/**
 * @brief Dodaje wiadomość do logów.
 * @param message Wiadomość do dodania.
 */
void Pomoc::addToLogs(QString message)
{
        QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
        ui2->textLogs->append(currentDateTime + "\t" + message);
}

/**
 * @brief Odczytuje dane z portu szeregowego.
 */
void Pomoc::readFromPort() {
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

/**
 * @brief Odczytuje i przetwarza otrzymane dane.
 * @param receivedString Odebrany ciąg znaków.
 */
void Pomoc::readData(QString receivedString) {


    this->addToLogs("Odebrane dane: " + receivedString); // Wyświetlenie odebranych danych w konsoli

        QStringList dataList = receivedString.split(" ");

        if (dataList.size() < 5) { // Sprawdzenie, czy otrzymano wystarczającą ilość danych
      this->addToLogs("Błąd: Nieprawidłowa ramka danych");
      return;
        }

        float temperature = dataList[0].toFloat(); // Wartość temperatury
        int temp2 = dataList[1].toInt(); // Druga wartość temperatury
        float tacho = dataList[2].toFloat();// Wartość sygnału TCRT5000
        int belka = dataList[3].toInt();
        int crc = dataList[4].toInt();


        int calculatedCrc = oknoPomocy->calculateCRC8(temperature,temp2,tacho,belka); // Obliczenie sumy kontrolnej dla odebranej temperatury

    if (std::round(crc*100)/100 == std::round(calculatedCrc*100)/100) { // Sprawdzenie poprawności sumy kontrolnej
        this->addToLogs("Suma kontrolna poprawna");
        this->addToLogs("Temperatura:" + dataList[0]);
        this->addToLogs("Temperatura2:" + dataList[2]);
        this->addToLogs("RPM:" + dataList[4]);
        this->addToLogs("Wskazanie belki:" + dataList[6]);
    } else {
        this->addToLogs("Błąd sumy kontrolnej");
        this->addToLogs("Suma Obliczona:" + QString::number(calculatedCrc));
        this->addToLogs("Suma Otrzymana:" + QString::number(crc));
    }
}

/**
 * @brief Wysyła wiadomość do urządzenia.
 * @param message Wiadomość do wysłania.
 */
void Pomoc::sendMessageToDevice(QString message) {
  if(this->device->isOpen() && this->device->isWritable()) {
    this->addToLogs("Wysyłam informacje do urządzenia " + message);
    this->device->write(message.toStdString().c_str());
  } else {
    this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty!");
  }
}

/**
 * @brief Oblicza sumę kontrolną CRC-8 dla wartości przekazanych jako argumenty.
 * @param value1 Pierwsza wartość do obliczenia sumy kontrolnej.
 * @param value2 Druga wartość do obliczenia sumy kontrolnej.
 * @param value3 Trzecia wartość do obliczenia sumy kontrolnej.
 * @param value4 Czwarta wartość do obliczenia sumy kontrolnej.
 * @return Suma kontrolna CRC-8 obliczona dla wartości.
 */
uint8_t Pomoc::calculateCRC8(float value1, float value2, int value3, int value4) {
  uint8_t crc = 0;

  // Obliczenie sumy kontrolnej dla value1
  uint32_t valueAsUint1;
  memcpy(&valueAsUint1, &value1, sizeof(value1));
  uint8_t* bytes1 = (uint8_t*)&valueAsUint1;
  for (uint8_t i = 0; i < sizeof(float); i++) {
    crc ^= bytes1[i];
    for (uint8_t j = 0; j < 8; j++) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0x07;
        } else {
            crc <<= 1;
        }
    }
  }

  // Obliczenie sumy kontrolnej dla value2
  uint32_t valueAsUint2;
  memcpy(&valueAsUint2, &value2, sizeof(value2));
  uint8_t* bytes2 = (uint8_t*)&valueAsUint2;
  for (uint8_t i = 0; i < sizeof(float); i++) {
    crc ^= bytes2[i];
    for (uint8_t j = 0; j < 8; j++) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0x07;
        } else {
            crc <<= 1;
        }
    }
  }

  // Obliczenie sumy kontrolnej dla value3
  uint8_t value3AsUint8 = (uint8_t)value3;
  crc ^= value3AsUint8;
  for (uint8_t i = 0; i < 8; i++) {
    if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07;
    } else {
        crc <<= 1;
    }
  }

  // Obliczenie sumy kontrolnej dla value4
  uint8_t value4AsUint8 = (uint8_t)value4;
  crc ^= value4AsUint8;
  for (uint8_t i = 0; i < 8; i++) {
    if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07;
    } else {
        crc <<= 1;
    }
  }

  return crc;
}



