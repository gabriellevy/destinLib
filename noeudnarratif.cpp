#include "noeudnarratif.h"
#include "univers.h"
#include <QSqlQuery>
#include "gestionnairecarac.h"
#include "choix.h"

NoeudNarratif::NoeudNarratif()
{
    m_TypeNoeud = TypeNoeud::etn_NoeudNarratif;
}

NoeudNarratif::NoeudNarratif(QString id,
      QString nom,
      QString text):NoeudNarratif()
{
    m_Id = id;
    m_Nom = nom;
    m_Texte = text;
}

bool NoeudNarratif::TesterConditions()
{
    return Noeud::TesterConditions() &&
            Condition::TesterTableauDeConditions(this->m_RepeatWhileConditions);
}

void NoeudNarratif::AppliquerValeurDeNoeudBDD(int bd_id)
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
       this->m_Texte = query.value("m_Text").toString();
       //this->m_CheminImg = query.value("m_CheminImg").toString();

        this->ChargerConditionsBdd();
        this->ChargerSetCaracBdd();
        this->ChargerFonctionsCallbacksBdd();
        this->ChargerFonctionsTestCallbacksBdd();
        this->ChargerSelectionneurEvtBdd();
    }
}

void NoeudNarratif::ChargerSelectionneurEvtBdd()
{
    QString req_str = "SELECT * FROM d_SelectionneurDEvt WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId);
    QSqlQuery query(req_str);

    while (query.next())
    {
        int bdd_id = query.value("id").toInt();
        QString intitule = query.value("intitule").toString();
        // vérifier si ce sélectionneur a déjà été créé depuis la bdd :
        for ( SelectionneurDeNoeud* sel: SelectionneurDeNoeud::s_TousLesSelectionneurs)
        {
            if ( sel->m_BddId == bdd_id) {
                this->m_SelectionneurDeNoeud = sel;
                return;
            }
        }

        // pas trouvé : on le crée
        SelectionneurDeNoeud* sel = new SelectionneurDeNoeud(intitule, bdd_id);
        this->m_SelectionneurDeNoeud = sel;
        SelectionneurDeNoeud::s_TousLesSelectionneurs.push_back(sel);
    }
}

NoeudNarratif::~NoeudNarratif()
{
    while (!m_RepeatWhileConditions.isEmpty())
          delete m_RepeatWhileConditions.takeFirst();
}

QString NoeudNarratif::TexteAAfficher()
{
    QString texteFinal = "";

    QStringList list = m_Texte.split("%%%");
    for ( int i = 0 ; i < list.size() ; ++i)
    {
       if ( i %2 == 0)
       {
           texteFinal += list.at(i);
       }
       else
       {
           // est forcément une variable à remplacer
           texteFinal += GestionnaireCarac::GetCaracValue(list.at(i));
       }
    }

   return texteFinal;
}


void NoeudNarratif::ChargerFonctionsCallbacksBdd()
{
    QString req_str = "SELECT * FROM d_FonctionCallback WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId) +
            " AND est_callback_test = 0";
    QSqlQuery query(req_str);
    while (query.next())
    {
       AppelCallback* appel = new AppelCallback(query.value("fonction_id").toString());
       appel->m_BDD_FonctId = query.value("id").toInt();

       appel->ChargerArgumentsBdd();
       m_FonctionsAppellees.push_back(appel);
    }
}

bool NoeudNarratif::AQuelqueChoseAAfficher()
{
    return (m_Texte != "" || this->m_ImgPath != "" || m_Nom != "" || m_FilmPath != "" ||
            m_CallbackDisplay != nullptr || m_LancerDe != nullptr);
}


void NoeudNarratif::ChangerChrono( int ms )
{
    m_MsChrono = ms;
}

void NoeudNarratif::ChargerFonctionsTestCallbacksBdd()
{
    QString req_str = "SELECT * FROM d_FonctionCallback WHERE est_a_noeud_id = " + QString::number(m_BDD_NoeudId) +
            " AND est_callback_test = 1";
    QSqlQuery query(req_str);
    while (query.next())
    {
       AppelCallback* appel = new AppelCallback(query.value("fonction_id").toString());
       appel->m_BDD_FonctId = query.value("id").toInt();

       appel->ChargerArgumentsBdd();
       m_FonctionsDeTest.push_back(appel);
    }
}

void NoeudNarratif::ChargerConditionsBdd()
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

void NoeudNarratif::ChargerSetCaracBdd()
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
        setC->m_IdValeurCaracCopie = query.value("m_ValeurCarac").toString();
        setC->m_ValeurRandom = query.value("m_ValeurRandom").toString();
    }
}


void NoeudNarratif::AjouterDuree(float duree)
{
    m_TempEcoule += duree;
}

double NoeudNarratif::GetTempEcoule()
{
    return m_TempEcoule;
}

