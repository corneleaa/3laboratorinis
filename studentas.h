#ifndef STUDENTAS_H
#define STUDENTAS_H

#include "zmogus.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>

double vidurkis(const std::vector<int>& v);
double mediana(const std::vector<int>& v);

inline double mediana_ref(const std::vector<int>& v) {
    std::vector<int> t = v;
    return mediana(t);
}

class Studentas : public Zmogus{
private:
    std::vector<int> nd_;
    int egzaminas_;
    double galutinis_;

public:
    Studentas();
    Studentas(const Studentas& other);
    Studentas& operator=(const Studentas& other);
    ~Studentas();

    Studentas(std::istream& is);

    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }
    inline double galutinis() const { return galutinis_; }

    inline void setVardas(const std::string& v) { vardas_ = v; }
    inline void setPavarde(const std::string& p) { pavarde_ = p; }
    inline void setEgzaminas(int e) { egzaminas_ = e; }
    inline void setNd(const std::vector<int>& v) { nd_ = v; }

    std::istream& readStudent(std::istream& is);
    double skaiciuotiGalutini(double (*f)(const std::vector<int>&)) const;
    void perskaiciuoti(double (*f)(const std::vector<int>&));
    void spausdinti(std::ostream& os) const;
    friend std::istream& operator>>(std::istream& is, Studentas& s);
    friend std::ostream& operator<<(std::ostream& os, const Studentas& s);
};

bool comparePagalVarda(const Studentas&, const Studentas&);
bool comparePagalPavarde(const Studentas&, const Studentas&);
bool comparePagalGalutini(const Studentas&, const Studentas&);

std::vector<Studentas> nuskaitytiIsFailo(const std::string& failas);

template <typename Container>
Container nuskaitytiIsFailoT(const std::string& failas) {
    Container grupe;
    std::ifstream in(failas);
    if (!in) {
        std::cerr << "Nepavyko atidaryti failo: " << failas << std::endl;
        return grupe;
    }
    std::string eil;
    std::getline(in, eil);
    while (std::getline(in, eil)) {
        if (eil.empty()) continue;
        std::istringstream iss(eil);
        Studentas s(iss);
        grupe.emplace_back(std::move(s));
    }
    std::cout << "Nuskaityta studentų: " << grupe.size() << std::endl;
    return grupe;
}

void isvestiStudentus(const std::vector<Studentas>& gr, const std::string& failas);

template <typename Container>
void isvestiStudentusT(const Container& gr, const std::string& failas) {
    std::ofstream out(failas);
    out << std::left << std::setw(12) << "Vardas"
        << std::setw(12) << "Pavarde"
        << std::setw(10) << "Galutinis" << "\n";
    for (const auto& s : gr) s.spausdinti(out);
}

void generuotiFaila(const std::string& failoVardas, int kiek);
Studentas generuotiStudenta(int id);

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

#endif

