#include "noeud.h"
//#include "ui_noeud.h"
#include "univers.h"
#include <QDebug>
#include <QSqlQuery>

Noeud::Noeud():m_TypeNoeud(TypeNoeud::etn_Noeud)
{
    m_Film = nullptr;
}

Noeud::Noeud(QString id,
      QString nom,
      QString text):Noeud()
{
    m_Film = nullptr;
    m_Id = id;
    m_Nom = nom;
    m_Text = text;
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

    if ( objJson.contains("themes") && objJson["themes"].isArray())
    {
        QJsonArray themes = objJson["themes"].toArray();
        for ( int i = 0; i < themes.size(); ++i)
            m_Themes.append(themes[i].toString());
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

void Noeud::ChargerSelectionneurEvtBdd()
{
    QString req_str = "SELECT * FROM d_SelectionneurDEvt WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId);
    QSqlQuery query(req_str);

    while (query.next())
    {
        int bdd_id = query.value("id").toInt();
        QString intitule = query.value("intitule").toString();
        // vérifier si ce sélectionneur a déjà été créé depuis la bdd :
        for ( SelectionneurDEvenement* sel: SelectionneurDEvenement::s_TousLesSelectionneurs)
        {
            if ( sel->m_BddId == bdd_id) {
                this->m_SelectionneurDEvenement = sel;
                return;
            }
        }

        // pas trouvé : on le crée
        SelectionneurDEvenement* sel = new SelectionneurDEvenement(intitule, bdd_id);
        this->m_SelectionneurDEvenement = sel;
        SelectionneurDEvenement::s_TousLesSelectionneurs.push_back(sel);
    }
}

void Noeud::AppliquerValeurDeNoeudBDD(int bd_id)
{
    QString req_str = "SELECT * FROM d_Noeud WHERE id = " + QString::number(bd_id);
    QSqlQuery query(req_str);
    Q_ASSERT_X(query.size() <= 1, "Plus d'un noeud de la BDD retourné par la requête.", "Noeud::AppliquerValeurDeNoeudBDD");
    while (query.next())
    {
       this->m_BDD_NoeudId = query.value("id").toInt();
       this->m_Duree = query.value("m_Duree").toFloat();
       this->m_ChangePerso = query.value("m_ChangePerso").toString();
       this->m_Id = query.value("m_Id").toString();
       this->m_Nom = query.value("m_Nom").toString();
       this->m_Son = query.value("m_Son").toString();
       this->m_GoToEvtId = query.value("m_GoToEvtId").toString();
       this->m_GoToEffetId = query.value("m_GoToEffetId").toString();
       this->m_Text = query.value("m_Text").toString();
       //this->m_CheminImg = query.value("m_CheminImg").toString();

        this->ChargerConditionsBdd();
        this->ChargerSetCaracBdd();
        this->ChargerFonctionsCallbacksBdd();
        this->ChargerFonctionsTestCallbacksBdd();
        this->ChargerSelectionneurEvtBdd();
    }
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

void Noeud::ChargerFonctionsCallbacksBdd()
{
    QString req_str = "SELECT * FROM d_FonctionCallback WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId) +
            " AND est_callback_test = 0";
    QSqlQuery query(req_str);
    while (query.next())
    {
       AppelCallback* appel = new AppelCallback();
       appel->m_NomFonction = query.value("fonction_id").toString();
       appel->m_BDD_FonctId = query.value("id").toInt();

       appel->ChargerArgumentsBdd();
       m_FonctionsAppellees.push_back(appel);
    }
}

void Noeud::ChargerFonctionsTestCallbacksBdd()
{
    QString req_str = "SELECT * FROM d_FonctionCallback WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId) +
            " AND est_callback_test = 1";
    QSqlQuery query(req_str);
    while (query.next())
    {
       AppelCallback* appel = new AppelCallback();
       appel->m_NomFonction = query.value("fonction_id").toString();
       appel->m_BDD_FonctId = query.value("id").toInt();

       appel->ChargerArgumentsBdd();
       m_FonctionsDeTest.push_back(appel);
    }
}

void Noeud::ChargerConditionsBdd()
{
    // A FAIRE : ne prend pas en compte les conditions de type repeat while
    QString req_str = "SELECT * FROM d_Condition WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId);
    QSqlQuery query(req_str);
    while (query.next())
    {
       int id = query.value("id").toInt();
       QString compStr = query.value("m_Comparateur").toString();
       Comparateur comparateur = Condition::GetComparateurFromStr(compStr);
       double proba = query.value("m_Proba").toDouble();

       Condition* cond = nullptr;

       if ( proba <0) {
           // condition de base
           cond = this->AjouterCondition(
                       query.value("m_CaracId").toString(),
                       comparateur,
                       query.value("m_Valeur").toString());
       } else {
            // condition à base de proba
           cond = this->AjouterConditionProba(proba);
           cond->m_CaracId = query.value("m_CaracId").toString();
           cond->m_Comparateur = comparateur;
           cond->m_Valeur = query.value("m_Valeur").toString();
        }

       if ( cond != nullptr) {
           cond->m_BDD_CondId = id;
           cond->ChargerModifProbaBdd();
       }
    }
}

void Noeud::ChargerSetCaracBdd()
{
    QString req_str = "SELECT * FROM d_SetCarac WHERE noeud_id = " + QString::number(m_BDD_NoeudId);
    QSqlQuery query(req_str);
    while (query.next())
    {
        ModifCaracType eType = SetCarac::GetModifCaracTypeFromQString(query.value("m_ModifCaracType").toString());

        QString idCarac = query.value("m_CaracId").toString();
        QString valeur = query.value("m_Valeur").toString();
        SetCarac* setC = nullptr;

        switch (eType)
        {
        case ModifCaracType::SetCarac :
            setC = this->AjouterChangeurDeCarac(idCarac, valeur);
            break;
        case ModifCaracType::AddToCarac:
            setC = this->AjouterAjouteurACarac(idCarac, valeur);
            break;
        case ModifCaracType::RetireDeCarac:
            setC = this->AjouterRetireurACarac(idCarac, valeur);
            break;
        }

        setC->m_ValeurRandom = query.value("m_ValeurRandom").toString();
        setC->m_ValeurMin = query.value("m_ValeurMin").toString();
        setC->m_ValeurMax = query.value("m_ValeurMax").toString();
        setC->m_ValeurCarac = query.value("m_ValeurCarac").toString();
        setC->m_ValeurRandom = query.value("m_ValeurRandom").toString();
    }
}

double Noeud::GetTempEcoule()
{
    return m_TempEcoule;
}

Noeud::~Noeud()
{
    while (!m_Conditions.isEmpty())
          delete m_Conditions.takeFirst();

    while (!m_RepeatWhileConditions.isEmpty())
          delete m_RepeatWhileConditions.takeFirst();

    /*while ( m_SetCaracs.size() > 0 )
        delete m_SetCaracs.takeAt(0);*/
}

 void Noeud::AfficherNoeud()
 {
     // commenté à cause des noeuds else (surement entre autres)
     /*QString msg = "AfficherNoeud ne doit pas être appelé sur un Noeud mais seulement sur des objets en héritant " ;
     Q_ASSERT_X(false, "AfficherNoeud", msg.toStdString().c_str() );*/
 }

 QString Noeud::TexteAAfficher()
 {
     QString texteFinal = "";

     QStringList list = m_Text.split("%%%");
     for ( int i = 0 ; i < list.size() ; ++i)
     {
        if ( i %2 == 0)
        {
            texteFinal += list.at(i);
        }
        else
        {
            // est forcément une variable à remplacer
            texteFinal += Univers::ME->GetHistoire()->GetCaracValue(list.at(i));
        }
     }

    return texteFinal;
 }


 void Noeud::AjouterDuree(float duree)
 {
     m_TempEcoule += duree;
 }


 bool Noeud::GestionTransition()
 {
     // commenté à cause des noeuds else (surement entre autres)
     //Q_ASSERT_X(false, "Noeud::GestionTransition", "Je ne crois pas que la gestion de transition devrait se faire dans Noeud si ?");
    Univers::ME->GetHistoire()->DeterminerPuisLancerNoeudSuivant(this);
    return true;
 }

 void Noeud::LancerNoeud()
 {
     if ( this->AQuelqueChoseAAfficher() )
         this->AfficherNoeud();

     if ( m_Son != "" )
     {
        Univers::ME->m_Lecteur->stop();
        Univers::ME->m_Lecteur->setMedia(QUrl(m_Son));
        Univers::ME->m_Lecteur->setVolume(50);
        if ( Univers::ME->m_Reglages.m_SonOn )
            Univers::ME->m_Lecteur->play();
     }

     this->ExecuterActionsNoeud();

     bool transition_auto = this->GestionTransition( );

     if (!transition_auto || Univers::ME->GetEtatPartie() == EP_FinPartie)
         Univers::ME->GetHistoire()->RafraichirAffichageEvtEtOuEffet( nullptr, nullptr );
}

void Noeud::FinExecutionNoeud()
{
    bool afficheFilm = (m_Film != nullptr);
    if ( afficheFilm )
        m_Film->stop();

    // l'exécution de ce noeud est terminée. Lors de la prochaine itération il faudra refaire le test avec les nouvelles valeurs
    //m_EtatCondition = ec_NonTeste;
}

void Noeud::ExecuterActionsNoeud(/*bool afficherNoeud, bool lancerNoeudSuivantSiRienAAfiicher*/)
{
    // si il y un champs de temps, il s'écoule :
    // pour le noeud courant
    AjouterDuree( m_Duree );
    // pour l'aventure complète
    Univers::ME->AjouterDuree(m_Duree);
    // si on est en mode aléatoire, le temps s'écoule aussi pour le noeud histoire qui a fait appel à l'aléatoire
    if ( Univers::ME->GetTypeEvtActuel() == TE_Aleatoire)
        Univers::ME->GetHistoire()->AjouterDureeAEffetHistoireCourant(m_Duree);

    // maj du perso :
    if ( m_ChangePerso != "")
    {
        IPerso::GetPersoInterface()->ChangerPersoCourant(m_ChangePerso);
    }

    /*int index = 0;
    foreach ( std::function<void(QVector<QString>)> f_CallbackFunction, m_CallbackFunctions)
    {
        f_CallbackFunction(m_CallbackArguments[index++]);
        Univers::ME->GetPersoInterface()->RafraichirAffichage();
    }*/

    foreach(AppelCallback* appel, m_FonctionsAppellees)
    {
        Univers::ME->GetHistoire()->AppelerFonctionCallback(
                    appel->m_NomFonction,
                    appel->m_ArgumentsCaracId,
                    appel->m_ArgumentsParValeur);
        Univers::ME->GetPersoInterface()->RafraichirAffichage();
    }

    // maj des caracs
    if ( m_SetCaracs.size()>0)
    {
        IPerso* perso = Univers::ME->GetPersoInterface();
        for ( int i = 0 ; i < m_SetCaracs.size() ; ++i)
        {
            Univers::ME->GetHistoire()->AppliquerCarac(*m_SetCaracs[i] );
        }
        perso->RafraichirAffichage();
    }

    // mise à jour de l'état de la partie :
    if ( m_NouvelEtatPartie != "" )
        Univers::ME->ChangerEtatPartie(m_NouvelEtatPartie);

    /*if (this->AQuelqueChoseAAfficher() && afficherNoeud)
    {
        Aventure::ME->AfficherEvtEtOuEffetCourant();
    }
    else if ( lancerNoeudSuivantSiRienAAfiicher)
    {
        Histoire* hist = Aventure::ME->GetHistoire();

        hist->DeterminerPuisExecuterEffetSuivant(this);
    }*/
}

bool Noeud::AQuelqueChoseAAfficher()
{
    return (m_Text != "" || !m_Img.isNull() || m_Nom != "" || m_Film != nullptr);
}

bool Noeud::TesterConditions()
{
    /*if ( m_EtatCondition != ec_NonTeste)
    {
        // cette condition a déjà été testé une fois : on ne la reteste aps car la proba aléatoire pourrait donner des résultat différents cette fois
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
               Univers::ME->GetHistoire()->AppelerFonctionCallback(
                   appel->m_NomFonction,
                   appel->m_ArgumentsCaracId,
                   appel->m_ArgumentsParValeur);
        Univers::ME->GetPersoInterface()->RafraichirAffichage();
    }

    bool res = ( resultatCallback &&
        Condition::TesterTableauDeConditions(m_Conditions) &&
        Condition::TesterTableauDeConditions(this->m_RepeatWhileConditions));

    /*if ( res )
        m_EtatCondition = ec_True;
    else m_EtatCondition = ec_False;*/

    return res;
}


void Noeud::ChangerChrono( int ms )
{
    m_MsChrono = ms;
}


/*void Noeud::AjouterCallback(std::function<void(QVector<QString>)> callback, QVector<QString> arg)
{
    m_CallbackFunctions.push_back( callback );
    m_CallbackArguments.push_back( arg);
}


void Noeud::AjouterCallbackDeTest(std::function<bool(QVector<QString>)> callback, QVector<QString> arg)
{
    m_CallbackTestFunctions.push_back( callback);
    m_CallbackTestArguments.push_back( arg);
}*/

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
