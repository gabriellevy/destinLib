#include "perso.h"
#include "ui_perso.h"
#include <QMessageBox>
#include "univers.h"
#include "aspectratiolabel.h"

DPerso::DPerso(QString id, QString nom, QString description, QString CheminImagePortrait)
    :m_Id(id), m_Nom(nom), m_Description(description), m_CheminImagePortrait(CheminImagePortrait)
{

}

IPerso::IPerso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Perso)
{
    ui->setupUi(this);
    IPerso::s_PersosInterface = this;
}

IPerso* IPerso::s_PersosInterface = nullptr;
QString IPerso::s_IdPersoActif = "";

IPerso* IPerso::GetPersoInterface()
{
    return IPerso::s_PersosInterface;
}

DPerso* IPerso::GetPersoCourant()
{
    return m_Persos[IPerso::s_IdPersoActif];
}

/*void IPerso::Rafraichir(QJsonArray persos)
{
    for (int i = 0 ; i < persos.size() ; i++)
    {
        QJsonObject perso = persos[i].toObject();
        DPerso donneePerso;

        if ( perso.contains("id") && perso["id"].isString())
        {
            donneePerso.m_Id= perso["id"].toString();
        }

        if ( perso.contains("nom") && perso["nom"].isString())
        {
            ui->portraitLabel->setFont(*Univers::TITRE_FONT);
            donneePerso.m_Nom = perso["nom"].toString();
        }
        else
        {
            QMessageBox::warning(Univers::ME, "erreur", "Pas de nom de heros dans ce fichier aventure !");
        }

        // portrait
        if ( perso.contains("portrait") && perso["portrait"].isString())
        {
            donneePerso.m_ImagePortrait = perso["portrait"].toString();
        }

        if ( perso.contains("caracs_a_afficher") && perso["caracs_a_afficher"].isArray())
        {
            QJsonArray jsonArrayCaracs = perso["caracs_a_afficher"].toArray();

            for ( int i = 0; i < jsonArrayCaracs.size(); ++i)
            {
                Carac* carac = new Carac;
                QJsonObject caracJsonObject = jsonArrayCaracs[i].toObject();

                if ( caracJsonObject.contains("id") && caracJsonObject["id"].isString() )
                {
                    QString id = caracJsonObject["id"].toString();
                    donneePerso.m_CaracsAAfficher.push_back( id );

                    if (Univers::ME->GetHistoire()->CetteCaracExisteDeja(id))
                        continue;
                    carac->m_DataCarac.m_Id = id;
                }
                else
                    QMessageBox::warning(Univers::ME, "erreur de carac", "Elle ne contient pas d'id !");

                if ( caracJsonObject.contains("intitule") && caracJsonObject["intitule"].isString() )
                {
                    carac->m_DataCarac.m_Intitule = caracJsonObject["intitule"].toString();
                }

                if ( caracJsonObject.contains("valeur") && caracJsonObject["valeur"].isString() )
                {
                    carac->m_DataCarac.m_Valeur = caracJsonObject["valeur"].toString();
                }

                if ( caracJsonObject.contains("description") && caracJsonObject["description"].isString() )
                {
                    carac->m_DataCarac.m_Description = caracJsonObject["description"].toString();
                }

                QString m_TypeAffichage = "";
                if ( caracJsonObject.contains("mode_affichage") && caracJsonObject["mode_affichage"].isString() )
                {
                    m_TypeAffichage = caracJsonObject["mode_affichage"].toString();
                }

                carac->DeterminerModeAffichage(m_TypeAffichage);
                Univers::ME->GetHistoire()->m_Caracs.append(carac);
            }
        }

        AjouterPersoJouable(donneePerso);

    }

    RafraichirAffichage();
}*/

void IPerso::AjouterPersoJouable(DPerso* perso)
{
    IPerso::GetPersoInterface()->m_Persos.insert(perso->m_Id, perso);

    // par défaut le perso joué est le prmeir ajouté
    if ( s_IdPersoActif == "" )
       s_IdPersoActif = perso->m_Id;
}

const DPerso* IPerso::GetPerso(QString id)
{
    return m_Persos[id];
}


void IPerso::ChangerPersoCourant(QString changePerso)
{
    s_IdPersoActif = changePerso;
    RafraichirAffichage();
}

void DPerso::InitialiserPerso()
{

}

void IPerso::InitialiserPerso()
{
    QHashIterator<QString, DPerso*> i(m_Persos);
    while (i.hasNext()) {
        i.next();
        i.value()->InitialiserPerso();
    }
}

void IPerso::RafraichirAffichage()
{
    // portrait
    if ( myImageLabel == nullptr )
    {
        if ( GetPersoCourant()->m_CheminImagePortrait != "" )
        {
            GetPersoCourant()->m_ImagePortrait.load(GetPersoCourant()->m_CheminImagePortrait);
            ui->imagePortrait->setPixmap(GetPersoCourant()->m_ImagePortrait);
        }

        ui->portraitLabel->setText(GetPersoCourant()->m_Nom);
    }

    // TODO : nettoyer chaque fois les caracsaffichées ? MAJ ?
    QVector<Carac*> caracs = Univers::ME->GetHistoire()->m_Caracs;
    // caracs
    for ( int i = 0; i < caracs.size() ; ++i)
    {
        caracs[i]->hide();
    }
    if ( caracs.size() > 0 )
    {
        for ( int i = 0; i < caracs.size() ; ++i)
        {
            if ( caracs[i]->bAffichable())
            {
                caracs[i]->Afficher();
                ui->caracsLayout2->addWidget(caracs[i]);
                ui->caracsLayout2->setAlignment(caracs[i], Qt::AlignLeft);
                caracs[i]->show();
            }
        }
    }

    //ui->verticalLayoutPerso->update();
}

IPerso::~IPerso()
{
    delete ui;
}
