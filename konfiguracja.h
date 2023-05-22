#ifndef KONFIGURACJA_H
#define KONFIGURACJA_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class Pomoc;
}

/**
 * @brief Klasa Pomoc reprezentuje okno pomocnicze.
 *
 * Pozwala na interakcję z użytkownikiem w celu konfiguracji połączenia i wysyłania danych do urządzenia.
 */
class Pomoc : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy Pomoc.
     * @param parent Wskaźnik na obiekt rodzica (domyślnie: nullptr)
     */
    explicit Pomoc(QWidget *parent = nullptr);

    /**
     * @brief Funkcja oblicza sumę kontrolną CRC8 na podstawie podanych wartości.
     * @param value1 Wartość 1
     * @param value2 Wartość 2
     * @param value3 Wartość 3
     * @param value4 Wartość 4
     * @return Suma kontrolna CRC8
     */
    uint8_t calculateCRC8(float value1, float value2, int value3, int value4);

    /**
     * @brief Destruktor klasy Pomoc.
     */
    ~Pomoc();

private slots:
    /**
     * @brief Slot wywoływany po wciśnięciu przycisku "ROZŁĄCZ".
     */
    void on_ROZLACZ_clicked();

    /**
     * @brief Slot wywoływany po wciśnięciu przycisku "POŁĄCZ".
     */
    void on_POLACZ_clicked();

    /**
     * @brief Slot wywoływany po wciśnięciu przycisku "SZUKAJ".
     */
    void on_SZUKAJ_clicked();

    /**
     * @brief Slot wywoływany po wciśnięciu przycisku.
     */
    void on_pushButton_clicked();

    /**
     * @brief Slot wywoływany po otrzymaniu danych z portu.
     */
    void readFromPort();

    /**
     * @brief Slot wywoływany po wysłaniu wiadomości do urządzenia.
     * @param message Wiadomość do wysłania
     */
    void sendMessageToDevice(QString message);

private:
    Ui::Pomoc *ui2;
    void addToLogs(QString message);
    void readData(QString receivedString);
    Pomoc *oknoPomocy;
    QSerialPort *device;

signals:
    /**
     * @brief Sygnał emitowany po nawiązaniu połączenia z portem.
     * @param portName Nazwa połączonego portu
     */
    void portConnected(QString portName);
};

#endif // KONFIGURACJA_H
