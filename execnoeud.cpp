#include "execnoeud.h"
#include "univers.h"
#include <QMovie>
#include "noeudnarratif.h"
#include "gestionnairecarac.h"
#include "execchoix.h"

ExecNoeud::ExecNoeud(NoeudNarratif* noeud, QWidget* parent):QWidget (parent), m_Noeud(noeud)
{}

/*ExecNoeud::~ExecNoeud()
{
    //delete ui;
}*/

void ExecNoeud::FinExecutionNoeud()
{
   bool afficheFilm = (m_Film != nullptr);
   if ( afficheFilm )
       m_Film->stop();

   // l'exécution de ce noeud est terminée. Lors de la prochaine itération il faudra refaire le test avec les nouvelles valeurs
   //m_EtatCondition = ec_NonTeste;
}

void ExecNoeud::ExecuterActionsNoeud(Noeud* noeudAExecuter/*bool afficherNoeud, bool lancerNoeudSuivantSiRienAAfiicher*/)
{
    bool executionNoeudNarratif = false; // si false il s'agit seulement de l'exécution d'un noeud de base qui change juste des caracs (par exemple)
    if ( noeudAExecuter == nullptr) {
        noeudAExecuter = m_Noeud; // cas le plus courant
        executionNoeudNarratif = true;
    }

    if ( noeudAExecuter->m_ChangeurModeDeroulement != ModeDeroulement::Aucun) {
        Univers::ME->GetExecHistoire()->m_Histoire->m_ModeDeroulement = noeudAExecuter->m_ChangeurModeDeroulement;
    }

    // si il y un champs de temps, il s'écoule :
    // pour le noeud courant
    if ( executionNoeudNarratif ) {
        this->m_Noeud->AjouterDuree( m_Noeud->m_Duree );
        // pour l'aventure complète
        Univers::ME->AjouterDuree(m_Noeud->m_Duree);
        // si on est en mode aléatoire, le temps s'écoule aussi pour le noeud histoire qui a fait appel à l'aléatoire
        if ( Univers::ME->GetTypeEvtActuel() == TE_Aleatoire)
            Univers::ME->GetExecHistoire()->AjouterDureeAEffetHistoireCourant(m_Noeud->m_Duree);
    }

    // maj du perso :
    if ( noeudAExecuter->m_ChangePerso != QLatin1String(""))
    {
        IPerso::GetPersoInterface()->ChangerPersoCourant(noeudAExecuter->m_ChangePerso);
    }

    /*int index = 0;
    foreach ( std::function<void(QVector<QString>)> f_CallbackFunction, m_CallbackFunctions)
    {
        f_CallbackFunction(m_CallbackArguments[index++]);
        Univers::ME->GetPersoInterface()->RafraichirAffichage();
    }*/

    foreach(AppelCallback* appel, noeudAExecuter->m_FonctionsAppellees)
    {
        Univers::ME->GetExecHistoire()->AppelerFonctionCallback(
                    appel->m_NomFonction,
                    appel->m_ArgumentsCaracId,
                    appel->m_ArgumentsParValeur);
        Univers::ME->RafraichirAffichage();
    }

    if ( noeudAExecuter->m_CallbackDisplay != nullptr) {
        noeudAExecuter->m_CallbackDisplay();
    }

    // maj des caracs
    if ( noeudAExecuter->m_SetCaracs.size()>0)
    {
        IPerso* perso = Univers::ME->GetPersoInterface();
        for ( int i = 0 ; i < noeudAExecuter->m_SetCaracs.size() ; ++i)
        {
            IPerso::GetPersoCourant()->AppliquerSetCarac(*noeudAExecuter->m_SetCaracs[i] );
        }
        perso->RafraichirAffichage();
    }
}

void ExecNoeud::LancerNoeud()
{
    this->ExecuterActionsNoeud();

    if ( this->m_Noeud->AQuelqueChoseAAfficher() && this->m_Noeud->m_TypeNoeud != TypeNoeud::etn_Choix)
        this->AfficherNoeud();

    if ( m_Noeud->m_Son != QLatin1String("") )
    {
       Univers::ME->m_Lecteur->stop();
       Univers::ME->m_Lecteur->setMedia(QUrl(m_Noeud->m_Son));
       Univers::ME->m_Lecteur->setVolume(50);
       if ( Univers::ME->m_Reglages.m_SonOn )
           Univers::ME->m_Lecteur->play();
    }

    bool transition_auto = this->GestionTransition( );

    if (!transition_auto)
        Univers::ME->GetExecHistoire()->RafraichirAffichageEvtEtOuEffet( nullptr, nullptr );
}


void ExecNoeud::AjouterAuxBoutonsHoriz(ExecNoeud* )
{
    Q_ASSERT_X(true, "cette fonction doit être surclassée !", "ExecNoeud::AjouterAuxBoutonsHoriz");
}

void ExecNoeud::AjouterAuxBoutonsVertic(ExecNoeud* )
{
    Q_ASSERT_X(true, "cette fonction doit être surclassée !", "ExecNoeud::AjouterAuxBoutonsVertic");
}

bool ExecNoeud::GestionTransition()
{
    bool est_ce_que_l_interface_vers_suite_est_affichee = false;
    // si l'effet contient un choix on l'affiche et on considère que le passage vers l'effet suivant est géré par les choix
    if ( m_ExecChoix.size() > 0 )
    {
        for ( int i = 0 ; i < m_ExecChoix.size() ; ++i)
        {
            m_ExecChoix[i]->GetExecNoeud()->hide();
            if ( m_ExecChoix[i]->m_Choix->TesterConditions())
            {
                if ( m_Noeud->m_OrientationAffichageChoix == OrientationAffichageChoix::oac_vertical)
                    AjouterAuxBoutonsVertic(m_ExecChoix[i]->GetExecNoeud());
                else
                    AjouterAuxBoutonsHoriz(m_ExecChoix[i]->GetExecNoeud());
                m_ExecChoix[i]->GetExecNoeud()->AfficherNoeud();
                m_ExecChoix[i]->GetExecNoeud()->show();

                est_ce_que_l_interface_vers_suite_est_affichee = true;
            }
        }
    }
    /* pourrait servir à cause des noeuds else => je m'embrouille
     * if ( !est_ce_que_l_interface_vers_suite_est_affichee )
    {
        Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
    }*/
   return !est_ce_que_l_interface_vers_suite_est_affichee;
}

void ExecNoeud::GenerationExecChoix()
{
    // pas de choix en mode automatique :
    if ( Univers::ME->GetExecHistoire()->m_Histoire->m_ModeDeroulement != ModeDeroulement::Automatique) {
        if ( this->m_Noeud->m_Choix.length() > 0 &&
             this->m_ExecChoix.length() < this->m_Noeud->m_Choix.length() ) {
            for (Choix* choix: this->m_Noeud->m_Choix) {
                this->m_ExecChoix.push_back(new ExecChoix(this, choix, this));
            }
        }

    }
}

void ExecNoeud::RafraichirAffichageLayouts(int , int )
{
    Q_ASSERT_X(false,
               "ExecNoeud est pseudo virtualle : il faut redéfinir RafraichirAffichageLayouts dans les classes héritantes!",
               "ExecNoeud::RafraichirAffichageLayouts");
}

void ExecNoeud::AfficherNoeud()
{
    GenerationExecChoix();
}
