#include "wyswietlanie.h"
#include "ui_wyswietlanie.h"


/**
 * @brief Konstruktor klasy wyswietlanie.
 * @param parent Wskaźnik na obiekt rodzica.
 */
wyswietlanie::wyswietlanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wyswietlanie)
{
    ui->setupUi(this);

    // Dodaj elementy do combobox dla koloru T1
    ui->comboBoxKolorT1->addItem("Czerwona");
    ui->comboBoxKolorT1->addItem("Zielona");
    ui->comboBoxKolorT1->addItem("Niebieska");
    ui->comboBoxKolorT1->addItem("Czarna");
    ui->comboBoxKolorT1->addItem("Szara");

    // Dodaj elementy do combobox dla koloru T2
    ui->comboBoxKolorT2->addItem("Zielona");
    ui->comboBoxKolorT2->addItem("Czerwona");
    ui->comboBoxKolorT2->addItem("Niebieska");
    ui->comboBoxKolorT2->addItem("Czarna");
    ui->comboBoxKolorT2->addItem("Szara");

    // Dodaj elementy do combobox dla linii T1
    ui->comboBoxLiniaT1->addItem("Ciągła");
    ui->comboBoxLiniaT1->addItem("Przerywana");
    ui->comboBoxLiniaT1->addItem("Kropkowana");
    ui->comboBoxLiniaT1->addItem("KropkowanaPrzerywana");

    // Dodaj elementy do combobox dla linii T2
    ui->comboBoxLiniaT2->addItem("Ciągła");
    ui->comboBoxLiniaT2->addItem("Przerywana");
    ui->comboBoxLiniaT2->addItem("Kropkowana");
    ui->comboBoxLiniaT2->addItem("KropkowanaPrzerywana");
}

/**
 * @brief Destruktor klasy wyswietlanie.
 */
wyswietlanie::~wyswietlanie()
{
    delete ui;
}

/**
 * @brief Obsługa kliknięcia przycisku.
 * Zamyka okno.
 */
void wyswietlanie::on_pushButton_clicked()
{
    close();
}



/**
 * @brief Obsługa zaznaczenia/odznaczenia checkboxa Temp2.
 * Ustawia wartość flagi Ctem2 na true lub false w zależności od stanu checkboxa.
 */
void wyswietlanie::on_checkBoxTemp2_clicked()
{
    if(ui->checkBoxTemp2->isChecked()){
        Ctem2=true;
    }
    else
    {
        Ctem2=false;
    }
}


/**
 * @brief Obsługa zaznaczenia/odznaczenia checkboxa Belka.
 * Ustawia wartość flagi Cbelka na true lub false w zależności od stanu checkboxa.
 */
void wyswietlanie::on_checkBoxBelka_clicked()
{
    if(ui->checkBoxBelka->isChecked()){
        Cbelka=true;
    }
    else
    {
        Cbelka=false;
    }
}

/**
 * @brief Obsługa zmiany wartości suwaka osi Y.
 * Ustawia wartość zmiennej zakresY na wartość suwaka.
 * @param value Wartość suwaka osi Y.
 */
void wyswietlanie::on_horizontalSlider_osY_valueChanged(int value)
{
    zakresX=value;
}


/**
 * @brief Ustawia wykres na podstawie wybranych opcji.
 * Ustala kolory i style linii dla serii danych na wykresie.
 */
void  wyswietlanie::ustaw_wykres(){

    if(ui->comboBoxKolorT1->currentText()=="Czerwona")
    {
        kolor1=Qt::red;
    }
    else if(ui->comboBoxKolorT1->currentText()=="Czarna"){
        kolor1=Qt::black;
    }
    else if(ui->comboBoxKolorT1->currentText()=="Niebieska"){
        kolor1=Qt::blue;
    }
    else if(ui->comboBoxKolorT1->currentText()=="Zielona"){
        kolor1=Qt::green;
    }
    else
    {
        kolor1=Qt::gray;
    }

    if(ui->comboBoxKolorT2->currentText()=="Czerwona")
    {
        kolor2=Qt::red;
    }
    else if(ui->comboBoxKolorT2->currentText()=="Czarna"){
        kolor2=Qt::black;
    }
    else if(ui->comboBoxKolorT2->currentText()=="Niebieska"){
        kolor2=Qt::blue;
    }
    else if(ui->comboBoxKolorT2->currentText()=="Zielona"){
        kolor2=Qt::green;
    }
    else
    {
        kolor2=Qt::gray;
    }

    if(ui->comboBoxLiniaT1->currentText()=="Kropkowana")
    {
        linia1=Qt::DotLine;
    }
    else if(ui->comboBoxLiniaT1->currentText()=="Przerywana"){
        linia1=Qt::DashLine;
    }
    else if(ui->comboBoxLiniaT1->currentText()=="KropkowanaPrzerywana"){
        linia1=Qt::DashDotLine;
    }
    else
    {
        linia1=Qt::SolidLine;
    }

    if(ui->comboBoxLiniaT2->currentText()=="Kropkowana")
    {
        linia2=Qt::DotLine;
    }
    else if(ui->comboBoxLiniaT2->currentText()=="Przerywana"){
        linia2=Qt::DashLine;
    }
    else if(ui->comboBoxLiniaT2->currentText()=="KropkowanaPrzerywana"){
        linia2=Qt::DashDotLine;
    }
    else
    {
        linia2=Qt::SolidLine;
    }



}



/**
 * @brief Obsługa zaznaczenia/odznaczenia checkboxa Tacho.
 * Ustawia wartość flagi Ctacho na true lub false w zależności od stanu checkboxa.
 */
void wyswietlanie::on_checkBoxTacho_stateChanged(int arg1)
{
    if(arg1){
        Ctacho=true;
    }
    else
    {
        Ctacho=false;
    }
}

/**
 * @brief Obsługa zaznaczenia/odznaczenia checkboxa Temp1.
 * Ustawia wartość flagi Ctem1 na true lub false w zależności od stanu checkboxa.
 */
void wyswietlanie::on_checkBoxTemp1_stateChanged(int arg1)
{
    if(arg1){
        Ctem1=true;
    }
    else
    {
        Ctem1=false;
    }
}




void wyswietlanie::on_horizontalSlider_osX_valueChanged(int value)
{
        zakresX=value;
}



void wyswietlanie::on_horizontalSlider_osX_2_valueChanged(int value)
{
    zakresY=value;
}

