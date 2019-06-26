#include "evt.h"
#include "univers.h"
#include <QDebug>

/*Evt::Evt(QJsonObject evtJson, QWidget *parent) :
    QWidget(parent),
    Noeud(evtJson),
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_TypeEvenement = TE_Base;

    // logique gameplay :
    if ( !evtJson.contains("id") || !evtJson["id"].isString())
        QMessageBox::warning(Univers::ME, "erreur", "Un événement sans id !");

    if ( evtJson.contains("effets") && evtJson["effets"].isArray())
    {
        QJsonArray jsonArrayEffets = evtJson["effets"].toArray();

        for ( int i = 0; i < jsonArrayEffets.size(); ++i)
        {
            Effet* effet = new Effet(jsonArrayEffets[i].toObject());
            //this->layout()->addWidget(effet);
            m_Effets.append(effet);
        }
    }
    else if ( m_Id != "abandon" )
        QMessageBox::warning(Univers::ME, "erreur", "Un événement d'id " + m_Id + " sans effets !");

    // interface
    m_EffetActuel = nullptr;
}*/

Evt::Evt(QString id,
         QString nom) :
    NoeudNarratif(id,
          nom,
          ""),
    m_CheminImgFond("")
{

    m_TypeEvenement = TE_Base;
    m_TypeNoeud = TypeNoeud::etn_Evt;
}


Effet* Evt::TrouverEffet(QString id)
{
    foreach(Effet* effet, m_Effets)
    {
        if ( effet->m_Id == id)
            return effet;
    }
    return nullptr;
}
