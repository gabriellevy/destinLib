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

Choix::Choix(std::shared_ptr<Effet> parentEffet, QString text, QString cheminImg) :
        NoeudNarratif(),
        m_ParentEffet(parentEffet),
        m_CheminImg(cheminImg)
{
    m_TypeNoeud = TypeNoeud::etn_Choix;
    m_Texte = text;
}


Choix::Choix(std::shared_ptr<LancerDe> lancerDe, QString text, QString cheminImg) :
    NoeudNarratif(),
    m_ParentLancerDe(lancerDe),
    m_CheminImg(cheminImg)
{
    m_TypeNoeud = TypeNoeud::etn_Choix;
    m_Texte = text;
}

bool Choix::AQuelqueChoseAAfficher()
{
    return false;
}

