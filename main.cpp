/**
 * @file main.cpp
 * @ingroup app
 * @brief CLI programa: duomenų generavimas/nuskaitymas, rikiavimas ir skirstymas, laiko matavimai.
 *
 * Vartotojas renkasi: failo režimą, skaičiavimo strategiją (vidurkis/mediana),
 * konteinerį (vector/list), rikiavimo kriterijų, skirstymo strategiją.
 */

#include "studentas.h"
#include <filesystem>
#include <list>
#include <vector>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>

static std::string rastiFaila(const std::string& failas) {
    namespace fs = std::filesystem;
    fs::path p = fs::current_path() / failas;
    if (fs::exists(p)) return p.string();
    fs::path p1 = fs::current_path().parent_path() / failas;
    if (fs::exists(p1)) return p1.string();
    fs::path p2 = fs::current_path().parent_path().parent_path() / failas;
    if (fs::exists(p2)) return p2.string();
    return "";
}

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    using std::cout;
    using std::cin;
    
    cout << "Pasirinkite režimą:\n"
         << "1) Generuoti naują failą\n"
         << "2) Naudoti esamą failą\n"
         << "3) Įvesti duomenis ranka\n";
    int rez;
    if (!(cin >> rez)) return 0;

    std::string failas;
    int kiek = 0;

    if (rez == 1) {
        cout << "Įveskite failo pavadinimą: ";
        cin >> failas;
        cout << "Kiek studentų generuoti? ";
        cin >> kiek;

        auto t0 = std::chrono::high_resolution_clock::now();
        generuotiFaila(failas, kiek);
        auto t1 = std::chrono::high_resolution_clock::now();

        cout << "Failas sugeneruotas per "
             << std::chrono::duration<double>(t1 - t0).count() << " s.\n";
        return 0;
    } else if (rez == 3) {
        cout << "Kiek studentų norite įvesti? ";
        cin >> kiek;

        std::vector<Studentas> gr;
        gr.reserve(kiek);

        for (int i = 0; i < kiek; ++i) {
            std::string v, p;
            std::vector<int> nd(5);
            int egz;

            cout << "\nStudentas #" << i+1 << "\nVardas: ";
            cin >> v;
            cout << "Pavardė: ";
            cin >> p;
            cout << "Įveskite 5 ND: ";
            for (int j = 0; j < 5; ++j) cin >> nd[j];
            cout << "Egzaminas: ";
            cin >> egz;

            Studentas s;
            s.setVardas(v);
            s.setPavarde(p);
            s.setNd(nd);
            s.setEgzaminas(egz);
            s.perskaiciuoti(vidurkis);

            Studentas& ref = gr.emplace_back(s);
            cout << "Objekto adresas atmintyje (saugojamas konteineryje): " << &ref << "\n";
        }

        std::ofstream out("ivedimas_rankinis.txt");
        out << std::left << std::setw(12) << "Vardas"
            << std::setw(12) << "Pavarde"
            << std::setw(10) << "Galutinis" << "\n";
        for (auto& s : gr) s.spausdinti(out);

        cout << "Rezultatai įrašyti į ivedimas_rankinis.txt\n";
        return 0;
    } else {
        cout << "Įveskite esamo failo pavadinimą: ";
        cin >> failas;

        cout << "Darbinis katalogas: " << std::filesystem::current_path() << "\n";
        std::string tikras = rastiFaila(failas);
        if (tikras.empty()) {
            std::cerr << "Nepavyko rasti failo: " << failas << "\n";
            return 1;
        }
        cout << "Naudojamas failas: " << tikras << "\n";
        failas = tikras;
    }

    cout << "Pasirinkite galutinio balo skaičiavimo būdą:\n"
         << "1) Pagal vidurkį\n"
         << "2) Pagal medianą\n"
         << "3) Pagal abu\n";
    int tipas;
    cin >> tipas;

    auto fptr = (tipas == 2) ? mediana_ref : vidurkis;

    cout << "Pasirinkite konteinerį:\n1) std::vector\n2) std::list\n";
    int kon;
    cin >> kon;

    cout << "Rikiuoti pagal:\n1) vardą\n2) pavardę\n3) galutinį\n";
    int rik;
    cin >> rik;

    cout << "Pasirinkite strategiją:\n"
            "1) Kopijuoti į du naujus (A+B)\n"
            "2) Tik vargšiukai (ištrinti iš bendro)\n"
            "3) Optimizuota greičiausia\n";
int strategija;
cin >> strategija;
{
    std::cout << "Rule of Three demonstracija (v2.0):\n";

    Studentas a;
    a.setVardas("Testas");
    a.setPavarde("Kopijavimas");
    a.setNd({10,9,8,7,6});
    a.setEgzaminas(9);
    a.perskaiciuoti(vidurkis);
    
    Studentas b = a;
    Studentas c; 
    c = a;

    std::cout << "Rule of Three test:\n";
    std::cout << b.vardas() << " " << b.pavarde() << "\n";
    std::cout << c.vardas() << " " << c.pavarde() << "\n";

    std::vector<Studentas> demo;
    demo.reserve(1000);
    for (int i = 0; i < 500; ++i) {
        demo.push_back(a); 
    }
} 

auto t_total0 = std::chrono::high_resolution_clock::now();

    if (kon == 1) {
        auto t_r0 = std::chrono::high_resolution_clock::now();
        std::vector<Studentas> gr = nuskaitytiIsFailo(failas);
        auto t_r1 = std::chrono::high_resolution_clock::now();

        for (auto& s : gr) s.perskaiciuoti(fptr);

        auto t_s0 = std::chrono::high_resolution_clock::now();
        if (rik == 1)      std::sort(gr.begin(), gr.end(), comparePagalVarda);
        else if (rik == 2) std::sort(gr.begin(), gr.end(), comparePagalPavarde);
        else               std::sort(gr.begin(), gr.end(), comparePagalGalutini);
        auto t_s1 = std::chrono::high_resolution_clock::now();

        std::vector<Studentas> vargs, kiet;
        auto t_sp0 = std::chrono::high_resolution_clock::now();
        if (strategija == 1)
            split_strat1_vector(gr, vargs, kiet);
        else if (strategija == 2) {
            split_strat2_vector(gr, vargs);
            kiet = std::move(gr);
        } else {
            split_strat3_vector(gr, vargs);
            kiet = std::move(gr);
        }
        auto t_sp1 = std::chrono::high_resolution_clock::now();

        std::filesystem::create_directory("results");
        auto t_w0 = std::chrono::high_resolution_clock::now();
        isvestiStudentus(vargs, "results/vargsiukai.txt");
        isvestiStudentus(kiet,  "results/kietuoliai.txt");
        auto t_w1 = std::chrono::high_resolution_clock::now();

        auto t_total1 = std::chrono::high_resolution_clock::now();

        cout << std::fixed << std::setprecision(6)
             << "\nLaikai (vector):\n"
             << "read = "  << std::chrono::duration<double>(t_r1 - t_r0).count()  << " s\n"
             << "sort = "  << std::chrono::duration<double>(t_s1 - t_s0).count()  << " s\n"
             << "split = " << std::chrono::duration<double>(t_sp1 - t_sp0).count() << " s\n"
             << "write = " << std::chrono::duration<double>(t_w1 - t_w0).count()  << " s\n"
             << "total = " << std::chrono::duration<double>(t_total1 - t_total0).count() << " s\n";

    } else {
        auto t_r0 = std::chrono::high_resolution_clock::now();
        std::list<Studentas> gr = nuskaitytiIsFailoT<std::list<Studentas>>(failas);
        auto t_r1 = std::chrono::high_resolution_clock::now();

        for (auto& s : gr) s.perskaiciuoti(fptr);

        auto t_s0 = std::chrono::high_resolution_clock::now();
        if (rik == 1)      gr.sort(comparePagalVarda);
        else if (rik == 2) gr.sort(comparePagalPavarde);
        else               gr.sort(comparePagalGalutini);
        auto t_s1 = std::chrono::high_resolution_clock::now();

        std::list<Studentas> vargs, kiet;
        auto t_sp0 = std::chrono::high_resolution_clock::now();
        if (strategija == 1)
            split_strat1_list(gr, vargs, kiet);
        else if (strategija == 2) {
            split_strat2_list(gr, vargs);
            kiet = std::move(gr);
        } else {
            split_strat3_list(gr, vargs);
            kiet = std::move(gr);
        }
        auto t_sp1 = std::chrono::high_resolution_clock::now();

        std::filesystem::create_directory("results");
        auto t_w0 = std::chrono::high_resolution_clock::now();
        isvestiStudentusT(vargs, "results/vargsiukai.txt");
        isvestiStudentusT(kiet,  "results/kietuoliai.txt");
        auto t_w1 = std::chrono::high_resolution_clock::now();

        auto t_total1 = std::chrono::high_resolution_clock::now();

        cout << std::fixed << std::setprecision(6)
             << "\nLaikai (list):\n"
             << "read = "  << std::chrono::duration<double>(t_r1 - t_r0).count()  << " s\n"
             << "sort = "  << std::chrono::duration<double>(t_s1 - t_s0).count()  << " s\n"
             << "split = " << std::chrono::duration<double>(t_sp1 - t_sp0).count() << " s\n"
             << "write = " << std::chrono::duration<double>(t_w1 - t_w0).count()  << " s\n"
             << "total = " << std::chrono::duration<double>(t_total1 - t_total0).count() << " s\n";
    }

    std::cout << "\nRezultatai įrašyti į 'results/'.\n";
    return 0;
}
