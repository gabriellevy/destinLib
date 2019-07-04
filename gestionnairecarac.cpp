#include "gestionnairecarac.h"

#include "univers.h"

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

int GestionnaireCarac::AJouterValeurACaracId(QString idCarac, int valeurAjoutee)
{
    Univers::ME->GetExecHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::AddToCarac, idCarac, QString::number(valeurAjoutee)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

int GestionnaireCarac::RetirerValeurACaracId(QString idCarac, int valeurRetiree)
{
    Univers::ME->GetExecHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::RetireDeCarac, idCarac, QString::number(valeurRetiree)));
    return GestionnaireCarac::GetCaracValueAsInt(idCarac);
}

QString GestionnaireCarac::SetValeurACaracId(QString idCarac, QString valeurSet)
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
