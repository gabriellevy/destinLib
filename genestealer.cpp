#include "genestealer.h"
//#include "ui_genestealer.h"
#include <QPixmap>

Genestealer::Genestealer(QWidget *parent) :
    IPerso(parent)
{
    //ui_Genestealer->setupUi(this);
}


void Genestealer::GenererPersos()
{
    DPerso genestealer;
    genestealer.m_Id = "genestealer";
    genestealer.m_Nom = "Genestealer";
    genestealer.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/GenestealerPortrait.jpg");
    genestealer.m_CaracsAAfficher.append("nombre_insemines");
    genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen1");
    genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen2");
    genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen3");
    genestealer.m_CaracsAAfficher.append("pourcentage_culte_repéré");
    genestealer.m_CaracsAAfficher.append("nombre_genestealers");
    genestealer.m_CaracsAAfficher.append("population_humaine");
    genestealer.m_CaracsAAfficher.append("nid");
    genestealer.m_CaracsAAfficher.append("jours_sans_manger");
    genestealer.m_CaracsAAfficher.append("age_patriarche");
    m_Persos.push_back(genestealer);

    DPerso patriarche_genestealer;
    patriarche_genestealer.m_Id = "patriarche_genestealer";
    patriarche_genestealer.m_Nom = "Patriarche";
    patriarche_genestealer.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/Patriarche.jpg");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_insemines");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen1");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen2");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen3");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_hybrides_gen4");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_convertis");
    patriarche_genestealer.m_CaracsAAfficher.append("pourcentage_culte_repéré");
    patriarche_genestealer.m_CaracsAAfficher.append("nombre_genestealers");
    patriarche_genestealer.m_CaracsAAfficher.append("population_humaine");
    patriarche_genestealer.m_CaracsAAfficher.append("nid");
    patriarche_genestealer.m_CaracsAAfficher.append("age_patriarche");
    m_Persos.push_back(patriarche_genestealer);

    DPerso magus;
    magus.m_Id = "magus";
    magus.m_Nom = "Magus";
    magus.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/MagusPortrait.jpg");
    magus.m_CaracsAAfficher.append("nombre_insemines");
    magus.m_CaracsAAfficher.append("nombre_hybrides_gen1");
    magus.m_CaracsAAfficher.append("nombre_hybrides_gen2");
    magus.m_CaracsAAfficher.append("nombre_hybrides_gen3");
    magus.m_CaracsAAfficher.append("nombre_hybrides_gen4");
    magus.m_CaracsAAfficher.append("nombre_convertis");
    magus.m_CaracsAAfficher.append("pourcentage_culte_repéré");
    magus.m_CaracsAAfficher.append("nombre_genestealers");
    magus.m_CaracsAAfficher.append("population_humaine");
    magus.m_CaracsAAfficher.append("age_patriarche");
    m_Persos.push_back(magus);

    DPerso primus;
    primus.m_Id = "primus";
    primus.m_Nom = "Primus";
    primus.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/PrimusPortrait.jpg");
    primus.m_CaracsAAfficher.append("nombre_insemines");
    primus.m_CaracsAAfficher.append("nombre_hybrides_gen1");
    primus.m_CaracsAAfficher.append("nombre_hybrides_gen2");
    primus.m_CaracsAAfficher.append("nombre_hybrides_gen3");
    primus.m_CaracsAAfficher.append("nombre_hybrides_gen4");
    primus.m_CaracsAAfficher.append("nombre_convertis");
    primus.m_CaracsAAfficher.append("pourcentage_culte_repéré");
    primus.m_CaracsAAfficher.append("nombre_genestealers");
    primus.m_CaracsAAfficher.append("population_humaine");
    primus.m_CaracsAAfficher.append("age_patriarche");
    m_Persos.push_back(primus);

    DPerso reine_norne;
    reine_norne.m_Id = "reine_norne";
    reine_norne.m_Nom = "Reine norne";
    reine_norne.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/EspritDeLaRuche.jpg");
    reine_norne.m_CaracsAAfficher.append("biomasse");
    m_Persos.push_back(reine_norne);

}


