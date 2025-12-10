/// @file zmogus.h
/// @brief Abstrakti bazinė klasė asmeniui su vardu ir pavarde.
/// @ingroup core

#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>
#include <iostream>
#include <iomanip>
/**
 * @defgroup core Pagrindinės klasės
 * @brief Zmogus ir Studentas, jų API ir kontraktai.
 */

/**
 * @class Zmogus
 * @ingroup core
 * @brief Abstrakti bazinė klasė asmeniui su vardu ir pavarde.
 *
 * Klasė skirta paveldėjimui. Privaloma perrašyti @ref spausdinti.
 */
class Zmogus {
protected:
    std::string vardas_;   /**< @brief Vardas */
    std::string pavarde_;  /**< @brief Pavardė */
public:
    /// @name Konstruktoriai / destruktorius
    ///@{
    /// Tuščias konstruktorius.
    Zmogus() : vardas_(""), pavarde_("") {}
    /// Konstruktorius su reikšmėmis.
    /// @param v vardas
    /// @param p pavardė
    Zmogus(const std::string& v, const std::string& p) : vardas_(v), pavarde_(p) {}

    virtual ~Zmogus() = default;
    ///@}

    /// @name Get’ai ir set’ai
    ///@{
    /// @return Vardas

    inline std::string vardas() const { return vardas_; }
    /// @return Pavardė
    inline std::string pavarde() const { return pavarde_; }
    /// Nustato vardą.
    /// @param v naujas vardas

    inline void setVardas(const std::string& v) { vardas_ = v; }
    /// Nustato pavardę.
    /// @param p nauja pavardė
    inline void setPavarde(const std::string& p) { pavarde_ = p; }
    ///@}

    /**
     * @brief Abstrakti spausdinimo funkcija.
     * @param os išvesties srautas
     */
    virtual void spausdinti(std::ostream& os) const = 0;
};

#endif

