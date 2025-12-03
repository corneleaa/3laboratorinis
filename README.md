# Studentai v2.0

Studentų duomenų apdorojimo, analizės ir rikiavimo sistema, realizuota naudojant C++17, objektinį programavimą, STL konteinerius, strateginius algoritmus, vienetinius testus ir Doxygen dokumentaciją.

## Projekto paskirtis

Programa skirta efektyviam studentų duomenų apdorojimui. Ji leidžia:

- nuskaityti arba sugeneruoti studentų duomenų failus;
- apskaičiuoti galutinį balą pagal vidurkį, medianą arba abu būdus;
- rikiuoti studentus pagal vardą, pavardę arba galutinį balą;
- skirstyti studentus į dvi grupes pagal galutinio balo ribą;
- pasirinkti skirtingus konteinerius (`std::vector` arba `std::list`);
- vykdyti tris skirtingas duomenų skirstymo strategijas;
- atlikti veikimo laiko matavimus;
- generuoti išvesties failus ir Doxygen dokumentaciją;
- vykdyti vienetinius testus su Catch2.

## Naudotos technologijos

- C++17  
- STL konteineriai: `vector`, `list`  
- Paveldėjimas: `Zmogus → Studentas`  
- Strateginiai algoritmai (3 skirstymo strategijos)  
- CMake  
- Catch2 v3 (vienetiniams testams)  
- Doxygen (automatinė dokumentacija)

## Projekto struktūra
projektas8/
│ CMakeLists.txt
│ main.cpp
│ studentas.cpp
│ studentas.h
│ zmogus.h
│ test_studentas.cpp
│ Doxyfile
│
├── results/ Programos sugeneruoti išvesties failai
├── docs/ Doxygen dokumentacija
│ └── html/index.html
└── build/ Kompiliuoti failai (generuojama automatiškai)

## Projekto kompiliavimas ir paleidimas

### 1. Kompiliavimas

Atsidaryti projekto katalogą:

```bash
cd ~/Desktop/projektas8/projektas8
Pašalinti seną build katalogą:

rm -rf build


Sukompiliuoti projektą:

cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_PREFIX_PATH=/opt/homebrew
cmake --build build -j
2. Programos paleidimas
./build/student_app

Vienetiniai testai:

Projektas turi Catch2 pagrįstus testus.
Jie kompiliuojami kartu su projektu ir gali būti paleidžiami naudojant CTest.

Testų paleidimas:
ctest --test-dir build --output-on-failure

Doxygen dokumentacija:
Dokumentacijos generavimas
doxygen Doxyfile

Dokumentacijos peržiūra
open docs/html/index.html

Programos funkcionalumas
Įvedimo režimai:
Naujo failo generavimas
Egzistuojančio failo naudojimas
Duomenų įvedimas ranka

Galutinio balo skaičiavimo metodai:
pagal vidurkį
pagal medianą
abu metodai vienu metu

Konteinerių pasirinkimas:
std::vector
std::list

Rikiavimo kriterijai:
vardas
pavardė
galutinis balas

Skirstymo strategijos:
A + B: sukurti du atskirus konteinerius (vargšiukai + kietiakai)
Tik vargšiukai: ištrinti iš bendro konteinerio
Optimizuota strategija: remove_if pagrindu (greičiausia)

**Veikimo laiko pavyzdžiai**

Testuojant su 1 000 000 įrašų:
read   = 10.000910 s
sort   = 5.249577 s
split  = 0.742097 s
write  = 0.551951 s
total  = 17.398502 s

Klasės
Zmogus
Atributai:
vardas
pavarde
Funkcionalumas:
getteriai ir setteriai
spausdinimo funkcija
Studentas (paveldi Zmogus)
Atributai:
namų darbų rezultatai
egzamino rezultatas
galutinis balas

Funkcionalumas:
galutinio balo skaičiavimas (strategija perduodama funkcijos pointeriu)
rikiavimo ir skirstymo metodai
„Rule of Three“ realizacija


