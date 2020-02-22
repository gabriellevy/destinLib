#ifndef EXECLANCERDE_H
#define EXECLANCERDE_H

#include "execnoeud.h"
#include "execeffet.h"
#include "../abs/lancerde.h"

namespace Ui {
class LancerDe;
}

struct ResExecutionLancerDe;
class ExecEffet;
class LancerDe;

class ExecLancerDe : public ExecNoeud,
        my_enable_shared_from_this<ExecLancerDe>
{
    Q_OBJECT

public:
    using my_enable_shared_from_this<ExecLancerDe>::shared_from_this;
    ExecLancerDe(std::shared_ptr<ExecEffet> exec_effet, std::shared_ptr<LancerDe> effet, QWidget *parent = nullptr);
    virtual ~ExecLancerDe();

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);
    virtual bool GestionTransition();

    //virtual void FinExecutionNoeud();
    void NettoyageAffichage();
    std::shared_ptr<ExecChoix> AjoutChoixGoToEffet(QString texte, QString idDest);
    void ChangerIntituleBouton(QString texte);

    void AfficherNoeud();

    std::shared_ptr<LancerDe> m_LancerDe = nullptr;
    std::weak_ptr<ExecEffet> m_ExecEffet;
    std::shared_ptr<ExecNoeud> GetExecNoeud();
    int GetTotalRes();

    // résultat final de l'exécution de la fonction callback du lancer de dé
    std::shared_ptr<ResExecutionLancerDe> m_ResExecution = nullptr;

protected:
    virtual void AjouterAuxBoutonsHoriz(std::shared_ptr<ExecNoeud> execNoeud);
    virtual void AjouterAuxBoutonsVertic(std::shared_ptr<ExecNoeud> execNoeud);

private:
    Ui::LancerDe *ui;

    // résultat final de chaque dé lancé
    QVector<int> m_Res;

    QPixmap m_ImgDe[6];// images des 6 faces de dés

public slots:
    void ExecuterNoeudSlot();
};

#endif // EXECLANCERDE_H
