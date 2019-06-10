#ifndef EXECEFFET_H
#define EXECEFFET_H

#include "execnoeud.h"
#include "effet.h"

class ExecEvt;
class ExecChoix;

namespace Ui {
class Effet;
}

class ExecEffet : public ExecNoeud
{
    Q_OBJECT
public:
    ExecEffet(ExecEvt* exec_evt, Effet* effet, QWidget *parent = nullptr);
    virtual ~ExecEffet();

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);
    Glisseur* AjouterGlisseur();
    void GenerationExecChoix();
    bool GestionTransition();

    virtual void FinExecutionNoeud();
    void ChargerImage(QString chemin);
    void NettoyageAffichage();

    Effet* GetEffet();
    void AfficherNoeud();

    void AfficherBoutonSuivant();

    ExecEvt* m_ExecEvt = nullptr;

    QList<ExecChoix*> m_ExecChoix;

public slots:
    //void ExecuterNoeudSlot(bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true);
    void FinChrono();
    void valeurGlisseurAChange();

private:
    Ui::Effet *ui;
};

#endif // EXECEFFET_H
