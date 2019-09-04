#include "execnoeud.h"
#include "univers.h"
#include <QMovie>
#include "noeudnarratif.h"
#include "gestionnairecarac.h"

ExecNoeud::ExecNoeud(NoeudNarratif* noeud, QWidget* parent):QWidget (parent), m_Noeud(noeud)
{

}

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
            GestionnaireCarac::GetGestionnaireCarac()->AppliquerCarac(*noeudAExecuter->m_SetCaracs[i] );
        }
        perso->RafraichirAffichage();
    }

    // mise à jour de l'état de la partie :
    if ( noeudAExecuter->m_NouvelEtatPartie != QLatin1String("") )
        Univers::ME->ChangerEtatPartie(noeudAExecuter->m_NouvelEtatPartie);

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

    if (!transition_auto || Univers::ME->GetEtatPartie() == EP_FinPartie)
        Univers::ME->GetExecHistoire()->RafraichirAffichageEvtEtOuEffet( nullptr, nullptr );
}


bool ExecNoeud::GestionTransition()
{
    // commenté à cause des noeuds else (surement entre autres)
    //Q_ASSERT_X(false, "Noeud::GestionTransition", "Je ne crois pas que la gestion de transition devrait se faire dans Noeud si ?");
   Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
   return true;
}

void ExecNoeud::RafraichirAffichageLayouts(int , int )
{
    Q_ASSERT_X(false,
               "ExecNoeud est pseudo virtualle : il faut redéfinir RafraichirAffichageLayouts dans les classes héritantes!",
               "ExecNoeud::RafraichirAffichageLayouts");
}

void ExecNoeud::AfficherNoeud()
{
    // commenté à cause des noeuds else (surement entre autres)
    /*QString msg = "AfficherNoeud ne doit pas être appelé sur un Noeud mais seulement sur des objets en héritant " ;
    Q_ASSERT_X(false, "AfficherNoeud", msg.toStdString().c_str() );*/
}
