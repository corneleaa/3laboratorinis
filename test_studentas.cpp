#include <catch2/catch_all.hpp>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include "studentas.h"

using Catch::Approx;
// Pagalbinis konstruktorius sutrumpintam studentų kūrimui
static Studentas make_student(std::string v, std::string p,
                              std::vector<int> nd, int egz,
                              double (*f)(const std::vector<int>&) = vidurkis)
{
    Studentas s;
    s.setVardas(v);
    s.setPavarde(p);
    s.setNd(nd);
    s.setEgzaminas(egz);
    s.perskaiciuoti(f);
    return s;
}
// 1. Getteriai ir default konstruktorius
TEST_CASE("default ctor ir getteriai: pradines reiksmes", "[ctor][getters]") {
    Studentas s;
    REQUIRE( s.nd().size() == 0 );
    REQUIRE( s.egzaminas() == 0 );
    REQUIRE( s.galutinis() == Approx(0.0) );
}
// 2. Parametrizuotas konstruktorius
TEST_CASE("parametrizuotas ctor: visi laukai nustatyti ir galutinis perskaiciuotas", "[ctor][params]") {
    std::vector<int> nd{10, 8, 6, 4, 2};
    int egz = 9;
    Studentas s("Jonas", "Jonaitis", nd, egz, vidurkis);

    REQUIRE( s.vardas()    == "Jonas" );
    REQUIRE( s.pavarde()   == "Jonaitis" );
    REQUIRE( s.nd()        == nd );
    REQUIRE( s.egzaminas() == egz );
    REQUIRE( s.galutinis() == Approx(0.4*6.0 + 0.6*9.0) );
}
// 3. Lygybės operatoriai == ir !=
TEST_CASE("operator== ir operator!=: lyginami visi laukai", "[equality]") {
    auto mk = [](std::string v, std::string p, std::vector<int> nd, int egz){
        Studentas s; s.setVardas(v); s.setPavarde(p); s.setNd(nd); s.setEgzaminas(egz); s.perskaiciuoti(vidurkis); return s;
    };

    Studentas a = mk("A","B",{1,2,3,4,5},8);
    Studentas b = a;
    REQUIRE( a == b );
    REQUIRE_FALSE( a != b );

    Studentas c = mk("A","B",{1,2,3,4,5},7);
    REQUIRE_FALSE( a == c );
    REQUIRE( a != c );

    Studentas d = a;
    d.setNd({1,2,3,4,6});
    d.perskaiciuoti(vidurkis);
    REQUIRE_FALSE( a == d );
    REQUIRE( a != d );
}
// 4. Copy ctor ir copy assignment
TEST_CASE("copy ctor ir operator=: lygybes santykis", "[rule-of-three][copy]") {
    Studentas a; a.setVardas("V"); a.setPavarde("P"); a.setNd({1,2,3,4,5}); a.setEgzaminas(8); a.perskaiciuoti(vidurkis);

    Studentas b = a;
    Studentas c; c = a;

    REQUIRE( b == a );
    REQUIRE( c == a );
    REQUIRE_FALSE( b != a );
    REQUIRE_FALSE( c != a );

    // patikrinimai su getteriais
    REQUIRE( b.vardas()    == a.vardas() );
    REQUIRE( b.pavarde()   == a.pavarde() );
    REQUIRE( b.nd()        == a.nd() );
    REQUIRE( b.egzaminas() == a.egzaminas() );
    REQUIRE( b.galutinis() == Approx(a.galutinis()) );
}
// 5. Move ctor ir move assignment
TEST_CASE("move ctor: paskirties objektas gauna vertes", "[rule-of-five][move-ctor]") {
    Studentas src;
    src.setVardas("M"); src.setPavarde("N");
    src.setNd({7,7,7,7,7}); src.setEgzaminas(8); src.perskaiciuoti(vidurkis);
    const double g = src.galutinis();

    Studentas dst = std::move(src);
    REQUIRE( dst.vardas()    == "M" );
    REQUIRE( dst.pavarde()   == "N" );
    REQUIRE( dst.nd()        == std::vector<int>({7,7,7,7,7}) );
    REQUIRE( dst.egzaminas() == 8 );
    REQUIRE( dst.galutinis() == Approx(g) );
}

TEST_CASE("move assignment: paskirties objektas gauna vertes", "[rule-of-five][move-assign]") {
    Studentas a; a.setVardas("A"); a.setPavarde("B"); a.setNd({1,2,3,4,5}); a.setEgzaminas(6); a.perskaiciuoti(vidurkis);
    Studentas b; b.setVardas("X"); b.setPavarde("Y"); b.setNd({9,9,9,9,9}); b.setEgzaminas(10); b.perskaiciuoti(vidurkis);

    const double gA = a.galutinis();
    b = std::move(a);
    REQUIRE( b.vardas()    == "A" );
    REQUIRE( b.pavarde()   == "B" );
    REQUIRE( b.nd()        == std::vector<int>({1,2,3,4,5}) );
    REQUIRE( b.egzaminas() == 6 );
    REQUIRE( b.galutinis() == Approx(gA) );
}
// 6. Matematinės funkcijos: vidurkis, mediana, mediana_ref
TEST_CASE("vidurkis: baziniai ir kraštiniai atvejai", "[math][avg]") {
    SECTION("nelyginis kiekis") {
        std::vector<int> v{10, 8, 6, 4, 2};
        REQUIRE( vidurkis(v) == Approx(6.0) );
    }
    SECTION("lyginis kiekis") {
        std::vector<int> v{1, 2, 3, 4};
        REQUIRE( vidurkis(v) == Approx(2.5) );
    }
    SECTION("tuščias vektorius") {
        std::vector<int> empty;
        REQUIRE( vidurkis(empty) == Approx(0.0) );
    }
}

TEST_CASE("mediana: baziniai ir kraštiniai atvejai", "[math][median]") {
    SECTION("nelyginis kiekis") {
        std::vector<int> v{10, 8, 6, 4, 2};
        REQUIRE( mediana(v) == Approx(6.0) );
    }
    SECTION("lyginis kiekis") {
        std::vector<int> v{1, 2, 3, 4};
        REQUIRE( mediana(v) == Approx((2 + 3) / 2.0) );
    }
    SECTION("tuščias vektorius") {
        std::vector<int> empty;
        REQUIRE( mediana(empty) == Approx(0.0) );
    }
    SECTION("mediana_ref nenaikina originalo tvarkos") {
        std::vector<int> v{3,1,2};
        auto before = v;
        REQUIRE( mediana_ref(v) == Approx(2.0) );
        REQUIRE( v == before );
    }
}
// 7. Galutinio balo formulė
TEST_CASE("skaiciuotiGalutini: vidurkis ir mediana strategijos", "[student][final]") {
    Studentas s;
    s.setVardas("A");
    s.setPavarde("B");
    s.setNd({10, 8, 6, 4, 2});
    s.setEgzaminas(9);

    const double expected = 0.4*6.0 + 0.6*9.0;
    REQUIRE( s.skaiciuotiGalutini(vidurkis)    == Approx(expected) );
    REQUIRE( s.skaiciuotiGalutini(mediana_ref) == Approx(expected) );
}

// 8. Skirstymo strategijos
TEST_CASE("split strategijos – teisingas atskyrimas ties 5.0", "[split]") {
    std::vector<Studentas> base;
    {
        auto s1 = make_student("v1","p1", {5,5,5,5,5}, 4);
        auto s2 = make_student("v2","p2", {5,5,5,5,5}, 5);
        auto s3 = make_student("v3","p3", {9,9,9,9,9}, 9);
        base = {s1, s2, s3};
    }

    SECTION("strat2 vector – vargšiukai pašalinami iš all") {
        std::vector<Studentas> all = base;
        std::vector<Studentas> vargs;
        const auto n0 = all.size();

        split_strat2_vector(all, vargs);

        REQUIRE( std::all_of(all.begin(), all.end(),
                 [](const Studentas& s){ return s.galutinis() >= 5.0; }) );
        REQUIRE( std::all_of(vargs.begin(), vargs.end(),
                 [](const Studentas& s){ return s.galutinis() <  5.0; }) );
        REQUIRE( all.size() + vargs.size() == n0 );
    }

    SECTION("strat3 vector – remove_copy_if/remove_if") {
        std::vector<Studentas> all = base;
        std::vector<Studentas> vargs;
        const auto n0 = all.size();

        split_strat3_vector(all, vargs);

        REQUIRE( std::all_of(all.begin(), all.end(),
                 [](const Studentas& s){ return s.galutinis() >= 5.0; }) );
        REQUIRE( std::all_of(vargs.begin(), vargs.end(),
                 [](const Studentas& s){ return s.galutinis() <  5.0; }) );
        REQUIRE( all.size() + vargs.size() == n0 );
    }
}

#if defined(HAVE_PALIKTI_TIK_KIETUOLIUS) && HAVE_PALIKTI_TIK_KIETUOLIUS
std::size_t paliktiTikKietuolius(std::vector<Studentas>&);

TEST_CASE("paliktiTikKietuolius: vektoriuje lieka tik >=5.0", "[tdd]") {
    std::vector<Studentas> v = {
        make_student("a","a",{5,5,5,5,5},4),
        make_student("b","b",{5,5,5,5,5},5),
        make_student("c","c",{9,9,9,9,9},9)
    };

    size_t removed = paliktiTikKietuolius(v);

    REQUIRE( removed == 1 );
    REQUIRE( v.size() == 2 );
    REQUIRE( std::all_of(v.begin(), v.end(),
             [](const Studentas& s){ return s.galutinis() >= 5.0; }) );
    REQUIRE( v[0].vardas() == "b" );
    REQUIRE( v[1].vardas() == "c" );
}

TEST_CASE("paliktiTikKietuolius: tuščias vektorius", "[tdd]") {
    std::vector<Studentas> v;
    REQUIRE( paliktiTikKietuolius(v) == 0 );
    REQUIRE( v.empty() );
}

TEST_CASE("paliktiTikKietuolius: visi kietuoliai", "[tdd]") {
    std::vector<Studentas> v = {
        make_student("x","y",{10,10,10,10,10},7),
        make_student("m","n",{9,9,9,9,9},9)
    };
    REQUIRE( paliktiTikKietuolius(v) == 0 );
    REQUIRE( v.size() == 2 );
}
#endif


