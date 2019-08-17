#include "setcarac.h"
#include "perso.h"
#include "univers.h"
#include <QDebug>
#include "gestionnairecarac.h"
#include <chrono>
#include <random>

SetCarac::SetCarac(ModifCaracType modifCaracType, QString caracId, QString valeur)
{
    m_CaracId = caracId;
    m_ModifCaracType = modifCaracType;
    m_Valeur = valeur;
}

SetCarac::SetCarac(QJsonObject obj, ModifCaracType modifCaracType, QString valeur)
{
    m_ModifCaracType = modifCaracType;

    if ( obj.contains("carac_id") && obj["carac_id"].isString())
    {
        m_CaracId = obj["carac_id"].toString();
    }
    else qDebug()<<"Pas de carac id pour le set carac";

    if ( obj.contains("valeur") && obj["valeur"].isString())
    {
        m_Valeur = obj["valeur"].toString();
    }
    else if ( obj.contains("valeur_random") && obj["valeur_random"].isString() )
    {
        m_ValeurRandom = obj["valeur_random"].toString();
    }
    else if ( obj.contains("valeur_carac") && obj["valeur_carac"].isString() )
    {
        m_IdValeurCaracCopie = obj["valeur_carac"].toString();
    }
    else if ( obj.contains("valeur_min") && obj["valeur_min"].isString() &&
              obj.contains("valeur_max") && obj["valeur_max"].isString())
    {
        m_ValeurMin = obj["valeur_min"].toString();
        m_ValeurMax = obj["valeur_max"].toString();
    }
    else if ( valeur != nullptr)
    {
        m_Valeur = valeur;
    }
    else
        qDebug()<<"Pas de valeur pour le set carac. m_CaracId : "<<m_CaracId << " id : "<<obj["id"];

    /*if ( obj.contains("intitule") && obj["intitule"].isString())
    {
        m_Intitule = obj["intitule"].toString();
    }*/

}

ModifCaracType SetCarac::GetModifCaracTypeFromQString(QString str)
{
    if ( str == "SetCarac") {
        return ModifCaracType::SetCarac;
    } else if ( str == "AddToCarac") {
        return ModifCaracType::AddToCarac;
    } else if ( str == "RetireDeCarac") {
        return ModifCaracType::RetireDeCarac;
    }
    Q_ASSERT_X(true, "SetCarac::GetModifCaracTypeFromQString", "type qui n'existe pas");
    return ModifCaracType::SetCarac;
}

QString SetCarac::GetQStringFromModifCaracType(ModifCaracType mct)
{
    if ( mct == ModifCaracType::SetCarac) {
        return "SetCarac";
    } else if ( mct == ModifCaracType::AddToCarac) {
        return "AddToCarac";
    } else if ( mct == ModifCaracType::RetireDeCarac) {
        return "RetireDeCarac";
    }
    Q_ASSERT_X(true, "SetCarac::GetQStringFromModifCaracType", "type qui n'existe pas");
    return "";
}

QString SetCarac::GetValeur()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    if ( m_Valeur != "nexistepas*µ£$")
        return m_Valeur;
    else if ( m_ValeurRandom != "nexistepas*µ£$" )
    {
        std::uniform_int_distribution<int> distribution(0, m_ValeurRandom.toInt());

        return QString::number(distribution(generator));
    }
    else if ( m_ValeurMin != "nexistepas*µ£$" && m_ValeurMax != "nexistepas*µ£$")
    {
        Q_ASSERT_X( m_ValeurMax.toInt() > m_ValeurMin.toInt(), "GetValeur", "Valeur max doit être supérieur à valeur min !!");
        std::uniform_int_distribution<int> distribution(0, m_ValeurMax.toInt() - m_ValeurMin.toInt());
        int valeur = m_ValeurMin.toInt() + distribution(generator);
        return QString::number(valeur);
    }
    else if ( m_IdValeurCaracCopie != "nexistepas*µ£$" )
    {
        QString valeurStr = GestionnaireCarac::GetGestionnaireCarac()->GetCaracValue(m_IdValeurCaracCopie);
        if ( valeurStr == "" )
            valeurStr = "0";
        int valeur =  valeurStr.toInt();
        return QString::number(valeur);
    }
    QString msg = "Pas de valeur pour ce set carac d'id :  " + m_CaracId ;
    Q_ASSERT_X(false, "SetCarac", msg.toStdString().c_str() );
    return "";
}
