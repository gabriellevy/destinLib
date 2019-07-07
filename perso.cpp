#include "perso.h"
#include "ui_perso.h"
#include <QMessageBox>
#include "univers.h"
#include "aspectratiolabel.h"
#include "gestionnairecarac.h"

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
                GestionnaireCarac::GetGestionnaireCarac()->m_Caracs.append(carac);
            }
        }

        AjouterPersoJouable(donneePerso);

    }

    RafraichirAffichage();
}*/

void IPerso::AjouterPersoJouable(DPerso* perso)
{
    IPerso::GetPersoInterface()->m_Persos.insert(
                perso->GetId(),
                perso);

    // par défaut le perso joué est le prmeir ajouté
    if ( s_IdPersoActif == "" )
       s_IdPersoActif = perso->GetId();
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
    this->GetPersoCourant()->RafraichirAffichage();

    // portrait
    if ( myImageLabel == nullptr )
    {
        QString cheminImg = GestionnaireCarac::GetCaracValue(GestionnaireCarac::CARAC_CHEMIN_PORTRAIT);
        if ( cheminImg != "" )
        {
            GetPersoCourant()->m_ImagePortrait.load(cheminImg);
            ui->imagePortrait->setPixmap(GetPersoCourant()->m_ImagePortrait);
        }
    }

    ui->portraitLabel->setText(
                GestionnaireCarac::GetCaracValue(GestionnaireCarac::CARAC_NOM));

    // TODO : nettoyer chaque fois les caracsaffichées ? MAJ ?
    QMap<QString, Carac*> caracs = GestionnaireCarac::GetGestionnaireCarac()->GetCaracs();
    // caracs
    QMap<QString, Carac*>::const_iterator i = caracs.constBegin();
    while (i != caracs.constEnd()) {
        Q_ASSERT_X( i.value() != nullptr,
                    "Carac Inconnue",
                    " IPerso::RafraichirAffichage");
        i.value()->hide();
        ++i;
    }

    if ( caracs.size() > 0 )
    {
        i = caracs.constBegin();
        while (i != caracs.constEnd()) {
            if ( i.value()->bAffichable())
            {
                i.value()->Afficher();
                ui->caracsLayout2->addWidget(i.value());
                ui->caracsLayout2->setAlignment(i.value(), Qt::AlignLeft);
                i.value()->show();
            }
            ++i;
        }
    }

    //ui->verticalLayoutPerso->update();
}

IPerso::~IPerso()
{
    delete ui;
}
