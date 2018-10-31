#include "perso.h"
#include "ui_perso.h"
#include <QMessageBox>
#include "aventure.h"
#include "aspectratiolabel.h"

IPerso::IPerso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Perso)
{
    ui->setupUi(this);
    IPerso::s_PersosInterface = this;
    s_IndexPersoActif = 0;
}

IPerso* IPerso::s_PersosInterface = nullptr;
int IPerso::s_IndexPersoActif = -1;

IPerso* IPerso::GetPersoInterface()
{
    return IPerso::s_PersosInterface;
}

DPerso IPerso::GetPersoCourant()
{
    return m_Persos[IPerso::s_IndexPersoActif];
}

void IPerso::Rafraichir(QJsonArray persos)
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
            ui->portraitLabel->setFont(*Aventure::TITRE_FONT);
            donneePerso.m_Nom = perso["nom"].toString();
        }
        else
        {
            QMessageBox::warning(Aventure::ME, "erreur", "Pas de nom de heros dans ce fichier aventure !");
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

                    if (Aventure::ME->GetHistoire()->CetteCaracExisteDeja(id))
                        continue;
                    carac->m_Id = id;
                }
                else
                    QMessageBox::warning(Aventure::ME, "erreur de carac", "Elle ne contient pas d'id !");

                if ( caracJsonObject.contains("intitule") && caracJsonObject["intitule"].isString() )
                {
                    carac->m_Intitule = caracJsonObject["intitule"].toString();
                }

                if ( caracJsonObject.contains("valeur") && caracJsonObject["valeur"].isString() )
                {
                    carac->m_Valeur = caracJsonObject["valeur"].toString();
                }

                /*if ( caracJsonObject.contains("img") && caracJsonObject["img"].isString() )
                {
                    carac->m_Img = caracJsonObject["img"].toString();
                }*/

                if ( caracJsonObject.contains("description") && caracJsonObject["description"].isString() )
                {
                    carac->m_Description = caracJsonObject["description"].toString();
                }

                QString m_TypeAffichage = "";
                if ( caracJsonObject.contains("mode_affichage") && caracJsonObject["mode_affichage"].isString() )
                {
                    m_TypeAffichage = caracJsonObject["mode_affichage"].toString();
                }

                carac->DeterminerModeAffichage(m_TypeAffichage);
                Aventure::ME->GetHistoire()->m_Caracs.append(carac);
            }
        }

        m_Persos.push_back(donneePerso);

    }

    RafraichirAffichage();
}


void IPerso::ChangerPersoCourant(QString changePerso)
{
    for ( int i = 0 ; i < m_Persos.size() ; i++)
    {
        if ( m_Persos[i].m_Id == changePerso)
        {
            s_IndexPersoActif = i;
            RafraichirAffichage();
            return;
        }
    }
}

void IPerso::RafraichirAffichage()
{
    // portrait
    if ( myImageLabel == nullptr )
    {
        ui->imagePortrait->setPixmap(GetPersoCourant().m_ImagePortrait);

        ui->portraitLabel->setText(GetPersoCourant().m_Nom);
    }

    // TODO : nettoyer chaque fois les caracsaffichées ? MAJ ?
    QVector<Carac*> caracs = Aventure::ME->GetHistoire()->m_Caracs;
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
