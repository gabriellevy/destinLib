#ifndef INVASIONKRAKEN_H
#define INVASIONKRAKEN_H

#include "exechistoire.h"


class InvasionKraken : public ExecHistoire
{
    Q_OBJECT
public:
    explicit InvasionKraken(QWidget *parent = nullptr);

    virtual void GenererHistoire();

    static QString biomasse;
    static QString icharIV;
    static QString miral;
    static QString larnarno;
    static QString salem;
    static QString devlan_system;
    static QString genestealers_exploration;
    static QString biomasse_exploration;
    static QString lictors_exploration;
    static QString exploration_capture;
    static QString exploration_massacre;
    static QString gaunts_exploration;

private:
    void GenererDebutExploration();

signals:

public slots:
};

#endif // INVASIONKRAKEN_H
