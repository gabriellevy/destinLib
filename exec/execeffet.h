#ifndef EXECEFFET_H
#define EXECEFFET_H

#include "execnoeud.h"
#include "../abs/effet.h"



class ExecEvt;
class ExecChoix;
class ExecLancerDe;

namespace Ui {
class Effet;
}

class ExecEffet : public ExecNoeud,
        my_enable_shared_from_this<ExecEffet>
{
    Q_OBJECT
public:
    using my_enable_shared_from_this<ExecEffet>::shared_from_this;
    ExecEffet(std::shared_ptr<Effet> effet, QWidget *parent = nullptr);
    virtual ~ExecEffet();

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);
    Glisseur* AjouterGlisseur();
    bool GestionTransition();

    virtual void FinExecutionNoeud();
    void ChargerImage(QString chemin);
    void NettoyageAffichage();
    std::shared_ptr<ExecLancerDe> SetExecLancerDe(std::shared_ptr<ExecLancerDe> exec_lancer_de);
    std::shared_ptr<ExecLancerDe> SetExecLancerDe(std::shared_ptr<LancerDe> lancer_de);

    std::shared_ptr<Effet> GetEffet();
    void AfficherNoeud();

    void AfficherBoutonSuivant();

    std::shared_ptr<ExecLancerDe> m_ExecLancerDe = nullptr;

public slots:
    //void ExecuterNoeudSlot(bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true);
    void FinChrono();
    void valeurGlisseurAChange();

protected:
    virtual void AjouterAuxBoutonsHoriz(std::shared_ptr<ExecNoeud> execNoeud);
    virtual void AjouterAuxBoutonsVertic(std::shared_ptr<ExecNoeud> execNoeud);

private:
    Ui::Effet *ui;
};

#endif // EXECEFFET_H
