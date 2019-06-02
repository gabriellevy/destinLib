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

Choix::Choix(Effet* ParentEffet, QString text, QString cheminImg) :
        Noeud(),
        m_ParentEffet(ParentEffet),
        m_CheminImg(cheminImg)
{
    m_TypeNoeud = TypeNoeud::etn_Choix;
    m_Text = text;
}

bool Choix::AQuelqueChoseAAfficher()
{
    return false;
}

