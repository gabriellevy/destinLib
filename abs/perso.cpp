#include "perso.h"
#include "ui_perso.h"
#include <QMessageBox>
#include "univers.h"
#include "../aspectratiolabel.h"
#include "../gestcarac.h"

using std::shared_ptr;
using std::make_shared;

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
    Q_ASSERT_X(IPerso::s_PersosInterface != nullptr, "IPerso mal initialisé", "IPerso::GetPersoInterface");
    return IPerso::s_PersosInterface;
}

void IPerso::AjouterPersoJouable(shared_ptr<DPerso> perso)
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
    QHashIterator<QString, shared_ptr<DPerso>> i(m_Persos);
    while (i.hasNext()) {
        i.next();
        i.value()->InitialiserPerso();
    }
}


shared_ptr<DPerso> IPerso::GetPerso(QString id) const
{
    return m_Persos[id];
}

shared_ptr<DPerso> IPerso::GetPersoCourant()
{
    IPerso* iPerso = IPerso::GetPersoInterface();
    return iPerso->GetPerso(IPerso::s_IdPersoActif);
}

void IPerso::RafraichirAffichage()
{
    shared_ptr<DPerso> persoCourant = m_Persos[IPerso::s_IdPersoActif];
    persoCourant->RafraichirAffichage();

    // portrait
    if ( myImageLabel == nullptr )
    {
        QString cheminImg = persoCourant->GetValeurCarac(GestCarac::CARAC_CHEMIN_PORTRAIT);
        if ( cheminImg != "" )
        {
            persoCourant->m_ImagePortrait.load(cheminImg);
            ui->imagePortrait->setPixmap(persoCourant->m_ImagePortrait);
        }
    }

    ui->portraitLabel->setText(
                persoCourant->GetValeurCarac(GestCarac::CARAC_NOM));

    // TODO : nettoyer chaque fois les caracsaffichées ? MAJ ?
    QHash<QString, Carac*> caracs = GestCarac::GetGestionnaireCarac()->GetCaracs();
    // caracs
    QHash<QString, Carac*>::const_iterator i = caracs.constBegin();
    while (i != caracs.constEnd()) {
        Q_ASSERT_X( i.value() != nullptr,
                    "Carac Inconnue",
                    " IPerso::RafraichirAffichage");
        i.value()->hide();
        ++i;
    }

    QVector<QString> caracsAffichees = GestCarac::GetGestionnaireCarac()->m_CaracsAffichees;
    if ( caracsAffichees.size() > 0 )
    {
        for ( QString idCarac: caracsAffichees) {
            Carac* carac = caracs[idCarac];
            if ( carac->bAffichable())
            {
                carac->Afficher();
                switch(carac->m_EmplacementAffichage) {
                case ea_Secondaire : {
                    ui->caracsSecondaires->addWidget(carac);
                    ui->caracsSecondaires->setAlignment(carac, Qt::AlignLeft);
                }break;
                case ea_Primaire : {
                    ui->caracsPrimaires->addWidget(carac);
                    ui->caracsPrimaires->setAlignment(carac, Qt::AlignCenter);
                }break;
                case ea_ImgEntete : {
                    ui->imagesEnteteLayout->addWidget(carac);
                }break;
                case ea_ImgPrimaire : {
                    ui->imgPrimaire->addWidget(carac);
                    ui->imgPrimaire->setAlignment(carac, Qt::AlignLeft);
                }break;
                }
                carac->show();
            }
        }
    }

    //ui->verticalLayoutPerso->update();
}

IPerso::~IPerso()
{
    delete ui;
}
