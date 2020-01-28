#include "perso.h"
#include "ui_perso.h"
#include <QMessageBox>
#include "univers.h"
#include "../aspectratiolabel.h"
#include "../gestionnairecarac.h"

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
        QString cheminImg = persoCourant->GetValeurCarac(GestionnaireCarac::CARAC_CHEMIN_PORTRAIT);
        if ( cheminImg != "" )
        {
            persoCourant->m_ImagePortrait.load(cheminImg);
            ui->imagePortrait->setPixmap(persoCourant->m_ImagePortrait);
        }
    }

    ui->portraitLabel->setText(
                persoCourant->GetValeurCarac(GestionnaireCarac::CARAC_NOM));

    // TODO : nettoyer chaque fois les caracsaffichées ? MAJ ?
    QMap<QString, shared_ptr<Carac>> caracs = GestionnaireCarac::GetGestionnaireCarac()->GetCaracs();
    // caracs
    QMap<QString, shared_ptr<Carac>>::const_iterator i = caracs.constBegin();
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
                ui->caracsLayout2->addWidget(i.value().get());
                ui->caracsLayout2->setAlignment(i.value().get(), Qt::AlignLeft);
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
