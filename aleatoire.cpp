#include "aleatoire.h"
#include <QDebug>

Aleatoire* Aleatoire::ME = nullptr;

Aleatoire::Aleatoire()
{
    // construct a trivial random generator engine from a time-based seed:
    m_Graine = std::chrono::system_clock::now().time_since_epoch().count();

    m_Generateur = std::default_random_engine(m_Graine);

    m_Distribution = std::uniform_int_distribution<int>(0, std::numeric_limits<int>::max());
}


double Aleatoire::Entre0Et1()
{
    double val = static_cast<double>(m_Distribution(m_Generateur)) / static_cast<double>(std::numeric_limits<int>::max());

    return val;
}


Aleatoire* Aleatoire::GetAl()
{
    if ( ME == nullptr)
        ME = new Aleatoire();
    return ME;
}

int Aleatoire::EntierEntreAEtB(int A, int B)
{
    if ( (B-A) <= 0)
        return 0;
    return A + ( m_Distribution(m_Generateur) ) % (B-A + 1);
}

int Aleatoire::EntierInferieurA(int A)
{
    return EntierEntreAEtB(0, A-1);
}


int Aleatoire::D6()
{
    return EntierEntreAEtB(1, 7);
}

int Aleatoire::GetIndexSelonPoids(QList<double> /*poids*/)
{
    // à afire un jour ! mais peut être avec une fonction de calcul de poids en param et un Template T
    return -1;

}
