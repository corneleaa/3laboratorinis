#pragma once
#include <string>
#include <utility>

/**
 * @file zmogus.h
 * @ingroup model
 * @brief Bazinė klasė bendriems laukams (vardas, pavardė).
 */

/**
 * @class Zmogus
 * @brief Bazinė abstrakcija su vardu ir pavarde.
 */
class Zmogus {
protected:
    std::string vardas_;   /**< Vardas */
    std::string pavarde_;  /**< Pavardė */
public:
    /** @brief Numatytoji konstruktorius. */
    inline Zmogus() = default;

    /**
     * @brief Pilnas konstruktorius.
     * @param v vardas
     * @param p pavardė
     */
    inline Zmogus(std::string v, std::string p)
        : vardas_(std::move(v)), pavarde_(std::move(p)) {}

    /** @brief Virtualus destruktorius. */
    inline virtual ~Zmogus() = default;

    /** @brief Prieiga prie vardo. */
    inline const std::string& vardas_ref() const { return vardas_; }
    /** @brief Prieiga prie pavardės. */
    inline const std::string& pavarde_ref() const { return pavarde_; }
};

