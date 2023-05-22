#ifndef WYSWIETLANIE_H
#define WYSWIETLANIE_H

#include <QDialog>

namespace Ui {
class wyswietlanie;
}

/**
 * @brief Klasa dialogowa odpowiedzialna za wyświetlanie danych.
 */
class wyswietlanie : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy wyswietlanie.
     * @param parent Wskaźnik na rodzica obiektu.
     */
    explicit wyswietlanie(QWidget *parent = nullptr);

    /**
     * @brief Destruktor klasy wyswietlanie.
     */
    ~wyswietlanie();

    bool Ctem1 =true; // Zmienna oznaczająca, czy tem1 jest zaznaczone
    bool Ctem2=true; // Zmienna oznaczająca, czy tem2 jest zaznaczone
    bool Ctacho=true; // Zmienna oznaczająca, czy tacho jest zaznaczone
    bool Cbelka=true; // Zmienna oznaczająca, czy belka jest zaznaczona
    int zakresX=50; // Zakres osi X na wykresie
    int zakresY=50; // Zakres osi Y na wykresie
    Qt::GlobalColor kolor1; // Kolor dla danych tem1
    Qt::GlobalColor kolor2; // Kolor dla danych tem2
    Qt::PenStyle linia1; // Styl linii dla danych tem1
    Qt::PenStyle linia2; // Styl linii dla danych tem2

    /**
     * @brief Metoda ustawiająca wykres.
     */
    void ustaw_wykres();

private slots:
    /**
     * @brief Slot wywoływany po wciśnięciu przycisku.
     */
    void on_pushButton_clicked();



    /**
     * @brief Slot wywoływany po zmianie stanu pola wyboru Ctem2.
     */
    void on_checkBoxTemp2_clicked();


    /**
     * @brief Slot wywoływany po zmianie stanu pola wyboru Cbelka.
     */
    void on_checkBoxBelka_clicked();

    /**
     * @brief Slot wywoływany po zmianie wartości suwaka osi Y.
     * @param value Nowa wartość suwaka osi Y.
     */
    void on_horizontalSlider_osY_valueChanged(int value);

    /**
     * @brief Slot wywoływany po zmianie stanu pola wyboru Ctacho.
     */
    void on_checkBoxTacho_stateChanged(int arg1);

    /**
     * @brief Slot wywoływany po zmianie stanu pola wyboru Ctem1.
     */
    void on_checkBoxTemp1_stateChanged(int arg1);

    /**
     * @brief Slot wywoływany po zmianie wartości suwaka osi X.
     * @param value Nowa wartość suwaka osi X.
     */
    void on_horizontalSlider_osX_valueChanged(int value);

    void on_horizontalSlider_osX_2_valueChanged(int value);

private:
    Ui::wyswietlanie *ui;
    wyswietlanie *oknoWyswietlania;
};

#endif // WYSWIETLANIE_H
