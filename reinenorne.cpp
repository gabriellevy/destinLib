#include "reinenorne.h"

ReineNorne::ReineNorne(QWidget *parent) : IPerso(parent)
{

}

void ReineNorne::GenererPersos()
{
    DPerso reine_norne;
    reine_norne.m_Id = "reine_norne";
    reine_norne.m_Nom = "Reine Norne";
    reine_norne.m_ImagePortrait.load(":/Aventures/Warhammer 40000/Tyranides/EspritDeLaRuche.jpg");
    reine_norne.m_CaracsAAfficher.append("biomasse");
    reine_norne.m_CaracsAAfficher.append("icharIV");
    reine_norne.m_CaracsAAfficher.append("miral");
    reine_norne.m_CaracsAAfficher.append("larnarno");
    reine_norne.m_CaracsAAfficher.append("salem");
    reine_norne.m_CaracsAAfficher.append("devlan_system");
    reine_norne.m_CaracsAAfficher.append("genestealers_exploration");
    reine_norne.m_CaracsAAfficher.append("biomasse_exploration");
    reine_norne.m_CaracsAAfficher.append("lictors_exploration");
    reine_norne.m_CaracsAAfficher.append("exploration_capture");
    reine_norne.m_CaracsAAfficher.append("exploration_massacre");
    reine_norne.m_CaracsAAfficher.append("gaunts_exploration");
    m_Persos.push_back(reine_norne);
}
