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

int Aleatoire::GetSeed()
{
    if ( m_Seed == -1)
        m_Seed = Aleatoire::EntierInferieurA(std::numeric_limits<int>::max());
    return m_Seed;
}

double Aleatoire::Entre0Et1(bool utiliserSeed)
{
    double val;
    if ( utiliserSeed )
        val = GetSeed() / static_cast<double>(std::numeric_limits<int>::max());
    else
        val = static_cast<double>(m_Distribution(m_Generateur)) / static_cast<double>(std::numeric_limits<int>::max());

    return val;
}


Aleatoire* Aleatoire::GetAl()
{
    if ( ME == nullptr)
        ME = new Aleatoire();
    return ME;
}

bool Aleatoire::GetBool(bool utiliserSeed)
{
    if ( utiliserSeed)
        return GetSeed()%2==0;
    return EntierInferieurA(2) == 0;
}

int Aleatoire::EntierEntreAEtB(int A, int B, bool utiliserSeed)
{
    if ( (B-A) <= 0)
        return 0;
    if ( utiliserSeed )
        return A + ( GetSeed() ) % (B-A + 1);
    return A + ( m_Distribution(m_Generateur) ) % (B-A + 1);
}

int Aleatoire::EntierInferieurA(int A, bool utiliserSeed)
{
    return EntierEntreAEtB(0, A-1, utiliserSeed);
}


int Aleatoire::D6(bool utiliserSeed)
{
    return EntierEntreAEtB(1, 7, utiliserSeed);
}

int Aleatoire::GetIndexSelonPoids(QList<double> /*poids*/, bool /*utiliserSeed*/)
{
    // à afire un jour ! mais peut être avec une fonction de calcul de poids en param et un Template T
    return -1;

}
