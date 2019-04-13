#include "condition.h"
#include "perso.h"
#include "univers.h"

Condition::Condition():m_CaracId(""), m_Valeur(""), m_Comparateur(c_Egal)
{

}

Condition::Condition(QString caracId, QString valeur, Comparateur comparateur):m_CaracId(caracId), m_Valeur(valeur), m_Comparateur(comparateur)
{

}

Condition::Condition(double proba):m_Proba(proba)
{

}

double Condition::CalculerProbaFinale()
{
    //On calcule la valeur réelle de la probabilité en fonction des modificateurs de prob :
    double probaFinale = m_Proba;
    for ( int j = 0 ; j < m_ModifsProba.size() ; j++)
    {
        if ( Condition::TesterTableauDeConditions(m_ModifsProba[j]->m_Conditions))
            probaFinale += m_ModifsProba[j]->m_Valeur;
    }
    return probaFinale;
}


Comparateur Condition::GetComparateurFromStr(QString compStr)
{
    if (compStr == "c_Egal") return Comparateur::c_Egal;
    else if (compStr == "c_Superieur") return Comparateur::c_Superieur;
    else if (compStr == "c_SuperieurEgal") return Comparateur::c_SuperieurEgal;
    else if (compStr == "c_InferieurEgal") return Comparateur::c_InferieurEgal;
    else if (compStr == "c_Inferieur") return Comparateur::c_Inferieur;
    else if (compStr == "c_Different") return Comparateur::c_Different;
    return Comparateur::c_Aucun;
}

QString Condition::GetStrFromComparateur(Comparateur comp)
{
    if (comp == c_Egal) return "c_Egal";
    else if (comp == c_Superieur) return "c_Superieur";
    else if (comp == c_SuperieurEgal) return "c_SuperieurEgal";
    else if (comp == c_InferieurEgal) return "c_InferieurEgal";
    else if (comp == c_Inferieur) return "c_Inferieur";
    else if (comp == c_Different) return "c_Different";
    return "c_Aucun";
}

bool Condition::Tester()
{
    // une condition doit soit reposer sur les proba soit sur une valeur fixe
    if ( abs(m_Proba  + 1.0) > 0.00001)
    //if ( fabs(m_Proba + 1) <= 0.0001)
    {
        double resProba = (double(qrand()%1000))/1000;

        return resProba <= CalculerProbaFinale();
    }
    else
    {
        QString valeurCarac;

        // la plupart des caracs viennent du perso mais certaines sont "spéciales" (si elles commencent par §)
        if ( m_CaracId == "§duree_effet_histoire")
        {
            valeurCarac = QString::number(
                        Univers::ME->GetHistoire()->EffetActuel(true)->GetTempEcoule());
        }
        else
            valeurCarac = Univers::ME->GetHistoire()->GetCaracValue(m_CaracId);

        switch (m_Comparateur) {
        case c_Egal:
            return (valeurCarac == m_Valeur );
            break;
        case c_Superieur:
            return (valeurCarac.toDouble() > m_Valeur.toDouble() );
            break;
        case c_SuperieurEgal:
        {

            //QString str = "1234.56";
            //double valTmp = str.toDouble();
            double val1 = valeurCarac.toDouble();

            bool ok;
            double val2 = m_Valeur.toDouble(&ok);
            Q_ASSERT_X(ok, " Condition::Tester", (QString("Conversion de m_Valeur : "  + m_Valeur + " impossible")).toStdString().c_str());
            return ( val1 >= val2 );
            break;
        }
        case c_Inferieur:
            return (valeurCarac.toDouble() < m_Valeur.toDouble() );
            break;
        case c_InferieurEgal:
            return (valeurCarac.toDouble() <= m_Valeur.toDouble() );
            break;
        case c_Different:
            return (valeurCarac != m_Valeur );
            break;
        default:
            break;
        }
    }

    QString msg = "Test de comparaison impossible car pas de type de condition déclaré " ;
    Q_ASSERT_X(false, "condition", msg.toStdString().c_str() );

    return false;
}

void Condition::RemplirListeCondition( QJsonObject objJson, QList<Condition*> &conditions, bool conditionsWhile)
{
    QList<QString> conditionsStrs;// = new QList();
    if ( conditionsWhile)
    {
        conditionsStrs.append("repeat_while_false");
        conditionsStrs.append("repeat_while");
        conditionsStrs.append("repeat_while_true");
    }
    else
    {
        conditionsStrs.append("if_false");
        conditionsStrs.append("if");
        conditionsStrs.append("if_true");
    }
    for ( int j = 0; j < conditionsStrs.size(); ++j)
    {
        if ( objJson.contains(conditionsStrs[j]) && objJson[conditionsStrs[j]].isArray())
        {
            QJsonArray conditionsArr = objJson[conditionsStrs[j]].toArray();
            for ( int i = 0; i < conditionsArr.size(); ++i)
            {
                Condition* condition = Condition::CreerConditionDepuisObject(
                            conditionsStrs[j], conditionsArr[i].toObject());
                conditions.append(condition);
            }
        }
    }
}

ModifProba* Condition::AjouterModifProba(double valeur, QList<Condition*> conditions)
{
    // les modificateurs de proba sont des conditions dans la condition qui affectent le probabilité de cette dernière :
    ModifProba* modifproba = new ModifProba( valeur);
    m_ModifsProba.append(modifproba);
    modifproba->m_Conditions = conditions;
    return modifproba;
}

Condition* Condition::CreerConditionDepuisObject(QString balise, QJsonObject obj)
{
    QString msg = "Tentative de créer une condition sans carac_id ! balise : " + balise;
    Q_ASSERT_X(( obj.contains("carac_id") && obj["carac_id"].isString()),
        "condition",
        msg.toStdString().c_str());

    Condition* condition = new Condition();
    condition->m_CaracId = obj["carac_id"].toString();
    if (balise == "if_false" || balise == "repeat_while_false" )
    {
        condition->m_Comparateur = c_Egal;
        condition->m_Valeur = "";
    }
    else if (balise == "if_true" || balise == "repeat_while_true" )
    {
        if ( obj.contains("valeur"))
        {
            condition->m_Comparateur = c_Egal;
            condition->m_Valeur = obj["valeur"].toString();
        }
        else
        {
            condition->m_Comparateur = c_Different;
            condition->m_Valeur = "";
        }
    }
    else if (balise == "if" || balise == "repeat_while" )
    {
        if ( obj.contains("valeur"))
        {
            condition->m_Comparateur = c_Egal;
            condition->m_Valeur = obj["valeur"].toString();
        }
        else
        {
            condition->m_Comparateur = c_Different;
            condition->m_Valeur = "";
        }

        if ( obj.contains("signe"))
        {
            QString signe = obj["signe"].toString();
            if ( signe == "<" )
            {
                condition->m_Comparateur = c_Inferieur;
            }
            else if ( signe == "<=" )
            {
                condition->m_Comparateur = c_InferieurEgal;
            }
            else if ( signe == ">" )
            {
                condition->m_Comparateur = c_Superieur;
            }
            else if ( signe == ">" )
            {
                condition->m_Comparateur = c_SuperieurEgal;
            }
            else if ( signe == "==" )
            {
                condition->m_Comparateur = c_Egal;
            }
            else if ( signe == "!=" )
            {
                condition->m_Comparateur = c_Different;
            }
        }
    }
    else
    {
        QString msg = "Tentative de créer une condition mais type de la balise non interprété  : " + balise;
        Q_ASSERT_X(false,
            "condition",
            msg.toStdString().c_str());
    }

    return condition;
}


bool Condition::TesterTableauDeConditions(QList<Condition*> &conditions)
{
    bool ret = true;
    for ( int i = 0; i < conditions.size(); ++i)
    {
        if (!conditions[i]->Tester())
            return false;
    }
    return ret;
}

/******************
    ModifProba
    */

ModifProba::ModifProba(double valeur):m_Valeur(valeur)
{

}
ModifProba::ModifProba()
{

}
