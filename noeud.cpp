#include "noeud.h"
//#include "ui_noeud.h"
#include "univers.h"
#include <QDebug>
#include <QSqlQuery>

Noeud::Noeud():m_TypeNoeud(TypeNoeud::etn_Noeud)
{
}

Noeud::Noeud(QString id):Noeud()
{
    m_Id = id;
}

/*Noeud::Noeud(QJsonObject objJson)
{
    //ui->setupUi(this);
    m_Film = nullptr;
    m_Id = "";
    m_Nom = "";
    //m_ImgPath = "";
    m_GoToEvtId = "";
    m_GoToEffetId = "";
    m_NouvelEtatPartie = "";
    m_Duree = 0;
    m_TempEcoule = 0;

    if ( objJson.contains("id") && objJson["id"].isString())
    {
        m_Id = objJson["id"].toString();
    }

    if ( objJson.contains("change_perso") && objJson["change_perso"].isString())
    {
        m_ChangePerso = objJson["change_perso"].toString();
    }

    if ( objJson.contains("etat_partie") && objJson["etat_partie"].isString())
    {
        m_NouvelEtatPartie = objJson["etat_partie"].toString();
    }

    if ( objJson.contains("nom") && objJson["nom"].isString())
    {
        m_Nom = objJson["nom"].toString();
    }

    if ( objJson.contains("text") && objJson["text"].isString())
    {
        m_Text = objJson["text"].toString();
    }

    if ( objJson.contains("img") && objJson["img"].isString())
    {
        m_Img.load( objJson["img"].toString());
    }

    if ( objJson.contains("go_to_evt") && objJson["go_to_evt"].isString())
    {
        m_GoToEvtId = objJson["go_to_evt"].toString();
    }

    if ( objJson.contains("duree") && objJson["duree"].isString())
    {
        m_Duree = objJson["duree"].toString().toFloat();
    }

    if ( objJson.contains("go_to_effet") && objJson["go_to_effet"].isString())
    {
        m_GoToEffetId = objJson["go_to_effet"].toString();
    }

    if ( objJson.contains("set_carac") && objJson["set_carac"].isArray())
    {
        QJsonArray set_caracs = objJson["set_carac"].toArray();
        for ( int i = 0; i < set_caracs.size(); ++i)
        {
            SetCarac set_carac(set_caracs[i].toObject(), ModifCaracType::SetCarac);
            m_SetCaracs.append(set_carac);
        }
    }
    if ( objJson.contains("set_carac_false") && objJson["set_carac_false"].isArray())
    {
        QJsonArray set_caracs = objJson["set_carac_false"].toArray();
        for ( int i = 0; i < set_caracs.size(); ++i)
        {
            SetCarac set_carac(set_caracs[i].toObject(), ModifCaracType::SetCarac, "0");
            m_SetCaracs.append(set_carac);
        }
    }
    if ( objJson.contains("set_carac_true") && objJson["set_carac_true"].isArray())
    {
        QJsonArray set_caracs = objJson["set_carac_true"].toArray();
        for ( int i = 0; i < set_caracs.size(); ++i)
        {
            SetCarac set_carac(set_caracs[i].toObject(), ModifCaracType::SetCarac, "1");
            m_SetCaracs.append(set_carac);
        }
    }
    if ( objJson.contains("add_to_carac") && objJson["add_to_carac"].isArray())
    {
        QJsonArray set_caracs = objJson["add_to_carac"].toArray();
        for ( int i = 0; i < set_caracs.size(); ++i)
        {
            SetCarac set_carac(set_caracs[i].toObject(), ModifCaracType::AddToCarac);
            m_SetCaracs.append(set_carac);
        }
    }
    if ( objJson.contains("retire_de_carac") && objJson["retire_de_carac"].isArray())
    {
        QJsonArray set_caracs = objJson["retire_de_carac"].toArray();
        for ( int i = 0; i < set_caracs.size(); ++i)
        {
            SetCarac set_carac(set_caracs[i].toObject(), ModifCaracType::RetireDeCarac);
            m_SetCaracs.append(set_carac);
        }
    }

    //noeuds de condition :
    Condition::RemplirListeCondition( objJson, m_Conditions, false);

    Condition::RemplirListeCondition( objJson, m_RepeatWhileConditions, true);

    // noeuds de proba (= noeud de condition particuliers)
    if ( objJson.contains("if_proba") && objJson["if_proba"].isString())
    {
        Condition* conditionProba = new Condition( objJson["if_proba"].toString().toDouble() );

        // les modificateurs de proba sont des conditions dans la condition qui affectent le probabilité de cette dernière :
        if ( objJson.contains("modif_proba") && objJson["modif_proba"].isArray())
        {
            QJsonArray modifprobasArr = objJson["modif_proba"].toArray();
            for ( int i = 0; i < modifprobasArr.size(); ++i)
            {
                QJsonObject modifProbaObj = modifprobasArr[i].toObject();

                Q_ASSERT_X(modifProbaObj.contains("valeur"),
                           "création de modifprobas d'un noeud", "un noeud modif proba n'a pas de valeur!");

                float valeur = modifProbaObj["valeur"].toString().toFloat();
                ModifProba* modifproba = new ModifProba( valeur);
                conditionProba->m_ModifsProba.append(modifproba);
                Condition::RemplirListeCondition(modifProbaObj, modifproba->m_Conditions, false);
            }
        }

        m_Conditions.append(conditionProba);
    }
}*/

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


/*bool Noeud::AUnDeCesThemes(QList<QString> themes)
{
    for ( int i = 0 ; i < themes.size() ; ++i)
    {
        for ( int j = 0 ; j < m_Themes.size() ; ++j)
            if ( themes[i] == m_Themes[j] )
                return true;
    }
    return false;
}*/

Noeud::~Noeud()
{
    while (!m_Conditions.isEmpty())
          delete m_Conditions.takeFirst();

    /*while ( m_SetCaracs.size() > 0 )
        delete m_SetCaracs.takeAt(0);*/
}

bool Noeud::TesterConditions()
{
    /*if ( m_EtatCondition != ec_NonTeste)
    {
        // cette condition a déjà été testé une fois : on ne la reteste pas car la proba aléatoire pourrait donner des résultat différents cette fois
        return ( m_EtatCondition == ec_True);
    }*/

    bool resultatCallback = true;


    /*int index = 0;
    foreach ( std::function<bool(QVector<QString>)> f_CallbackFunction, m_CallbackTestFunctions)
    {
        resultatCallback = resultatCallback && f_CallbackFunction(m_CallbackTestArguments[index++]);
    }*/

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

    /*if ( res )
        m_EtatCondition = ec_True;
    else m_EtatCondition = ec_False;*/

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
