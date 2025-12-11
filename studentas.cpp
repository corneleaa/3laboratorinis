#include "studentas.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

std::atomic<unsigned long long> Studentas::g_next_id_{0};

//Konstruktoriai destruktorius priskyrimai

Studentas::Studentas() : Zmogus(), egzaminas_(0), galutinis_(0.0) {
    init_new_instance_gen1_();
}

Studentas::Studentas(const Studentas& other)
    : Zmogus(other.vardas_, other.pavarde_),
      nd_(other.nd_),
      egzaminas_(other.egzaminas_),
      galutinis_(other.galutinis_) {
    init_copied_from_(other);
}

Studentas::Studentas(Studentas&& other) noexcept
    : Zmogus(std::move(other.vardas_), std::move(other.pavarde_)),
      nd_(std::move(other.nd_)),
      egzaminas_(other.egzaminas_),
      galutinis_(other.galutinis_) {
    init_moved_from_(std::move(other));
}

Studentas::~Studentas() = default;

Studentas& Studentas::operator=(const Studentas& other) {
    if (this != &other) {
        vardas_    = other.vardas_;
        pavarde_   = other.pavarde_;
        nd_        = other.nd_;
        egzaminas_ = other.egzaminas_;
        galutinis_ = other.galutinis_;
        init_copied_from_(other);
    }
    return *this;
}

Studentas& Studentas::operator=(Studentas&& other) noexcept {
    if (this != &other) {
        vardas_    = std::move(other.vardas_);
        pavarde_   = std::move(other.pavarde_);
        nd_        = std::move(other.nd_);
        egzaminas_ = other.egzaminas_;
        galutinis_ = other.galutinis_;
        init_moved_from_(std::move(other));
    }
    return *this;
}

Studentas::Studentas(std::istream& is) {
    readStudent(is);
}

Studentas::Studentas(std::string v,
                     std::string p,
                     std::vector<int> nd,
                     int egz,
                     double (*f)(const std::vector<int>&))
    : Zmogus(std::move(v), std::move(p)),
      nd_(std::move(nd)),
      egzaminas_(egz) {
    init_new_instance_gen1_();
    perskaiciuoti(f ? f : vidurkis);
}

// funkcijos

double vidurkis(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    double s = 0;
    for (int x : v) s += x;
    return s / static_cast<double>(v.size());
}

double mediana(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    std::vector<int> t = v;
    std::sort(t.begin(), t.end());
    const std::size_t n = t.size();
    if (n % 2 == 0) return (t[n/2 - 1] + t[n/2]) / 2.0;
    return static_cast<double>(t[n/2]);
}

double mediana_ref(const std::vector<int>& v) {
    return mediana(v); // saugo originalą, nes rikiuojama kopija
}

//I/O

std::istream& operator>>(std::istream& is, Studentas& s) {
    s.nd_.assign(5, 0);
    if (!(is >> s.vardas_ >> s.pavarde_)) return is;
    for (int i = 0; i < 5; ++i) {
        if (!(is >> s.nd_[i])) return is;
    }
    if (!(is >> s.egzaminas_)) return is;
    s.perskaiciuoti(vidurkis);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Studentas& s) {
    os << std::left << std::setw(12) << s.vardas_
       << std::setw(12) << s.pavarde_
       << std::setw(10) << std::fixed << std::setprecision(2)
       << s.galutinis_;
    return os;
}

std::istream& Studentas::readStudent(std::istream& is) {
    is >> vardas_ >> pavarde_;
    nd_.assign(5, 0);
    for (int i = 0; i < 5; ++i) is >> nd_[i];
    is >> egzaminas_;
    galutinis_ = skaiciuotiGalutini(vidurkis);
    init_new_instance_gen1_();
    return is;
}

// skaiciavimai

double Studentas::skaiciuotiGalutini(double (*f)(const std::vector<int>&)) const {
    return 0.4 * (f ? f(nd_) : vidurkis(nd_)) + 0.6 * static_cast<double>(egzaminas_);
}

void Studentas::perskaiciuoti(double (*f)(const std::vector<int>&)) {
    galutinis_ = skaiciuotiGalutini(f);
}

void Studentas::spausdinti(std::ostream& os) const {
    os << std::left << std::setw(12) << vardas_
       << std::setw(12) << pavarde_
       << std::setw(10) << std::fixed << std::setprecision(2) << galutinis_
       << "\n";
}

// palyginimai ir rikiavimai

bool Studentas::operator==(const Studentas& rhs) const {
    return vardas_    == rhs.vardas_    &&
           pavarde_   == rhs.pavarde_   &&
           nd_        == rhs.nd_        &&
           egzaminas_ == rhs.egzaminas_ &&
           std::fabs(galutinis_ - rhs.galutinis_) < 1e-9;
}

bool comparePagalVarda(const Studentas& a, const Studentas& b)   { return a.vardas()   < b.vardas(); }
bool comparePagalPavarde(const Studentas& a, const Studentas& b) { return a.pavarde()  < b.pavarde(); }
bool comparePagalGalutini(const Studentas& a, const Studentas& b){ return a.galutinis() < b.galutinis(); }

// strategijos

void split_strat1_vector(const std::vector<Studentas>& src,
                         std::vector<Studentas>& vargs,
                         std::vector<Studentas>& kiet) {
    vargs.clear();
    kiet.clear();
    vargs.reserve(src.size());
    kiet.reserve(src.size());
    for (const auto& s : src) {
        if (s.galutinis() < 5.0) vargs.push_back(s);
        else                     kiet.push_back(s);
    }
    vargs.shrink_to_fit();
    kiet.shrink_to_fit();
}

void split_strat2_vector(std::vector<Studentas>& all,
                         std::vector<Studentas>& vargs) {
    vargs.clear();
    vargs.reserve(all.size());
    std::size_t keep = 0;
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].galutinis() < 5.0) {
            vargs.push_back(all[i]);
        } else {
            all[keep++] = std::move(all[i]);
        }
    }
    all.erase(all.begin() + static_cast<std::ptrdiff_t>(keep), all.end());
    all.shrink_to_fit();
    vargs.shrink_to_fit();
}

void split_strat3_vector(std::vector<Studentas>& all,
                         std::vector<Studentas>& vargs) {
    vargs.clear();
    vargs.reserve(all.size() / 2);
    std::remove_copy_if(all.begin(), all.end(), std::back_inserter(vargs),
                        [](const Studentas& s){ return !(s.galutinis() < 5.0); });
    auto it = std::remove_if(all.begin(), all.end(),
                             [](const Studentas& s){ return s.galutinis() < 5.0; });
    all.erase(it, all.end());
    all.shrink_to_fit();
    vargs.shrink_to_fit();
}

void split_strat1_list(const std::list<Studentas>& src,
                       std::list<Studentas>& vargs,
                       std::list<Studentas>& kiet) {
    vargs.clear();
    kiet.clear();
    for (const auto& s : src) {
        if (s.galutinis() < 5.0) vargs.push_back(s);
        else                     kiet.push_back(s);
    }
}
void split_strat2_list(std::list<Studentas>& all,
                       std::list<Studentas>& vargs) {
    vargs.clear();
    for (auto it = all.begin(); it != all.end(); ) {
        if (it->galutinis() < 5.0) {
            vargs.push_back(*it);
            it = all.erase(it);
        } else {
            ++it;
        }
    }
}

void split_strat3_list(std::list<Studentas>& all,
                       std::list<Studentas>& vargs) {
    split_strat2_list(all, vargs);
}
// wrapperiai

std::vector<Studentas> nuskaitytiIsFailo(const std::string& failas) {
    return nuskaitytiIsFailoT<std::vector<Studentas>>(failas);
}

void isvestiStudentus(const std::vector<Studentas>& gr, const std::string& failas) {
    isvestiStudentusT(gr, failas);
}
//generatoriai
Studentas generuotiStudenta(int id) {
    std::vector<int> nd(5);
    for (int& x : nd) x = 1 + std::rand() % 10;
    int egz = 1 + std::rand() % 10;
    return Studentas("Vardas" + std::to_string(id),
                     "Pavarde" + std::to_string(id),
                     std::move(nd),
                     egz,
                     vidurkis);
}
void generuotiFaila(const std::string& failoVardas, int kiek) {
    std::ofstream out(failoVardas);
    out << "Vardas Pavarde ND1 ND2 ND3 ND4 ND5 Egzaminas\n";
    for (int i = 1; i <= kiek; ++i) {
        auto s = generuotiStudenta(i);
        const auto& nd = s.nd();
        out << s.vardas() << ' ' << s.pavarde() << ' '
            << nd[0] << ' ' << nd[1] << ' ' << nd[2] << ' ' << nd[3] << ' ' << nd[4] << ' '
            << s.egzaminas() << '\n';
    }
}


