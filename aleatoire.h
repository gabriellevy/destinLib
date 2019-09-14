#ifndef ALEATOIRE_H
#define ALEATOIRE_H

#include <chrono>
#include <random>

class Aleatoire
{
    long long m_Graine;
    std::default_random_engine m_Generateur;
    std::uniform_int_distribution<int> m_Distribution;


    static Aleatoire* ME;

public:
    Aleatoire();

    static Aleatoire* GetAl();

    // retourne entier entre A et B (inclus)
    int EntierEntreAEtB(int A, int B);

    // retourne entier entre 0 (inclu) et A (exclu)
    int EntierInferieurA(int A);

    int D6();
};

#endif // ALEATOIRE_H
