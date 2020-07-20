#include "gestcarac.h"

#include "abs/univers.h"
#include "abs/setcarac.h"

using std::make_shared;
using std::shared_ptr;

GestCarac::GestCarac()
{

}

GestCarac* GestCarac::ME = nullptr;

GestCarac* GestCarac::GetGestionnaireCarac()
{
    if ( GestCarac::ME == nullptr)
        GestCarac::ME = new GestCarac();

    return GestCarac::ME;
}


QString GestCarac::GetCaracValue(QString id)
{
    if ( !GestCarac::GetGestionnaireCarac()->CetteCaracExisteDeja(id) )
        return "";
    return GestCarac::GetGestionnaireCarac()->GetCarac(id)->m_DataCarac.GetValeur();
}

bool GestCarac::IsCaracTrue(QString id)
{
    if ( !GestCarac::GetGestionnaireCarac()->CetteCaracExisteDeja(id) )
        return false;
    QString val = GestCarac::GetGestionnaireCarac()->GetCarac(id)->m_DataCarac.GetValeur();
    return val != "" && val != "0";
}

void GestCarac::AppliquerCarac(SetCarac setCarac)
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

    // log :
    if(Univers::LOG && Univers::FILE.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      QTextStream stream(&Univers::FILE);
      stream  <<setCarac.m_CaracId << " " << carac->m_DataCarac.GetValeur() << " -> ";
      Univers::FILE.close();
    }

    switch(setCarac.m_ModifCaracType)
    {
    case ModifCaracType::SetCarac : {
        carac->m_DataCarac.SetValeur(setCarac.GetValeur());
        qDebug() << "ModifCaracType::SetCarac"<<endl;
        qDebug() << carac->m_DataCarac.GetValeur()<<endl;

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

    // log :
    if(Univers::LOG && Univers::FILE.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      QTextStream stream(&Univers::FILE);
      stream << setCarac.GetValeur()<< "\n";
      Univers::FILE.close();
    }
}

void GestCarac::AjouterCarac(Carac* carac)
{
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
}

Carac* GestCarac::AjouterCaracNombre(QString idCarac, int valeur, int valMin, int valMax)
{
    Carac* carac = new Carac(idCarac, idCarac, QString::number(valeur),
                             "", idCarac, MODE_AFFICHAGE::ma_Nombre, nullptr,
                             QString::number(valMin), QString::number(valMax));
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracNombreSupZero(QString idCarac, int valeur, int valMin, int valMax)
{
    Carac* carac = new Carac(idCarac, idCarac, QString::number(valeur),
                             "", idCarac, MODE_AFFICHAGE::ma_NombreSupZero, nullptr,
                             QString::number(valMin), QString::number(valMax));
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracBinaire(QString idCarac, bool valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur?"1":"",
                             "", idCarac, MODE_AFFICHAGE::ma_Binaire);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracString(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_Texte);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracStringIntitule(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_Texte_intitule);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracImagePrimaire(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_Img);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    carac->m_EmplacementAffichage = ea_ImgPrimaire;
    return carac;
}

Carac* GestCarac::AjouterCaracImage(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_Img);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

Carac* GestCarac::AjouterCaracImageValeur(QString idCarac, QString valeur)
{
    Carac* carac = new Carac(idCarac, idCarac, valeur,
                             "", idCarac, MODE_AFFICHAGE::ma_ImgValeur);
    this->m_Caracs[carac->m_DataCarac.m_Id] = carac;
    this->m_CaracsAffichees.push_back(carac->m_DataCarac.m_Id);
    return carac;
}

int GestCarac::GetCaracValueAsInt(QString id)
{
    return GestCarac::GetCaracValue(id).toInt();
}

bool GestCarac::CetteCaracExisteDeja(QString id)
{
    return m_Caracs.contains(id);
}

void GestCarac::AppliquerSetCarac(const SetCarac& setCarac)
{
    GestCarac::GetGestionnaireCarac()->AppliquerCarac(setCarac);
}

int GestCarac::AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee)
{
    GestCarac::GetGestionnaireCarac()->AppliquerCarac(
                SetCarac(ModifCaracType::AddToCarac, idCarac, QString::number(valeurAjoutee)));
    return GestCarac::GetCaracValueAsInt(idCarac);
}

int GestCarac::RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree)
{
    GestCarac::GetGestionnaireCarac()->AppliquerCarac(
                SetCarac(ModifCaracType::RetireDeCarac, idCarac, QString::number(valeurRetiree)));
    return GestCarac::GetCaracValueAsInt(idCarac);
}

QString GestCarac::SetValeurACaracId(const QString& idCarac, const QString& valeurSet)
{
    GestCarac::GetGestionnaireCarac()->AppliquerCarac(SetCarac(ModifCaracType::SetCarac, idCarac, valeurSet));

    return GestCarac::GetCaracValue(idCarac);
}

QString GestCarac::SetValeurACaracId(const QString& idCarac, const int& valeurSet)
{
    return SetValeurACaracId(idCarac, QString::number(valeurSet));
}

QString GestCarac::EffacerValeurACaracId(const QString& idCarac)
{
    return SetValeurACaracId(idCarac, "");
}

Carac* GestCarac::GetCarac(QString idCarac)
{
    return m_Caracs[idCarac];
}


QString GestCarac::CARAC_NOM = "PersoNom";
QString GestCarac::CARAC_PERSO_ID = "PersoId";
QString GestCarac::CARAC_DESCRIPTION = "PersoDescription";
QString GestCarac::CARAC_CHEMIN_PORTRAIT = "PersoCheminPortrait";

DPerso::DPerso(QString id, QString nom, QString description, QString CheminImagePortrait)
{
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestCarac::CARAC_PERSO_ID, id));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestCarac::CARAC_NOM, nom));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestCarac::CARAC_DESCRIPTION, description));
    this->m_SetCaracs.push_back( make_shared<SetCarac>(ModifCaracType::SetCarac, GestCarac::CARAC_CHEMIN_PORTRAIT, CheminImagePortrait));
}


void DPerso::MajNom(QString nouveauNom)
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        if ( m_SetCaracs[i]->m_CaracId == GestCarac::CARAC_NOM)
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
        if ( m_SetCaracs[i]->m_CaracId == GestCarac::CARAC_CHEMIN_PORTRAIT)
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
        if ( m_SetCaracs[i]->m_CaracId == GestCarac::CARAC_PERSO_ID)
            return m_SetCaracs[i]->m_Valeur;
    }
    Q_ASSERT_X(false, "id de perso introuvable ! ", "DPerso::GetId");
    return "id introuvable !!";
}

QHash<QString, Carac*> GestCarac::GetCaracs()
{
    return this->m_Caracs;
}


QMap<QString, QString> GestCarac::GetCaracsQHash()
{
    QMap<QString, QString> caracsStr;

    QHash<QString, Carac*>::iterator it = m_Caracs.begin();
    for (;it != m_Caracs.end();++it) {
        caracsStr[it.key()] = it.value()->m_DataCarac.GetValeur();
    }

    return caracsStr;
}

void DPerso::RafraichirAffichage()
{
    Appliquer();
}

QString DPerso::GetValeurCarac(QString id)
{
    return GestCarac::GetCaracValue(id);
}

int DPerso::GetValeurCaracAsInt(QString id)
{
    return GestCarac::GetCaracValueAsInt(id);
}

MultiSetterDeCarac::MultiSetterDeCarac()
{

}

void MultiSetterDeCarac::Appliquer()
{
    for (int i = 0 ; i < this->m_SetCaracs.length() ; ++i ) {
        GestCarac::AppliquerSetCarac(*m_SetCaracs[i]);
    }
}

void DPerso::AppliquerSetCarac(const SetCarac& setCarac)
{
    ExecutionPreChangeCarac();
    GestCarac::AppliquerSetCarac(setCarac);
    ExecutionPostChangeCarac();
}

int DPerso::AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee)
{
    ExecutionPreChangeCarac();
    int res = GestCarac::AJouterValeurACaracId(idCarac, valeurAjoutee);
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
    int res = GestCarac::RetirerValeurACaracId(idCarac, valeurRetiree);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::SetValeurACaracId(const QString& idCarac,const  QString& valeurSet)
{
    ExecutionPreChangeCarac();
    QString res = GestCarac::SetValeurACaracId(idCarac, valeurSet);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::SetValeurACaracId(const QString& idCarac,const int& valeurSet)
{
    ExecutionPreChangeCarac();
    QString res = GestCarac::SetValeurACaracId(idCarac, valeurSet);
    ExecutionPostChangeCarac();
    return res;
}

QString DPerso::EffacerValeurACaracId(const QString& idCarac)
{
    ExecutionPreChangeCarac();
    QString res = GestCarac::EffacerValeurACaracId(idCarac);
    ExecutionPostChangeCarac();
    return res;
}

bool DPerso::IsCaracTrue(QString id)
{
    return GestCarac::IsCaracTrue(id);
}
