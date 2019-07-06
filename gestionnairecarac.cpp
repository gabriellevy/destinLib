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
    if ( !GestionnaireCarac::GetGestionnaireCarac()->CetteCaracExisteDeja(id) )
        return "";
    return GestionnaireCarac::GetGestionnaireCarac()->GetCarac(id)->m_DataCarac.m_Valeur;
}

void GestionnaireCarac::AppliquerCarac(SetCarac setCarac)
{
    bool trouve = this->CetteCaracExisteDeja(setCarac.m_CaracId);

    Carac* carac;
    if ( trouve )
    {
        carac = m_Caracs[setCarac.m_CaracId];
    }
    else {
        carac = new Carac;
        carac->m_DataCarac.m_Id = setCarac.m_CaracId;
        carac->m_DataCarac.m_Valeur = "0";
        m_Caracs[setCarac.m_CaracId] = carac;
    }

    switch(setCarac.m_ModifCaracType)
    {
    case ModifCaracType::SetCarac : {
        carac->m_DataCarac.m_Valeur = setCarac.GetValeur();

        if ( carac->m_ModeAffichage == MODE_AFFICHAGE::ma_ImgValeur ) {
            carac->m_Img.load(carac->m_DataCarac.m_Valeur);
        }
    }break;
    case ModifCaracType::AddToCarac : {
        double valeur = carac->m_DataCarac.m_Valeur.toDouble();
        valeur += setCarac.GetValeur().toDouble();
        carac->m_DataCarac.m_Valeur = QString::number(valeur);

    }break;
    case ModifCaracType::RetireDeCarac : {
        double valeur = carac->m_DataCarac.m_Valeur.toDouble();
        valeur -= setCarac.GetValeur().toDouble();
        carac->m_DataCarac.m_Valeur = QString::number(valeur);

    }break;
    }
}

void GestionnaireCarac::AjouterCarac(Carac* carac)
{
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
}

int GestionnaireCarac::GetCaracValueAsInt(QString id)
{
    return GestionnaireCarac::GetCaracValue(id).toInt();
}

bool GestionnaireCarac::CetteCaracExisteDeja(QString id)
{
    return m_Caracs.contains(id);
}

void GestionnaireCarac::AppliquerSetCarac(const SetCarac& setCarac)
{
    GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(setCarac);
}

int GestionnaireCarac::AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee)
{
    GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(
                SetCarac(ModifCaracType::AddToCarac, idCarac, QString::number(valeurAjoutee)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

int GestionnaireCarac::RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree)
{
    GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(
                SetCarac(ModifCaracType::RetireDeCarac, idCarac, QString::number(valeurRetiree)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

QString GestionnaireCarac::SetValeurACaracId(const QString& idCarac, const QString& valeurSet)
{
    Q_ASSERT_X(Univers::ME->GetExecHistoire() != nullptr, "Exec histoire non initialisée !","Carac::SetValeurACaracId" );
    GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(SetCarac(ModifCaracType::SetCarac, idCarac, valeurSet));

    return GestionnaireCarac::GetCaracValue(idCarac);
}

Carac* GestionnaireCarac::GetCarac(QString idCarac)
{
    return m_Caracs[idCarac];

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

QMap<QString, Carac*> GestionnaireCarac::GetCaracs()
{
    return this->m_Caracs;
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
