#include "tyranides.h"
//#include "ui_tyranides.h"
#include "genestealer.h"
#include "perso.h"
#include "ui_aventure.h"
#include "cultegenestealer.h"
#include "reinenorne.h"
#include "invasionkraken.h"

Tyranides::Tyranides(HistoireTyranide themeHistoire, QWidget *parent, QString premierEvt, QString premierEffet) :
    Aventure(parent),
    m_HistoireTyranide(themeHistoire)/*,
    ui_Tyranides(new Ui::Tyranides)*/
{
    //ui_Tyranides->setupUi(this);
    switch (themeHistoire) {
    case HistoireTyranide::eh_CulteGenestealer : {
        m_Perso = new Genestealer(ui->persoWidget);
        m_Histoire = new CulteGenestealer(ui->histoireWidget);

    }break;
    case HistoireTyranide::eh_FlotteKraken : {
        m_Perso = new ReineNorne(ui->persoWidget);
        m_Histoire = new InvasionKraken(ui->histoireWidget);

    }break;

    }

    // positionner l'interface
    ui->persoWidget->layout()->addWidget(m_Perso);
    //m_Perso->show();
    ui->histoireWidget->layout()->addWidget(m_Histoire);

    this->setWindowTitle("Tyranides");

    GenererAventure();

    if ( premierEvt != "" )
        m_Histoire->SetCurrentEvtId( premierEvt ) ;
    if ( premierEffet!= "" )
        m_Histoire->SetEffetIndex(m_Histoire->DeterminerIndexEffet(premierEffet) );

    m_EtatPartie = EP_Deroulement;
    m_TypeEvt = TE_Base;
    LancerEvtEtOuEffetCourant();
}

/*Tyranides::~Tyranides()
{
    delete ui_Tyranides;
}*/

void Tyranides::GenererAventure()
{
    GenererCaracs();

    (static_cast<Genestealer*>(m_Perso))->GenererPersos();

    m_Histoire->GenererHistoire();

    m_Perso->RafraichirAffichage();
}

void Tyranides::GenererCaracs()
{
    m_Perso->m_Caracs.append(new Carac(CulteGenestealer::patrouilles_humaines, "Patrouilles humaines", "",
                                       ":/Aventures/Warhammer 40000/Tyranides/PatrouilleHumaine.jpg",
                                       "", MODE_AFFICHAGE::ma_Img
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_insemines", "Humains inséminés", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteInséminé.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_hybrides_gen1", "Hybrides 1ère génération", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteHybride1.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_hybrides_gen2", "Hybrides 2ème génération", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteHybride2.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_hybrides_gen3", "Hybrides 3ème génération", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteHybride3.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_hybrides_gen4", "Hybrides 4ème génération", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteHybride4.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_convertis", "Cultistes", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteCultiste.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("pourcentage_culte_repéré", "Repérage du culte par les impériaux", "0",
                                       "",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nombre_genestealers", "Nombre de genestealers", "0",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteGenestealer.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("population_humaine", "Population Ichar IV", "500000000000",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteCitoyen.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("nid", "Nid", "",
                                       ":/Aventures/Warhammer 40000/Tyranides/icone_logements_détruits.jpg",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("jours_sans_manger", "Jours sans manger", "0",
                                       "",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("age_patriarche", "Âge du patriarche", "0", // tmp ? Initialiser à un gros nombe ?
                                       "",
                                       "", MODE_AFFICHAGE::ma_Nombre
                                       ));

    /**
      * caracs liées à la flotte Kraken
      */
    m_Perso->m_Caracs.append(new Carac("biomasse", "Biomasse", "1000000",
                                       "",
                                       "La somme totale de la biomasse de votre flotte, en milliards de tonnes.",
                                       MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("icharIV", "Ichar IV", "Pure",
                                       ":/Aventures/Warhammer 40000/Tyranides/ichar4.jpg",
                                       "État d'Ichar IV",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("miral", "Miral", "Pure",
                                       ":/Aventures/Warhammer 40000/Tyranides/Miral.jpg",
                                       "État de Miral",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("larnarno", "Larnarno", "Pure",
                                       ":/Aventures/Warhammer 40000/Tyranides/Larnarno.jpg",
                                       "État de Larnarno",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("salem", "Salem", "Pure",
                                       ":/Aventures/Warhammer 40000/Tyranides/Salem.jpg",
                                       "État de Salem",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("devlan_system", "Système Devlan", "Pure",
                                       ":/Aventures/Warhammer 40000/Tyranides/devlan_system.jpg",
                                       "Système Devlan",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("genestealers_exploration", "Exploration de genestealers", "",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteGenestealer.jpg",
                                       "",
                                       MODE_AFFICHAGE::ma_Img
                                       ));
    m_Perso->m_Caracs.append(new Carac("biomasse_exploration", "Biomasse d'exploration", "",
                                       "",
                                       "Biomasse dédiée à l'exploration de la galaxie.",
                                       MODE_AFFICHAGE::ma_Nombre
                                       ));
    m_Perso->m_Caracs.append(new Carac("lictors_exploration", "Exploration de lictors", "",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteLictor.jpg",
                                       "",
                                       MODE_AFFICHAGE::ma_Img
                                       ));
    m_Perso->m_Caracs.append(new Carac("gaunts_exploration", "Exploration de gaunts", "",
                                       ":/Aventures/Warhammer 40000/Tyranides/TeteHormagaunt.jpg",
                                       "",
                                       MODE_AFFICHAGE::ma_Img
                                       ));
    m_Perso->m_Caracs.append(new Carac("exploration_capture", "Capture pendant l'exploration", "",
                                       "",
                                       "",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
    m_Perso->m_Caracs.append(new Carac("exploration_massacre", "Massacre pendant l'exploration", "",
                                       "",
                                       "",
                                       MODE_AFFICHAGE::ma_Texte
                                       ));
}
