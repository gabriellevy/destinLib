#include "cultegenestealer.h"
#include "perso.h"

CulteGenestealer::CulteGenestealer(QWidget *parent):Histoire(parent)
{
}

void CulteGenestealer::GenererInstallationGenestealer()
{
    Evt* Debut = AjouterEvt("Debut", "Arrivée d'un genestealer sur Ichar IV");
    Debut->AjouterEffetModificateurCarac("phase_infestation_genestealer", "arrivee_genestealer",
                                         "Vous êtes un genestealer. Une terrifiante créature Xenos assoifée de sang.");
    Effet* effetAtterrissage = Debut->AjouterEffetNarration("Le vaisseau où vous étiez en hibernation depuis un temps indéterminé, des siècles peut-être, vient enfin de s'écraser sur une planête inconnue. Votre organisme sort de l'hibernation, vos muscles s'étirent, il est temps de se mettre en chasse. La faim vous taraude déjà. Mais un autre instinct s'y mélange, presque aussi fort : celui de créer un nid, de s'y réfugier et de s'y reproduire. \nPour l'heure une seule pensée vous obséde : que ce soit pour l'un ou l'autre de ces buts il vous faut découvrir si cette planète est habitée par des êtres vivants, qui pourront satisfaire tous vos instincts primitifs.",
                              ":/Aventures/Warhammer 40000/Tyranides/Genestealers_atterissage.jpg",
                              "" );
    effetAtterrissage->AjouterChoixGoToEffet(
                "Chercher une proie aux alentours", "chasse_genestealer");
    effetAtterrissage->AjouterChoixGoToEffet(
                "Essayer de s'approcher de la montagne de métal visible à l'horizon", "approche_ruche_lamos");
    effetAtterrissage->AjouterChoixGoToEffet(
                "Chercher dans l'épave si il y d'autres genestealers", "fouille_epave_genestealers");

    Effet* fouille_epave_genestealers = Debut->AjouterEffetNarration("Vous sentez confusément via le lien télépathique que partagent les genestealers qu'il y a plusieurs de vos semblables qui se trouvaient aussi en hibernation dans le vaisseau et qui n'ont pas été tués à l'impact. Après moins d'une heure vous parvenez à en trouver et dégager quatre. Certais sont blessés mais la pgrande résistance de votre race leur permettra sans nul doute de récupérer rapidement. Pas de remerciement entre genestalers. Même si vous avez votre individualité jusqu'à un certain point, l'esprit de la ruche tyranide est en tous ses enfants et vous pousse à un travail en commun sans hésitation. \n Il n'empêche, pour maximiser vos chances de propager votre espèce, il vaut mieux se disperser et chercher chacun de votre côté ce que ce monde a à vous offrir.",
                              ":/Aventures/Warhammer 40000/Tyranides/Genestealers_atterissage.jpg",
                              "fouille_epave_genestealers" );
    fouille_epave_genestealers->AjouterChangeurDeCarac("nombre_genestealers", "4");
    fouille_epave_genestealers->AjouterChangeurDeCarac("pourcentage_culte_repéré", "6");
    fouille_epave_genestealers->AjouterChoixGoToEffet(
                "Chercher une proie aux alentours", "chasse_genestealer");
    fouille_epave_genestealers->AjouterChoixGoToEffet(
                "Essayer de s'approcher de la montagne de métal visible à l'horizon", "approche_ruche_lamos");
    fouille_epave_genestealers->AjouterCallback(&CulteGenestealer::TempPasse, "1");

    Effet* chasse_genestealer = Debut->AjouterEffetNarration("Cette planète ne semble pas avoir une faune très développée. Et en tout cas les paisibles herbivores que vous y avez découvert n'ont pas l'habitude de croiser un prédateur alpha tel que vous. En fait ils ont été si lents à se mettre à vous fuir qu'ils semblent ne jamais avoir à fuir de chasseur carnivore. Cela va changer. Bien sûr à voir vos pinces monstrueuses ils ont tout de même fini par déguerpir mais trop tard. Une petite pointe de vitesse et vous en attrapiez un pour couper son long coup élégant d'un seul coup de griffe.\n Un bon repas plus tard, les sens vous reviennent : la nourriture sera facile à trouver sur ce monde, c'est une bonne chose. Mais le destin d'un genestealer solitaire est plus grand : vous avez besoin de créature intelligente à parasiter pour vous reproduire et revendiquer ce monde. Or ces herbivores délicieux que vous avez tués ne sont assurément pas intelligents, les parasiter est sans intérêt. La gigantesque structure métallique que vous voyez à l'horizon est prometteuse par contre.",
                              ":/Aventures/Warhammer 40000/Tyranides/genestealer_attaque.jpg",
                              "chasse_genestealer" );
    chasse_genestealer->AjouterChangeurDeCarac("jours_sans_manger", "0");
    chasse_genestealer->m_GoToEffetId = "approche_ruche_lamos";

    Debut->AjouterEffetNarration("Pas de doute possible. Cette structure gigantesque de métal a forcément été bâtie par une créature intelligente. Elle semble habitée et en plus elle vous rappelle vaguement le vaisseau dans lequel vous avez atteint la planète. Un vaisseau humain. Ce serait vraiment l'idéal. Les humais sont non seulement facile à attraper mais aussi délicieux et idéaux pour porter des enfants genestealers.",
                                  ":/Aventures/Warhammer 40000/Tyranides/ruche_lamos.jpg",
                                  "approche_ruche_lamos" );

    Effet* approche_ruche_lamos2 = Debut->AjouterEffetNarration("En approchant l'édifice vous appercevez une créature bipède à peu près de votre taille affairée sur un des énormes tuyaux de métal qui se dirigent vers la ruche. Il s'agit bel et bien d'un humain. L'atteindre et le tuer serait à coup sûr facile. Le contaminer avec votre adn le serait presque autant. Mais votre instinct alien vous recommande la prudence. Le but ultime tout genestealer est d'infester la planète entière. Si vous vous faites repérer aussi vite vous risquez de tout comprommettre.",
                                  "",
                                  "approche_ruche_lamos2" );
    approche_ruche_lamos2->AjouterChoixGoToEffet(
                "attaquer tout de même, il n'y de toute façon pas d'autres humains à des centaines de mètres à la ronde.",
                "chasse_humain_chemin_lamos");
    approche_ruche_lamos2->AjouterChoixGoToEffet(
                "L'éviter et tenter d'atteindre la ruche",
                "arrivee_lamos");
    approche_ruche_lamos2->AjouterCallback(&CulteGenestealer::TempPasse, "2");
    Effet* chasse_humain_chemin_lamos = Debut->AjouterEffetNarration("C'est en effet un jeu d'enfant de l'approcher tant il est occupé à faire dieu sait quoi. Sa vie est entre vos mains",
                                  "",
                                  "chasse_humain_chemin_lamos" );
    chasse_humain_chemin_lamos->AjouterChoixGoToEffet(
                "Le tuer et le dévorer",
                "meurtre_chemin_lamos");
    chasse_humain_chemin_lamos->AjouterChoixGoToEffet(
                "L'inséminer avec votre ADN",
                "insemination_chemin_lamos");
    Effet* meurtre_chemin_lamos = Debut->AjouterEffetNarration("Saisissant le chétif humain par les bras vous tirez brutalement tout en frappant de vos pinces tranchantes. La créature est littéralement coupée en quatre morceaux tant même sa colonne vertébrale est fragile. Ce sera un jeu d'anfant de s'installer ici et de chasser régulièrement. C'est une très bonne chose. Il va cependant falloir être prudent. Nul doute que la structure de métal contient une quantité énorme de ces êtres qui ont sans doute la capacité à devenir dangereux en grand nombre. En attendant il n'y en a aps un seul à l'horizon. C'est l'heure du festin. Une fois celui ci terminé vous enterrez les restes discrètement à quelque distance. Peut-être d'autres humains le chercheront-t'ils mais il auront bien du mal à retrouver ses restes.",
                                  "",
                                  "meurtre_chemin_lamos" );
    meurtre_chemin_lamos->AjouterChangeurDeCarac("jours_sans_manger", "0");
    meurtre_chemin_lamos->AjouterAjouteurACarac("pourcentage_culte_repéré", "2");
    meurtre_chemin_lamos->AjouterRetireurACarac("population_humaine", "1");
    meurtre_chemin_lamos->m_GoToEffetId = "arrivee_lamos";

    Effet* insemination_chemin_lamos = Debut->AjouterEffetNarration("Vous saisissez la pitoyable créature avec vos mains en prenant garde de ne pas arracher ses fragiles membres. Puis vous la mordez légèrement au bras, permettant ainsi à votre Ovipositor, via votre langue, de déposer votre ADN dans le corps de l'humain chétif. Le choc suffit à lui faire perdre connaissance. À son réveil il ne se rappellera de rien et sa blessure sera presque résorbée. Mais son corps aura déjà commencé à changer en profondeur. Bientôt il souhaitera ardemment se reproduire et son enfant sera un hybride genestealer.",
                                  "",
                                  "insemination_chemin_lamos" );
    insemination_chemin_lamos->AjouterAjouteurACarac("nombre_insemines", "1");
    insemination_chemin_lamos->AjouterAjouteurACarac("pourcentage_culte_repéré", "3");
    insemination_chemin_lamos->m_GoToEffetId = "arrivee_lamos";

    Effet* arrivee_lamos = Debut->AjouterEffetNarration("Vous atteignez enfin les abords de la structure et tous vos soupçons se confirment. C'est définitivement une construction humaine très ancienne. Ses proportions sont colossales. Peut-être 10 kilomètres de hauteur. Et sa population est innombrable. Il va être difficile de pénétrer à l'intérieur sans être remarqué. Pas qu'elle soit réellement gardée mais les humains finiront tôt ou tard par vous appercevoir et même si vous passiez en force ils lanceraient une chasse dans tous les environs pour vous retrouver.",
                                  "",
                                  "arrivee_lamos" );
    arrivee_lamos->AjouterChoixGoToEffet(
                "Peu importe la discrétion, je passe en force",
                "pas_fait");
    arrivee_lamos->AjouterChoixGoToEffet(
                "Passer prudemment et discrètement",
                "pas_fait");
    arrivee_lamos->AjouterChoixGoToEffet(
                "Attendre la nuit en restant à bonne distance",
                "entree_lamos_nuit");

    Effet* entree_lamos_nuit = Debut->AjouterEffetNarration("À la nuit tombée la plupart des humains rentrent chez eux se coucher. Ceux qui restent voient manifestement très mal malgré les lumières qu'ils allument, tands que vous voyez comme en plein jour. Vous pénétrez facilement dans la ruche.",
                                  "",
                                  "entree_lamos_nuit" );
    entree_lamos_nuit->m_GoToEffetId = "arrivée_intérieur_lamos";
    entree_lamos_nuit->AjouterCallback(&CulteGenestealer::TempPasse, "1");

    Effet* arrivee_interieur_lamos = Debut->AjouterEffetNarration("Vous êtes parvenu à vous introduire dans la ruche. Qu'allez vous faire ?",
                                  "",
                                  "arrivée_intérieur_lamos" );
    arrivee_interieur_lamos->AjouterChoixGoToEffet(
                "Chercher un repaire sûr pour créer votre nid",
                "recherche_nid");
    arrivee_interieur_lamos->AjouterChoixGoToEffet(
                "Commencer à vous introduire chez des humains pour les inséminer",
                "pas_fait");
    arrivee_interieur_lamos->AjouterChoixGoToEffet(
                "Tenter de tuer le maximum d'humains pendant leur sommeil.",
                "pas_fait");

    Effet* recherche_nid = Debut->AjouterEffetNarration("Après quelques heures à vous éloigner des zones trop fréquentées vous découvrez un vieil entrepôt de bonne taille manifestement abandonné depuis des années.",
                                  ":/Aventures/Warhammer 40000/Tyranides/entrepot.jpg",
                                  "recherche_nid" );
    recherche_nid->AjouterChoixGoToEffet(
                "S'installer ici",
                "pas_fait");
    recherche_nid->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "recherche_nid_vieux_logements");
    recherche_nid->AjouterChoixGoToEffet(
                "Attendre une journée complète de voir si le lieu est sûr.",
                "recherche_nid_entrepot");

    Effet* recherche_nid_entrepot = Debut->AjouterEffetNarration("En une journée complète non seulement personne n'est entré dans l'entrepôt mais personne ne s'en est même approché à moins de 300 mètres. De plus en dessous de lui se trouve un système d'égoût assez étendu et tout aussi désert. Le seul défaut est le manque de chaleur de l'endroit pour élever des petits.",
                                  ":/Aventures/Warhammer 40000/Tyranides/entrepot.jpg",
                                  "recherche_nid_entrepot" );
    recherche_nid_entrepot->AjouterCallback(&CulteGenestealer::TempPasse, "1");
    recherche_nid_entrepot->AjouterChoixGoToEffet(
                "S'installer ici",
                "pas_fait");
    recherche_nid_entrepot->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "recherche_nid_vieux_logements");
    recherche_nid_entrepot->AjouterChoixGoToEffet(
                "Attendre six mois de plus.",
                "recherche_nid_entrepot2");

    Effet* recherche_nid_entrepot2 = Debut->AjouterEffetNarration("Malheureusement au bout de deux mois où vous avez attendu avec la patience sans fond du tyranide que vous êtes, des humains sont venus plusieurs fois. Vous ignorez ce qu'ils cherchent mais c'est une raison suffisante pour fuir cet endroit et en trouver un autre. De plus vous êtes maintenant à nouveau affamé.",
                                  ":/Aventures/Warhammer 40000/Tyranides/entrepot.jpg",
                                  "recherche_nid_entrepot2" );
    recherche_nid_entrepot2->AjouterCallback(&CulteGenestealer::TempPasse, "61");
    recherche_nid_entrepot2->AjouterChoixGoToEffet(
                "Les tuer et les manger",
                "pas_fait");
    recherche_nid_entrepot2->AjouterChoixGoToEffet(
                "Les tuer et s'installer ici",
                "pas_fait");
    recherche_nid_entrepot2->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "recherche_nid_vieux_logements");

    Effet* recherche_nid_vieux_logements = Debut->AjouterEffetNarration("Après plusieurs jours passés à chercher un terrain plus favorables en pénétrant dans les profondeurs de la ruche vous finissez par atteindre un secteur qui semble avoir éé un système de logement. Il est dans un état déplorable. Sans doute à l'abandon depuis un siècle, ce qui est très bon signe. Les humains sont très peu nombreux dans les environs et évitent sans doute ces ruines dangereuses.",
                                  ":/Aventures/Warhammer 40000/Tyranides/logements_détruits.jpg",
                                  "recherche_nid_vieux_logements" );
    recherche_nid_vieux_logements->AjouterCallback(&CulteGenestealer::TempPasse, "4");
    recherche_nid_vieux_logements->AjouterChoixGoToEffet(
                "S'installer ici",
                "pas_fait");
    recherche_nid_vieux_logements->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "pas_fait");
    recherche_nid_vieux_logements->AjouterChoixGoToEffet(
                "Attendre un mois pour voir si des humains passent par ici.",
                "essaye_attente_nid_vieux_logements");

    Effet* essaye_attente_nid_vieux_logements = Debut->AjouterEffetTest("jours_sans_manger", Comparateur::c_Inferieur, "30", "essaye_attente_nid_vieux_logements" );
    essaye_attente_nid_vieux_logements->m_GoToEffetId = "attente_nid_vieux_logements";
    Noeud* else1 = essaye_attente_nid_vieux_logements->AjouterElse("Impossible d'attendre plus longtemps. Malgré votre organisme en quasi hibernation la faim vous taraude trop. Vous devez entrer en chasse.");
    else1->m_GoToEffetId = "chasse_ruche";

    Effet* chasse_ruche = Debut->AjouterEffetNarration("Heureusement il n'est pas trop difficile dans ces tunnels de trouver une proie facile et de cacher ses restes. Un habitant de la ruche ne rentrera pas chez lui ce soir. Une fois rassasié vous retournez aux logements désaffectés.",
                  ":/Aventures/Warhammer 40000/Tyranides/genestealer_attaque.jpg",
                  "chasse_ruche" );
    chasse_ruche->AjouterChangeurDeCarac("jours_sans_manger", "0");
    chasse_ruche->AjouterAjouteurACarac("pourcentage_culte_repéré", "1");
    chasse_ruche->AjouterRetireurACarac("population_humaine", "1");
    chasse_ruche->m_GoToEffetId = "attente_nid_vieux_logements";

    Effet* attente_nid_vieux_logements = Debut->AjouterEffetNarration("Toujours rien un mois plus tard. Pour un humain cette attente aurait déjà parue interminable. Et pour vous ?",
                  ":/Aventures/Warhammer 40000/Tyranides/logements_détruits.jpg",
                  "attente_nid_vieux_logements" );
    attente_nid_vieux_logements->AjouterCallback(&CulteGenestealer::TempPasse, "29");
    attente_nid_vieux_logements->AjouterChoixGoToEffet(
                "S'installer ici",
                "pas_fait");
    attente_nid_vieux_logements->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "pas_fait");
    attente_nid_vieux_logements->AjouterChoixGoToEffet(
                "Attendre deux ans de plus.",
                "attente_nid_vieux_logements_deux_ans");

    Effet* attente_nid_vieux_logements_deux_ans = Debut->AjouterEffetNarration("Deux ans plus tard vous êtes très satisfait : vous avez trouvé un nid relativement chaud et surtout où aucun humain n'a mis le pied. Mis à part bien sûr les quelques victmes que vous avez enlevées et enterrées dans les environs pour satisfaire votre faim. ",
                  ":/Aventures/Warhammer 40000/Tyranides/logements_détruits.jpg",
                  "attente_nid_vieux_logements_deux_ans" );
    attente_nid_vieux_logements_deux_ans->AjouterCallback(&CulteGenestealer::TempPasse, "730");
    attente_nid_vieux_logements_deux_ans->AjouterAjouteurACarac("pourcentage_culte_repéré", "3");
    attente_nid_vieux_logements_deux_ans->AjouterRetireurACarac("population_humaine", "6");
    attente_nid_vieux_logements_deux_ans->AjouterChoixGoToEffet(
                "S'installer ici",
                "installation_vieux_logements");
    attente_nid_vieux_logements_deux_ans->AjouterChoixGoToEffet(
                "Chercher un autre endroit",
                "pas_fait");

    Effet* installation_vieux_logements = Debut->AjouterEffetNarration("Vous avez choisi votre repaire et en connaissez les environs par coeur. Vous êtes maintenant chez vous. L'établissement du culte peut commencer. ",
                  ":/Aventures/Warhammer 40000/Tyranides/logements_détruits.jpg",
                  "installation_vieux_logements" );
    installation_vieux_logements->AjouterChangeurDeCarac("nid", "vieux_logements");
    installation_vieux_logements->m_GoToEvtId = "Debut_Reproduction_Genestealer";

}

void CulteGenestealer::TempPasse(QString nbJoursStr)
 {
     int nbJoursInt = nbJoursStr.toInt();
    SetCarac ajouterJoursSansManger(ModifCaracType::AddToCarac, CulteGenestealer::joursSansManger, nbJoursStr);
    IPerso::GetPersoInterface()->AppliquerCarac(ajouterJoursSansManger);

    SetCarac ajouterJoursAAge(ModifCaracType::AddToCarac, CulteGenestealer::age_patriarche, nbJoursStr);
    IPerso::GetPersoInterface()->AppliquerCarac(ajouterJoursAAge);

    int ageColonie = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::age_patriarche).toInt();

    /**
     * variations semi aléatoires de population chaque jour
     * */
    double totalPopulation = IPerso::GetPersoInterface()->GetCaracValue("population_humaine").toDouble();
    for ( int i = 0; i < nbJoursInt ; i++)
    {
        totalPopulation += ( totalPopulation *(static_cast<double>((qrand()%100-50))/100000.0));
    }
    //int popInt = std::round(totalPopulation);
    SetCarac changePop(ModifCaracType::SetCarac,
                       "population_humaine",
                       QString::number(std::round(totalPopulation)));
    IPerso::GetPersoInterface()->AppliquerCarac(changePop);

    QString autorisation_insemines = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::autorisation_insemines );
    /**
      * Reproduction des différentes générations d'hybrides
      */
    if ( ageColonie > 30 )
    {
        double nb_insemine = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::nombre_insemines).toDouble();
        double nb_nouv_gen1 = nb_insemine/60;
        if ( autorisation_insemines == "enfermes_au_nid") nb_nouv_gen1 /= 2;
        if ( autorisation_insemines == "interdit_d_entrer") nb_nouv_gen1 *= 2;
        if (nb_nouv_gen1 > 1)
        {
            SetCarac nombre_hybrides_gen1(ModifCaracType::AddToCarac, CulteGenestealer::nombre_hybrides_gen1, QString::number(std::round(nb_nouv_gen1)));
            IPerso::GetPersoInterface()->AppliquerCarac(nombre_hybrides_gen1);
        }

        double nb_gen1 = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::nombre_hybrides_gen1).toDouble();
        double nb_nouv_gen2 = nb_gen1/90;
        if (nb_nouv_gen2 > 1)
        {
            SetCarac nombre_hybrides_gen2(ModifCaracType::AddToCarac, CulteGenestealer::nombre_hybrides_gen2, QString::number(std::round(nb_nouv_gen2)));
            IPerso::GetPersoInterface()->AppliquerCarac(nombre_hybrides_gen2);
        }

        double nb_gen2 = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::nombre_hybrides_gen2).toDouble();
        double nb_nouv_gen3 = nb_gen2/120;
        if (nb_nouv_gen3 > 1)
        {
            SetCarac nombre_hybrides_gen3(ModifCaracType::AddToCarac, CulteGenestealer::nombre_hybrides_gen3, QString::number(std::round(nb_nouv_gen3)));
            IPerso::GetPersoInterface()->AppliquerCarac(nombre_hybrides_gen3);
        }

        /**
          * NOTE : ajouter du random dans la reproduction ?
          *
          * A FAIRE : reproduction finale : genestealer et quasi humains
          * */

    }

    /**
      * patrouilles humaines et leurs attaques contre les inséminés en liberté
      */
    if ( IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::patrouilles_humaines) != "")
    {
        if ( autorisation_insemines != "enfermes_au_nid")
        {
            double totalInsemine = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::nombre_insemines).toDouble();
            for ( int i = 0; i < nbJoursInt ; i++)
            {
                double pourcentage_perdu = (static_cast<double>(qrand()%100 - 50))/100;

                if ( pourcentage_perdu < 0 )
                    pourcentage_perdu = 0;

                totalInsemine -= 1;
                totalInsemine -= ( totalInsemine *pourcentage_perdu);

                if ( autorisation_insemines == "interdit_d_entrer")
                {
                    totalInsemine -= 1;
                    totalInsemine -= ( totalInsemine *pourcentage_perdu);
                }
            }
            if ( totalInsemine < 0 )
                totalInsemine = 0;
            SetCarac changePop(ModifCaracType::SetCarac,
                               CulteGenestealer::nombre_insemines,
                               QString::number(std::round(totalInsemine)));
            IPerso::GetPersoInterface()->AppliquerCarac(changePop);

            // pour l'instant on dira que els patrouilles durent une semaine et puis voilà ... :
            SetCarac finPatrouilles(ModifCaracType::SetCarac,
                               CulteGenestealer::patrouilles_humaines,
                               "");
            IPerso::GetPersoInterface()->AppliquerCarac(finPatrouilles);
        }
    }

    // A FAIRE :et la mise à jour d'affichage de la jauge + couleur ? + la garder à 100 maximum

    // effet des genestealers en "liberté" ?

 }

void CulteGenestealer::GenererHistoire()
{
    m_Themes.append("warhammer 40000");
    m_Themes.append("malefique");
    m_Themes.append("cosmique");

    // Genestealer seul qui débarque
    GenererInstallationGenestealer();
    GenererDebutReproductionGenestealer();
    GenererPhasePatriarche();
    GenererPhasePolitique();
    GenererPhaseRebellion();
}

void CulteGenestealer::GenererPhasePolitique()
{
    Evt* evt = AjouterEvt("Debut_Politique_Genestealer", "Infiltration politique du culte");

    Effet* magus = evt->AjouterEffetChangementPerso("magus",
           "C'est parti pour la phase politique."
           );
    magus->AjouterChangeurDeCarac("phase_infestation_genestealer", "phase_politique_genestealer");

    Effet* fin_magus = evt->AjouterEffetNarration("Fin de la phase politique.");
    fin_magus->m_GoToEvtId = "Rébellion_Genestealer";

}

void CulteGenestealer::GenererPhaseRebellion()
{
    Evt* evt = AjouterEvt("Rébellion_Genestealer", "Rébellion");

    Effet* primus = evt->AjouterEffetChangementPerso("primus",
           "C'est parti pour la rébellion."
           );

}

void CulteGenestealer::GenererPhasePatriarche()
{
    Evt* evt = AjouterEvt("Debut_Patriarche", "Ascension du patriarche");

    Effet* patriarche_genestealer = evt->AjouterEffetChangementPerso("patriarche_genestealer",
           "Vous avez longuement et patiemment tissé votre toile. Votre culte se répand dans la ruche Lamos, bientôt sur toute la planète. Vos sens se développent, vous percevez le flux du warp. Vous changez pour pouvoir vous occuper de votre confrérie nouvelle, de votre grande famille. Vous êtes différent."
           );
    patriarche_genestealer->AjouterChangeurDeCarac("phase_infestation_genestealer", "domination_patriarche");

    evt->AjouterEffetNarration("Vous êtes maintenant un patriarche, un énorme genestaler doué de pouvoirs psychique redoutables. Un maître de la manipulation. Votre contrôle sur les hybrides auxquels vous avez donné naissance est total. Ils mourront plutôt que de trahir votre présence dans les profondeurs de la ruche. \n Vous devez continuer à vous reproduire discrètement, à multiplier vos adeptes. Bientôt apparaîtront de nouvelles générations d'hybrides qui pourront se mêler sans risques aux humains et la vraie prise de contrôle commencera.");

    Effet* fin_patriarche = evt->AjouterEffetNarration("Fin de la phase patriarche.");
    fin_patriarche->m_GoToEvtId = "Debut_Politique_Genestealer";

}

void CulteGenestealer::GenererDebutReproductionGenestealer()
{
    Evt* evt = AjouterEvt("Debut_Reproduction_Genestealer", "Début de la reproduction");

    Effet* choix_sur_insemines = evt->AjouterEffetModificateurCarac("phase_infestation_genestealer",
                                       "nid_genestealer",
                                       "Vous êtes enfin installé et en sécurité relative. Votre instinct vous commande maintenant avec insistance de vous multiplier pour assurer la suprémacie de votre race sur cette planète. L'insémination des humains est facile si vous êtes un peu prudent, c'est la naissance et l'élevage qui sont dangreux. Une fois inséminé les humains sont soumis au pouvoir psychique de la ruche tyranide et s'occuperont fanatiquement de leurs enfants même si ceux ci sont à moitié extra-terrestres et pouvus de trois bras. Le danger est si vous ou leurs enfants sont vus par des humains. En conséquence qu'allez vous faire des humains que vous avez inséminés ?",
                                       "choix_sur_insemines"
                                       );
    choix_sur_insemines->AjouterChoixChangeurDeCarac(
                "Vous les kidnapperez et garderez enfermés au nid. Vous devrez donc les nourrir vous-mêmes",
                CulteGenestealer::autorisation_insemines, "enfermes_au_nid");
    choix_sur_insemines->AjouterChoixChangeurDeCarac(
                "Ils peuvent aller et venir",
                CulteGenestealer::autorisation_insemines, "vont_et_viennent");
    choix_sur_insemines->AjouterChoixChangeurDeCarac(
                " Ils ont interdiction d'approcher le nid. Seuls leurs enfants hybrides genestealers pourront rejoindre le nid.",
                CulteGenestealer::autorisation_insemines, "interdit_d_entrer");

    /**
     * Boucle principale de reproduction et chasse du genestealer solitaire
     */
    Effet* debut_phase_insemination = evt->AjouterEffetNarration("Dans les prochaines nuits allez-vous : ", "", "debut_phase_insémination");
    debut_phase_insemination->AjouterChoixGoToEffet(
                "Inséminer le maximum d'humains", "insémination1");
    debut_phase_insemination->AjouterChoixGoToEffet(
                "Inséminer des humains prudemment et seulement très loin de votre nid", "insémination4");
    // plus le chiffre après "insémination" est bas plus le risque est gros et plus le potentiel d'inséminés est grand
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.3f, "2", "insémination1"));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.3f, "2", "insémination2"));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.3f, "2", "insémination3+repéré", true));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.6f, "1", "insémination4"));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.8f, "1", "insémination5"));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(0.3f, "2", "insémination6+repéré", true));
    evt->m_Effets.push_back(GenererEffetTentativeInsemination(1.0f, "1", "insémination7"));

    Effet* temp_passe = evt->AjouterEffetNarration("Une semaine est passée.", "", "temp_passe");
    temp_passe->AjouterCallback(&CulteGenestealer::TempPasse, "7");

    // ------------- CHASSE
    {
        Effet* test_faim = evt->AjouterEffetTest("jours_sans_manger", Comparateur::c_Superieur, "15", "test_faim" );
        test_faim->m_Text = "Toute cette activité vous affame. Vous devez vous nourrir.";
        test_faim->AjouterChoixGoToEffet(
                    "Un humain des environs attaqué de nuit sera parfait", "chasse_proche");
        test_faim->AjouterChoixGoToEffet(
                    "Mieux vaut attaquer un humai loin du nid même si ce sera bien plus long", "chasse_loin");
        test_faim->AjouterChoixGoToEffet(
                    "Se nourrir d'animaux est plus prudent. Ils sont très rares et cela risque de prendre du temps", "chasse_animaux");
        Noeud* else1 = test_faim->AjouterElse();
        else1->m_GoToEffetId = "test_intervention_imperium";

        // NOTE : ajouter un danger à la chasse si on est à un niveau de repéré élevé ??
        Effet* chasse_proche = evt->AjouterEffetModificateurCarac(CulteGenestealer::joursSansManger, "0", "Vous êtes rassasié", "chasse_proche");
        chasse_proche->AjouterAjouteurACarac(CulteGenestealer::repererImperium, "2");
        chasse_proche->m_GoToEffetId = "test_intervention_imperium";
        Effet* chasse_loin = evt->AjouterEffetModificateurCarac(CulteGenestealer::joursSansManger, "0", "Vous êtes rassasié", "chasse_loin");
        chasse_loin->AjouterAjouteurACarac(CulteGenestealer::repererImperium, "1");
        chasse_loin->AjouterCallback(&CulteGenestealer::TempPasse, "1");
        chasse_loin->m_GoToEffetId = "test_intervention_imperium";
        Effet* chasse_animaux = evt->AjouterEffetModificateurCarac(CulteGenestealer::joursSansManger, "0", "Vous êtes rassasié", "chasse_animaux");
        chasse_animaux->AjouterCallback(&CulteGenestealer::TempPasse, "3");
        chasse_animaux->m_GoToEffetId = "test_intervention_imperium";
    }

    //-------------------INTERVENTION IMPERIUM
    {
        Effet* test_intervention_imperium = evt->AjouterEffetTest(
                    CulteGenestealer::repererImperium,
                    Comparateur::c_Inferieur,
                    "40",
                    "test_intervention_imperium");
        test_intervention_imperium->m_GoToEffetId = "test_fin_cycle_insemination";

        // risque d'intervention des humains :
        Effet* test_patrouilles = evt->AjouterEffetModificateurCarac(
                    CulteGenestealer::patrouilles_humaines,
                    "1",
                    "Il y a trop de rumeurs de monstres et de disparitions mystérieuses. Les humains se décident finalement à se méfier et organisent des patrouilles de nuit comme de jour. Il va être plus difficile de chasser comme d'inséminer vos proies.",
                    "test_patrouilles");
        test_patrouilles->AjouterCallbackDeTest(&CulteGenestealer::TestReactionImperium, "40");

        Effet* test_decouverte_nid = evt->AjouterEffetNarration(
                    "Catastrophe ! Les humains ont découvert votre nid et l'attaquent alors que vous êtes très loin d'être prêt.",
                    "",
                    "test_decouverte_nid");
        test_decouverte_nid->m_GoToEffetId = "pas_fait";
        test_decouverte_nid->AjouterCallbackDeTest(&CulteGenestealer::TestReactionImperium, "95");

    }

    Effet* test_fin_cycle_insemination = evt->AjouterEffetTest(
                CulteGenestealer::nombre_insemines, Comparateur::c_Inferieur, "1", "test_fin_cycle_insemination");
    /*Effet* test_fin_cycle_insemination = evt->AjouterEffetTest(
                "nombre_hybrides_gen3", Comparateur::c_Inferieur, "1", "test_fin_cycle_insemination");*/
    test_fin_cycle_insemination->m_GoToEffetId = "debut_phase_insémination";

    Effet* effet_vide2 = evt->AjouterEffetVide();
    effet_vide2->m_GoToEvtId = "Debut_Patriarche";
}

bool CulteGenestealer::TestReactionImperium(QString modificateur_negatif)
{
    float repereImperium = IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::repererImperium).toFloat();
    float proba = (repereImperium - modificateur_negatif.toFloat());

    if (IPerso::GetPersoInterface()->GetCaracValue(CulteGenestealer::autorisation_insemines) == "vont_et_viennent")
    {
        proba += 30;// les va et vient augmentent le risque d'être repéré
    }

    return (qrand()%100) < proba;
}

QString CulteGenestealer::repererImperium = "pourcentage_culte_repéré";
QString CulteGenestealer::joursSansManger = "jours_sans_manger";
QString CulteGenestealer::age_patriarche = "age_patriarche";
QString CulteGenestealer::nombre_insemines = "nombre_insemines";
QString CulteGenestealer::nombre_hybrides_gen1 = "nombre_hybrides_gen1";
QString CulteGenestealer::nombre_hybrides_gen2 = "nombre_hybrides_gen2";
QString CulteGenestealer::nombre_hybrides_gen3 = "nombre_hybrides_gen3";
QString CulteGenestealer::autorisation_insemines = "autorisation_insemines";
QString CulteGenestealer::patrouilles_humaines = "patrouilles_humaines";

Effet* CulteGenestealer::GenererEffetTentativeInsemination(float proba, QString nombreInsemines, QString id, bool repereImperium)
{
    Effet* effet = new Effet();
    effet->m_Id = id;
    effet->AjouterAjouteurACarac("nombre_insemines", nombreInsemines);
    if ( repereImperium)
        effet->AjouterAjouteurACarac("pourcentage_culte_repéré", "4");
    effet->m_Conditions.push_back(new Condition(proba));
    return effet;
}
