#include "gestionnairecarac.h"

#include "univers.h"
#include "setcarac.h"

GestionnaireCarac::GestionnaireCarac()
{

}

GestionnaireCarac* GestionnaireCarac::ME = nullptr;

GestionnaireCarac* GestionnaireCarac::GetGestionnaireCarac()
{
    if ( GestionnaireCarac::ME == nullptr)
        GestionnaireCarac::ME = new GestionnaireCarac();

    return GestionnaireCarac::ME;
}


QString GestionnaireCarac::GetCaracValue(QString id)
{
    Q_ASSERT_X(Univers::ME->GetExecHistoire() != nullptr, "Exec histoire non initialisée !","Carac::SetValeurACaracId" );
    return Univers::ME->GetExecHistoire()->GetCaracValue(id);
}

int GestionnaireCarac::GetCaracValueAsInt(QString id)
{
    return Univers::ME->GetExecHistoire()->GetCaracValue(id).toInt();
}

void GestionnaireCarac::AppliquerSetCarac(const SetCarac& setCarac)
{
    Univers::ME->GetExecHistoire()->AppliquerCarac(setCarac);
}

int GestionnaireCarac::AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee)
{
    Univers::ME->GetExecHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::AddToCarac, idCarac, QString::number(valeurAjoutee)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

int GestionnaireCarac::RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree)
{
    Univers::ME->GetExecHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::RetireDeCarac, idCarac, QString::number(valeurRetiree)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

QString GestionnaireCarac::SetValeurACaracId(const QString& idCarac, const QString& valeurSet)
{
    Q_ASSERT_X(Univers::ME->GetExecHistoire() != nullptr, "Exec histoire non initialisée !","Carac::SetValeurACaracId" );
    Univers::ME->GetExecHistoire()->AppliquerCarac(SetCarac(ModifCaracType::SetCarac, idCarac, valeurSet));

    return GestionnaireCarac::GetCaracValue(idCarac);
}

Carac* GestionnaireCarac::GetCarac(QString idCarac)
{
    for (int i = 0 ; m_Caracs.length() ; ++i) {
        if ( m_Caracs[i]->m_DataCarac.m_Id == idCarac)
            return m_Caracs[i];
    }
    return nullptr;
}


QString GestionnaireCarac::CARAC_NOM = "PersoNom";
QString GestionnaireCarac::CARAC_PERSO_ID = "PersoId";
QString GestionnaireCarac::CARAC_DESCRIPTION = "PersoDescription";
QString GestionnaireCarac::CARAC_CHEMIN_PORTRAIT = "PersoCheminPortrait";

DPerso::DPerso(QString id, QString nom, QString description, QString CheminImagePortrait)
{
    this->m_SetCaracs.push_back( new SetCarac(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_PERSO_ID, id));
    this->m_SetCaracs.push_back( new SetCarac(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_NOM, nom));
    this->m_SetCaracs.push_back( new SetCarac(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_DESCRIPTION, description));
    this->m_SetCaracs.push_back( new SetCarac(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_CHEMIN_PORTRAIT, CheminImagePortrait));
}


void DPerso::MajNom(QString nouveauNom)
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestionnaireCarac::CARAC_NOM)
            m_SetCaracs[i]->m_Valeur = nouveauNom;
    }
}

void DPerso::MajCheminImage(QString chemin)
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestionnaireCarac::CARAC_CHEMIN_PORTRAIT)
        {
            m_SetCaracs[i]->m_Valeur = chemin;
            return;
        }
    }
    Q_ASSERT_X(true, "chemin image de perso introuvable ! ", "DPerso::MajCheminImage");
}

QString DPerso::GetId()
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestionnaireCarac::CARAC_PERSO_ID)
            return m_SetCaracs[i]->m_Valeur;
    }
    Q_ASSERT_X(true, "id de perso introuvable ! ", "DPerso::GetId");
    return "id introuvable !!";
}

DPerso* IPerso::GetPersoCourant()
{
    return m_Persos[IPerso::s_IdPersoActif];
}


const DPerso* IPerso::GetPerso(QString id)
{
    return m_Persos[id];
}

void DPerso::RafraichirAffichage()
{
    Appliquer();
}

MultiSetterDeCarac::MultiSetterDeCarac()
{

}

void MultiSetterDeCarac::Appliquer()
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        GestionnaireCarac::AppliquerSetCarac(*m_SetCaracs[i]);
    }
}
