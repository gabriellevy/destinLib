#ifndef GENESTEALER_H
#define GENESTEALER_H

#include <QWidget>
#include "perso.h"

/*namespace Ui {
class Genestealer;
}*/

/**
 * @brief Malgré son nom on va estimer pour l'instant que cette classe concerne tous
 * les personnages du culte genestealer
 */
class Genestealer : public IPerso
{
    Q_OBJECT

public:
    explicit Genestealer(QWidget *parent = nullptr);
    //~Genestealer();

    virtual void GenererPersos();

/*private:
    Ui::Genestealer *ui_Genestealer;*/
};

#endif // GENESTEALER_H
