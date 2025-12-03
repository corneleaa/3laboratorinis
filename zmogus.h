#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>
#include <iostream>
#include <iomanip>

class Zmogus {
protected:
    std::string vardas_;
    std::string pavarde_;

public:
    Zmogus() : vardas_(""), pavarde_("") {}
    Zmogus(const std::string& v, const std::string& p) : vardas_(v), pavarde_(p) {}

    virtual ~Zmogus() = default;

    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }

    inline void setVardas(const std::string& v) { vardas_ = v; }
    inline void setPavarde(const std::string& p) { pavarde_ = p; }
    virtual void spausdinti(std::ostream& os) const = 0;
};

#endif

