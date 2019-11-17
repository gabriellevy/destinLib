#include "noeud.h"
//#include "ui_noeud.h"
#include "univers.h"
#include <QDebug>
#include <QSqlQuery>
#include "histoire.h"

Noeud::Noeud():m_TypeNoeud(TypeNoeud::etn_Noeud)
{
    m_ChangeurModeDeroulement = ModeDeroulement::Aucun;
}

Noeud::Noeud(QString id):Noeud()
{
    m_Id = id;
}

SetCarac* Noeud::AjouterRetireurACarac(QString id, int valeur)
{
    SetCarac* set_carac = new SetCarac(ModifCaracType::RetireDeCarac, id, QString::number(valeur));
    m_SetCaracs.append(set_carac);
    return set_carac;
}

SetCarac* Noeud::AjouterRetireurACarac(QString id, QString valeur)
{
    SetCarac* set_carac = new SetCarac(ModifCaracType::RetireDeCarac, id, valeur);
    m_SetCaracs.append(set_carac);
    return set_carac;
}

SetCarac* Noeud::AjouterAjouteurACarac(QString id, QString valeur)
{
    SetCarac* set_carac = new SetCarac(ModifCaracType::AddToCarac, id, valeur);
    m_SetCaracs.append(set_carac);
    return set_carac;
}

SetCarac* Noeud::AjouterAjouteurACarac(QString id, int valeur)
{
    return AjouterAjouteurACarac(id, QString::number(valeur));
}

SetCarac* Noeud::AjouterChangeurDeCarac(QString id, QString valeur)
{
    SetCarac* set_carac = new SetCarac(ModifCaracType::SetCarac, id, valeur);
    m_SetCaracs.append(set_carac);
    return set_carac;
}

SetCarac* Noeud::AjouterSetCaracTrue(QString id)
{
    SetCarac* set_carac = new SetCarac(ModifCaracType::SetCarac, id, "1");
    m_SetCaracs.append(set_carac);
    return set_carac;
}

Condition* Noeud::AjouterCondition( QString caracId, Comparateur comparateur, QString valeur)
{
    Condition* condition = new Condition(caracId, valeur, comparateur );

    m_Conditions.append(condition);

    return condition;
}

Condition* Noeud::AjouterConditionProba( double proba)
{
    Condition* conditionProba = new Condition( proba );

    m_Conditions.append(conditionProba);

    return conditionProba;
}
Noeud::~Noeud()
{
    while (!m_Conditions.isEmpty())
          delete m_Conditions.takeFirst();

    /*while ( m_SetCaracs.size() > 0 )
        delete m_SetCaracs.takeAt(0);*/
}

bool Noeud::TesterConditions()
{
    bool resultatCallback = true;

    foreach(AppelCallback* appel, m_FonctionsDeTest)
    {
       resultatCallback = resultatCallback &&
               Univers::ME->GetExecHistoire()->AppelerFonctionCallback(
                   appel->m_NomFonction,
                   appel->m_ArgumentsCaracId,
                   appel->m_ArgumentsParValeur);
        Univers::ME->GetPersoInterface()->RafraichirAffichage();
    }

    bool res = ( resultatCallback &&
        Condition::TesterTableauDeConditions(m_Conditions));

    return res;
}

void AppelCallback::ChargerArgumentsBdd()
{
    QString req_str = "SELECT * FROM d_CallbackArgument WHERE est_a_callback_id = " + QString::number(m_BDD_FonctId);
    QSqlQuery query(req_str);
    while (query.next())
    {
        // un argument issu de la BDD ne peut être que une référence à une carac du jeu ou une valeur string enregistrée brute
        QString argCaracId = query.value("caracIdParam").toString();
        if ( argCaracId != "") {
            this->m_ArgumentsCaracId.push_back(argCaracId);
        } else {
            this->m_ArgumentsParValeur.push_back(query.value("valeurBrute").toString());
        }
    }
}

double Noeud::GetProba()
{
    float proba = 0;

    // on vérifie si il y a au moins une condition de proba dans ce noeud :
    for ( int i = 0; i < m_Conditions.size() ; ++i)
    {
        if ( m_Conditions[i]->CalculerProbaFinale() != -1.0f)
        {
            proba = m_Conditions[i]->CalculerProbaFinale();
            break;
        }
    }

    return proba;
}
