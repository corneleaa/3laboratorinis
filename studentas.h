#pragma once
#include "zmogus.h"
#include <atomic>
#include <cstddef>
#include <iosfwd>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

/**
 * @file studentas.h
 * @ingroup model
 * @brief Studentas klasė, I/O API, matematinės funkcijos ir skirstymo strategijų deklaracijos.
 */

/** @brief Aritmetinis vidurkis. */
double vidurkis(const std::vector<int>& v);
/** @brief Mediana (rikiuoja kopiją). */
double mediana(const std::vector<int>& v);
/** @brief Mediana nekeičiant originalo (naudojama kopija). */
double mediana_ref(const std::vector<int>& v);

/**
 * @class Studentas
 * @brief Studentas su ND pažymiais, egzamino balu ir iš anksto laikomu galutiniu.
 *
 * Konstruktoriai pateikti su „by-value + move“ (copy-elision ir efektyvus pernešimas).
 */
class Studentas : public Zmogus {
    std::vector<int> nd_{};     /**< Namų darbų pažymiai */
    int egzaminas_{0};          /**< Egzamino balas */
    double galutinis_{0.0};     /**< Galutinis balas: 0.4*ND + 0.6*egz */

    // Diagnostikai ir Rule-of-Three/Rule-of-Five demonstracijai
    unsigned long long id_{0};  /**< Unikalus identifikatorius */
    unsigned long long gen_{0}; /**< Kiek kartų kopijuotas/perkeltas */

    static std::atomic<unsigned long long> g_next_id_;

    inline void init_copied_from_(const Studentas& other) {
        id_  = ++g_next_id_;
        gen_ = other.gen_ + 1;
    }
    inline void init_moved_from_(Studentas&& other) {
        id_  = ++g_next_id_;
        gen_ = other.gen_ + 1;
    }
    inline void init_new_instance_gen1_() {
        id_  = ++g_next_id_;
        gen_ = 1;
    }

public:
    /** @brief Tuščias konstruktorius. */
    Studentas();

    /** @brief Kopijavimo konstruktorius. */
    Studentas(const Studentas& other);

    /** @brief Perkėlimo konstruktorius. */
    Studentas(Studentas&& other) noexcept;

    /** @brief Destruktorius. */
    ~Studentas();

    /** @brief Kopijavimo priskyrimas. */
    Studentas& operator=(const Studentas& other);

    /** @brief Perkėlimo priskyrimas. */
    Studentas& operator=(Studentas&& other) noexcept;

    /**
     * @brief Konstruktorius iš srauto: skaito „Vardas Pavarde ND1..ND5 Egzaminas“.
     */
    explicit Studentas(std::istream& is);

    /**
     * @brief Pilnas konstruktorius.
     * @param v vardas
     * @param p pavardė
     * @param nd ND pažymiai
     * @param egz egzamino balas
     * @param f funkcija ND agregacijai (pvz., @ref vidurkis ar @ref mediana_ref)
     */
    Studentas(std::string v,
              std::string p,
              std::vector<int> nd,
              int egz,
              double (*f)(const std::vector<int>&) = vidurkis);

    /** @name Getteriai */
    ///@{
    inline std::string vardas()   const { return vardas_; }
    inline std::string pavarde()  const { return pavarde_; }
    inline const std::vector<int>& nd() const { return nd_; }
    inline int egzaminas()        const { return egzaminas_; }
    inline double galutinis()     const { return galutinis_; }
    inline unsigned long long id()  const { return id_; }
    inline unsigned long long gen() const { return gen_; }
    ///@}

    /** @name Setteriai */
    ///@{
    inline void setVardas(const std::string& v)  { vardas_ = v; }
    inline void setPavarde(const std::string& p) { pavarde_ = p; }
    inline void setNd(const std::vector<int>& v) { nd_ = v; }
    inline void setEgzaminas(int e)              { egzaminas_ = e; }
    ///@}

    /**
     * @brief Nuskaitymas iš srauto.
     * @return Tas pats @p is
     */
    std::istream& readStudent(std::istream& is);

    /**
     * @brief Apskaičiuoja galutinį pagal pateiktą agregavimo funkciją.
     */
    double skaiciuotiGalutini(double (*f)(const std::vector<int>&)) const;

    /**
     * @brief Perskaičiuoja ir įrašo @ref galutinis_.
     */
    void perskaiciuoti(double (*f)(const std::vector<int>&));

    /**
     * @brief Spausdina vieną eilutę: vardas, pavardė, galutinis.
     */
    void spausdinti(std::ostream& os) const;

    /** @brief Lyginimas pagal visus laukus. */
    bool operator==(const Studentas& rhs) const;
    /** @brief Nelygybė. */
    bool operator!=(const Studentas& rhs) const { return !(*this == rhs); }

    /** @brief Skaitymo operatorius. */
    friend std::istream& operator>>(std::istream& is, Studentas& s);
    /** @brief Rašymo operatorius. */
    friend std::ostream& operator<<(std::ostream& os, const Studentas& s);
};

/** @name Rikiavimo komparatoriai */
///@{
bool comparePagalVarda(const Studentas& a, const Studentas& b);
bool comparePagalPavarde(const Studentas& a, const Studentas& b);
bool comparePagalGalutini(const Studentas& a, const Studentas& b);
///@}

/** @name Skirstymo strategijos: <5.0 į „vargšiukai“, kiti į „kietuoliai“ */
///@{
void split_strat1_vector(const std::vector<Studentas>& src,
                         std::vector<Studentas>& vargs,
                         std::vector<Studentas>& kiet);
void split_strat2_vector(std::vector<Studentas>& all,
                         std::vector<Studentas>& vargs);
void split_strat3_vector(std::vector<Studentas>& all,
                         std::vector<Studentas>& vargs);

void split_strat1_list(const std::list<Studentas>& src,
                       std::list<Studentas>& vargs,
                       std::list<Studentas>& kiet);
void split_strat2_list(std::list<Studentas>& all,
                       std::list<Studentas>& vargs);
void split_strat3_list(std::list<Studentas>& all,
                       std::list<Studentas>& vargs);
///@}

/** @name Failų I/O utilitai */
///@{
/** @brief Nuskaitymas į konkretų konteinerio tipą. */
template <class Container>
Container nuskaitytiIsFailoT(const std::string& failas);
/** @brief Rašymas iš bet kurio konteinerio tipo. */
template <class Container>
void isvestiStudentusT(const Container& gr, const std::string& failas);
/** @brief Patogus vektoriaus wrapper’is. */
std::vector<Studentas> nuskaitytiIsFailo(const std::string& failas);
/** @brief Patogus vektoriaus wrapper’is. */
void isvestiStudentus(const std::vector<Studentas>& gr, const std::string& failas);
///@}

/** @name Generatoriai */
///@{
/** @brief Sugeneruoja vieną atsitiktinį studentą. */
Studentas generuotiStudenta(int id);
/** @brief Sugeneruoja failą „Vardas Pavarde ND1..ND5 Egzaminas“. */
void generuotiFaila(const std::string& failoVardas, int kiek);
///@}

template <class Container>
Container nuskaitytiIsFailoT(const std::string& failas) {
    std::ifstream in(failas);
    Container out;
    if (!in) return out;

    std::string header;
    std::getline(in, header);
    Studentas s;
    while (in >> s) {
        out.emplace_back(std::move(s));
    }
    return out;
}

template <class Container>
void isvestiStudentusT(const Container& gr, const std::string& failas) {
    std::ofstream out(failas);
    out << "Vardas       Pavarde      Galutinis\n";
    for (const auto& s : gr) s.spausdinti(out);
}

