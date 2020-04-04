#include "gestionnairecarac.h"

#include "abs/univers.h"
#include "abs/setcarac.h"

using std::make_shared;
using std::shared_ptr;

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
    return GestionnaireCarac::GetGestionnaireCarac()->GetCarac(id)->m_DataCarac.GetValeur();
}

bool GestionnaireCarac::IsCaracTrue(QString id)
{
    if ( !GestionnaireCarac::GetGestionnaireCarac()->CetteCaracExisteDeja(id) )
        return false;
    QString val = GestionnaireCarac::GetGestionnaireCarac()->GetCarac(id)->m_DataCarac.GetValeur();
    return val != "" && val != "0";
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
        carac =  new Carac();
        carac->m_DataCarac.m_Id = setCarac.m_CaracId;
        carac->m_DataCarac.SetValeur("0");
        m_Caracs[setCarac.m_CaracId] = carac;
    }

    switch(setCarac.m_ModifCaracType)
    {
    case ModifCaracType::SetCarac : {
        carac->m_DataCarac.SetValeur(setCarac.GetValeur());

        if ( carac->m_ModeAffichage == MODE_AFFICHAGE::ma_ImgValeur ) {
            carac->m_Img.load(carac->m_DataCarac.GetValeur());
        }
    }break;
    case ModifCaracType::AddToCarac : {
        double valeur = carac->m_DataCarac.GetValeurDouble();
        valeur += setCarac.GetValeur().toDouble();
        carac->m_DataCarac.SetValeur(valeur);

    }break;
    case ModifCaracType::RetireDeCarac : {
        double valeur = carac->m_DataCarac.GetValeurDouble();
        valeur -= setCarac.GetValeur().toDouble();
        carac->m_DataCarac.SetValeur(valeur);

    }break;
    }
}

void GestionnaireCarac::AjouterCarac(Carac* carac)
{
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
}

Carac* GestionnaireCarac::AjouterCaracNombre(QString idCarac, int valeur, int valMin, int valMax)
{
    Carac* carac = new Carac(idCarac, idCarac, QString::number(valeur),
                             "", idCarac, MODE_AFFICHAGE::ma_Nombre, nullptr,
                             QString::number(valMin), QString::number(valMax));
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestionnaireCarac::AjouterCaracBinaire(QString idCarac, bool valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur?"1":"",
                             "", idCarac, MODE_AFFICHAGE::ma_Binaire);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestionnaireCarac::AjouterCaracString(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_Texte);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestionnaireCarac::AjouterCaracImageValeur(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_ImgValeur);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
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
    GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(SetCarac(ModifCaracType::SetCarac, idCarac, valeurSet));

    return GestionnaireCarac::GetCaracValue(idCarac);
}

QString GestionnaireCarac::SetValeurACaracId(const QString& idCarac, const int& valeurSet)
{
    return SetValeurACaracId(idCarac, QString::number(valeurSet));
}

QString GestionnaireCarac::EffacerValeurACaracId(const QString& idCarac)
{
    return SetValeurACaracId(idCarac, "");
}

Carac* GestionnaireCarac::GetCarac(QString idCarac)
{
    return m_Caracs[idCarac];
}


QString GestionnaireCarac::CARAC_NOM = "PersoNom";
QString GestionnaireCarac::CARAC_PERSO_ID = "PersoId";
QString GestionnaireCarac::CARAC_DESCRIPTION = "PersoDescription";
QString GestionnaireCarac::CARAC_CHEMIN_PORTRAIT = "PersoCheminPortrait";

DPerso::DPerso(QString id, QString nom, QString description, QString CheminImagePortrait)
{
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_PERSO_ID, id));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_NOM, nom));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_DESCRIPTION, description));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestionnaireCarac::CARAC_CHEMIN_PORTRAIT, CheminImagePortrait));
}


void DPerso::MajNom(QString nouveauNom)
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestionnaireCarac::CARAC_NOM)
            m_SetCaracs[i]->m_Valeur = nouveauNom;
    }
}


shared_ptr<DPerso> DPerso::GetDPersoJoue()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
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
    Q_ASSERT_X(false, "chemin image de perso introuvable ! ", "DPerso::MajCheminImage");
}

QString DPerso::GetId()
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestionnaireCarac::CARAC_PERSO_ID)
            return m_SetCaracs[i]->m_Valeur;
    }
    Q_ASSERT_X(false, "id de perso introuvable ! ", "DPerso::GetId");
    return "id introuvable !!";
}

QHash<QString, Carac*> GestionnaireCarac::GetCaracs()
{
    return this->m_Caracs;
}


void DPerso::RafraichirAffichage()
{
    Appliquer();
}

QString DPerso::GetValeurCarac(QString id)
{
    return GestionnaireCarac::GetCaracValue(id);
}

int DPerso::GetValeurCaracAsInt(QString id)
{
    return GestionnaireCarac::GetCaracValueAsInt(id);
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

void DPerso::AppliquerSetCarac(const SetCarac& setCarac)
{
    ExecutionPreChangeCarac();
    GestionnaireCarac::AppliquerSetCarac(setCarac);
    ExecutionPostChangeCarac();
}

int DPerso::AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee)
{
    ExecutionPreChangeCarac();
    int res = GestionnaireCarac::AJouterValeurACaracId(idCarac, valeurAjoutee);
    ExecutionPostChangeCarac();
    return res;
}

void DPerso::ExecutionPostChangeCarac()
{

}

void DPerso::ExecutionPreChangeCarac()
{

}

int DPerso::RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree)
{
    ExecutionPreChangeCarac();
    int res = GestionnaireCarac::RetirerValeurACaracId(idCarac, valeurRetiree);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::SetValeurACaracId(const QString& idCarac,const  QString& valeurSet)
{
    ExecutionPreChangeCarac();
    QString res = GestionnaireCarac::SetValeurACaracId(idCarac, valeurSet);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::SetValeurACaracId(const QString& idCarac,const int& valeurSet)
{
    ExecutionPreChangeCarac();
    QString res = GestionnaireCarac::SetValeurACaracId(idCarac, valeurSet);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::EffacerValeurACaracId(const QString& idCarac)
{
    ExecutionPreChangeCarac();
    QString res = GestionnaireCarac::EffacerValeurACaracId(idCarac);
    ExecutionPostChangeCarac();
    return res;
}

bool DPerso::IsCaracTrue(QString id)
{
    return GestionnaireCarac::IsCaracTrue(id);
}
