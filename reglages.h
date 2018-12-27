#ifndef REGLAGES_H
#define REGLAGES_H


class Reglages
{

public:
    Reglages();

    bool m_SonOn;

    int m_Hauteur;
    int m_Largeur;
    void ChangeTaille(int largeur, int hauteur);

    int m_LargeurColonneGauche;
};

#endif // REGLAGES_H
