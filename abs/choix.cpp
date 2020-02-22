#include "choix.h"

/*Choix::Choix(QJsonObject choixJson, QWidget *parent) :
    QPushButton("", parent),
    Noeud(choixJson),
    ui(new Ui::Choix)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(ExecuterNoeudSlot()));
}*/

Choix::Choix(QString text, QString cheminImg) :
        NoeudNarratif(),
        m_CheminImg(cheminImg)
{
    m_TypeNoeud = TypeNoeud::etn_Choix;
    m_Texte = text;
}

bool Choix::AQuelqueChoseAAfficher()
{
    return false;
}

