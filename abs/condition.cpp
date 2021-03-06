#include "condition.h"
#include "perso.h"
#include "univers.h"
#include <QSqlQuery>
#include <QtMath>
#include "../gestcarac.h"
#include <chrono>
#include <random>

using std::shared_ptr;
using std::make_shared;

Condition::Condition():m_CaracId(""), m_Valeur(""), m_Comparateur(c_Egal)
{}

Condition::Condition(QString caracId, QString valeur, Comparateur comparateur):m_CaracId(caracId), m_Valeur(valeur), m_Comparateur(comparateur)
{}

Condition::Condition(double proba, TypeProba typeProba):m_TypeProba(typeProba), m_Proba(proba)
{}

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

Comparateur Condition::GetComparateurFromStrSigne(QString compStr)
{
    if (compStr == "==") return Comparateur::c_Egal;
    else if (compStr == ">") return Comparateur::c_Superieur;
    else if (compStr == ">=") return Comparateur::c_SuperieurEgal;
    else if (compStr == "<=") return Comparateur::c_InferieurEgal;
    else if (compStr == "<") return Comparateur::c_Inferieur;
    else if (compStr == "!=") return Comparateur::c_Different;
    return Comparateur::c_Aucun;
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
    bool retour = true;

    QString valeurCarac;

    // la plupart des caracs viennent du perso mais certaines sont "spéciales" (si elles commencent par §)
    if ( m_CaracId == "§duree_effet_histoire")
    {
        valeurCarac = QString::number(
                    Univers::ME->GetExecHistoire()->EffetActuel(true)->GetTempEcoule());
    }
    else
        valeurCarac = IPerso::GetPersoCourant()->GetValeurCarac(m_CaracId);

    switch (m_Comparateur) {
    case c_Egal:
        retour = (valeurCarac == m_Valeur );
        break;
    case c_Superieur:
        retour = (valeurCarac.toDouble() > m_Valeur.toDouble() );
        break;
    case c_SuperieurEgal:
    {

        //QString str = "1234.56";
        //double valTmp = str.toDouble();
        double val1 = valeurCarac.toDouble();

        bool ok;
        double val2 = m_Valeur.toDouble(&ok);
        Q_ASSERT_X(ok, " Condition::Tester", (QString("Conversion de m_Valeur : "  + m_Valeur + " impossible")).toStdString().c_str());
        retour = ( val1 >= val2 );
        break;
    }
    case c_Inferieur:
        retour = (valeurCarac.toDouble() < m_Valeur.toDouble() );
        break;
    case c_InferieurEgal:
        retour = (valeurCarac.toDouble() <= m_Valeur.toDouble() );
        break;
    case c_Different:
        retour = (valeurCarac != m_Valeur );
        break;
    default:
        break;
    }

    // seules les probas pures sont utilisées comme testeuses d'exécution :
    if ( m_TypeProba == TypeProba::p_Pure)
    {
        double val = m_Proba  + 1.0;
        if ( retour && (qFabs(val) > 0.00001) )
        //if ( fabs(m_Proba + 1) <= 0.0001)
        {
            long long seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed);
            std::uniform_int_distribution<int> distribution(0, 1000);

            double resProba = (double(distribution(generator)))/1000;

            retour = resProba <= CalculerProbaFinale();
        }
    }

    return retour;
}

bool Condition::IsProbaPure()
{
    return m_TypeProba == p_Pure;
}

void Condition::ChargerModifProbaBdd()
{
    QString req_str = "SELECT * FROM d_ModifProba WHERE est_a_condition_id = " + QString::number(m_BDD_CondId);
    QSqlQuery query(req_str);
    while (query.next())
    {
       shared_ptr<ModifProba> mdfproba = this->AjouterModifProbaVide();
       mdfproba->m_Valeur = query.value("m_Valeur").toDouble();
       mdfproba->m_BDD_Id = query.value("id").toInt();

       mdfproba->ChargerConditionsBdd();
    }
}

void Condition::RemplirListeCondition( QJsonObject objJson, QList<shared_ptr<Condition>> &conditions, bool conditionsWhile)
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
                shared_ptr<Condition> condition = Condition::CreerConditionDepuisObject(
                            conditionsStrs[j], conditionsArr[i].toObject());
                conditions.append(condition);
            }
        }
    }
}

shared_ptr<ModifProba> Condition::AjouterModifProbaVide()
{
    // les modificateurs de proba sont des conditions dans la condition qui affectent le probabilité de cette dernière :
    shared_ptr<ModifProba> modifproba = make_shared<ModifProba>();
    m_ModifsProba.append(modifproba);
    return modifproba;
}

shared_ptr<ModifProba> Condition::AjouterModifProba(double valeur, QList<shared_ptr<Condition>> conditions)
{
    // les modificateurs de proba sont des conditions dans la condition qui affectent le probabilité de cette dernière :
    shared_ptr<ModifProba> modifproba = make_shared<ModifProba>( valeur);
    m_ModifsProba.append(modifproba);
    modifproba->m_Conditions = conditions;
    return modifproba;
}

shared_ptr<Condition> Condition::CreerConditionDepuisObject(QString balise, QJsonObject obj)
{
    QString msg = "Tentative de créer une condition sans carac_id ! balise : " + balise;
    Q_ASSERT_X(( obj.contains("carac_id") && obj["carac_id"].isString()),
        "condition",
        msg.toStdString().c_str());

    shared_ptr<Condition> condition = make_shared<Condition>();
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


bool Condition::TesterTableauDeConditions(QList<shared_ptr<Condition>> &conditions)
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
{}
ModifProba::ModifProba()
{}


void ModifProba::ChargerConditionsBdd()
{
    QString req_str = "SELECT * FROM d_Condition WHERE est_a_modif_proba_id = " + QString::number(m_BDD_Id);
    QSqlQuery query(req_str);
    while (query.next())
    {
       shared_ptr<Condition> cond = make_shared<Condition>();

       int id = query.value("id").toInt();
       QString compStr = query.value("m_Comparateur").toString();
       Comparateur comparateur = Condition::GetComparateurFromStr(compStr);

       Q_ASSERT_X( query.value("m_Proba").toDouble() < 0,
                     "Une proba dans la condition d'un modif proba ça ne me semble pas naturel !",
                     "ChargerConditionsBdd");

       cond->m_Valeur = query.value("m_Valeur").toString();
       cond->m_CaracId = query.value("m_CaracId").toString();
       cond->m_Comparateur = comparateur;
       cond->m_BDD_CondId = id;

       m_Conditions.push_back(cond);
    }
}
