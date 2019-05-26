#include "invasionkraken.h"

InvasionKraken::InvasionKraken(QWidget *parent) : ExecHistoire(parent)
{
}

QString InvasionKraken::biomasse = "biomasse";
QString InvasionKraken::icharIV = "icharIV";
QString InvasionKraken::miral = "miral";
QString InvasionKraken::larnarno = "larnarno";
QString InvasionKraken::salem = "salem";
QString InvasionKraken::devlan_system = "devlan_system";
QString InvasionKraken::genestealers_exploration = "genestealers_exploration";
QString InvasionKraken::biomasse_exploration = "biomasse_exploration";
QString InvasionKraken::lictors_exploration = "lictors_exploration";
QString InvasionKraken::exploration_capture = "exploration_capture";
QString InvasionKraken::exploration_massacre = "exploration_massacre";
QString InvasionKraken::gaunts_exploration = "gaunts_exploration";

void InvasionKraken::GenererHistoire()
{
    m_Themes.append("warhammer 40000");
    m_Themes.append("malefique");
    m_Themes.append("cosmique");

    // Genestealer seul qui débarque
    GenererDebutExploration();
}

void InvasionKraken::GenererDebutExploration()
{
    Evt* Debut = AjouterEvt("DebutExploration", "Présentation de l'exploration");

    Debut->AjouterEffetChangementPerso("reine_norne",
          "Vous êtes l'esprit de la ruche. Le dominant d'une gigantesque flotte ruche de tyranides appelée à anéantir toutes les espèces vivantes de cette galaxie dans laquelle vous venez de pénétrer après avoir assimilé toute la vôtre.",
          "",
          "reine_norne");

    Debut->AjouterEffetNarration("Cette galaxie est riche en vie et nous allons la dévorer toute entière pour la plus grande puissance de la ruche.");

    Debut->AjouterEffetNarration(
                "Vous êtes la flotte ruche Kraken, la plus puissante des flottes ayant lancé l'invasion de la bordure orientale de la galaxie.",
                ":/Aventures/Warhammer 40000/Tyranides/Incursions_Tyranides.jpg");

    Debut->AjouterEffetGlisseur(
                "Il est temps de lancer l'exploration de cette galaxie pleine de proies qui viendront agrandir la taille de votre gigantesque flotte encore plus. Plus vous enverrez de vaisseaux en exploration plus vous aurez d'informations sur les planètes du secteur. Leur position, leur richesse en biosphère, et leur capacité défensive. Mais plus vous affaiblirez la puissance totale de votre flotte.",
                "0",
                "100000",
                "500",
                "biomasse_exploration",
                ":/Aventures/Warhammer 40000/Tyranides/DroneAvantGarde.jpg",
                "exploration_repartition");

    Debut->AjouterEffetRetireurACarac("biomasse", "biomasse_exploration", "", "effet_exploration1");
}
