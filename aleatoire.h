#ifndef ALEATOIRE_H
#define ALEATOIRE_H

#include <chrono>
#include <random>
#include <QList>
#include <memory>

class Aleatoire
{
private:
    long long m_Graine;
    std::default_random_engine m_Generateur;
    std::uniform_int_distribution<int> m_Distribution;

    // seed qui quand elle est utilisée renvoie toujours la même valeur aux même demande (donc demander x fois un nombre entre 0 et 100 donnera à chaque fois le même résultat)
    int m_Seed = -1;
    int GetSeed();

    static Aleatoire* ME;

public:
    Aleatoire();

    static Aleatoire* GetAl();

    // retourne entier entre A et B (inclus)
    int EntierEntreAEtB(int A, int B, bool utiliserSeed = false);

    // retourne entier entre 0 (inclu) et A (exclu)
    int EntierInferieurA(int A, bool utiliserSeed = false);

    int D6(bool utiliserSeed = false);

    bool GetBool(bool utiliserSeed = false);

    double Entre0Et1(bool utiliserSeed = false);
    int GetIndexSelonPoids(QList<double> poids, bool utiliserSeed = false);
};

#endif // ALEATOIRE_H
