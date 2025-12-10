**Studentų vertinimo sistema (v2.0)**

Šis projektas yra C++ programa, kuri:
skaito arba generuoja studentų duomenis,
apskaičiuoja galutinį balą (pagal vidurkį arba medianą),
rikiuoja studentus pagal pasirinktą kriterijų,
išskiria kietuolius ir vargšiukus,
matuoja skirtingų konteinerių (vector ir list) našumą,
turi pilnai veikiančius Unit testus (Catch2),
turi parengtą Doxygen HTML dokumentaciją.

Projektas demonstruoja:
objektinio programavimo principus (Rule of Three),
skaidymo strategijas (A, B, optimizuota),
CMake naudojimą,
testais grįstą kūrimą (TDD),
tvarkingą failų struktūrą.

1. Projekto struktūra
projektas8/
├── CMakeLists.txt
├── Doxyfile
├── README.md
│
├── studentas.h
├── studentas.cpp
├── zmogus.h
├── main.cpp
│
├── tests/
│   └── test_studentas.cpp
│
├── docs/
│   └── html/         ← Doxygen dokumentacija
│
└── results/          ← sugeneruojami rezultatai

2. Projekto įdiegimas ir kompiliavimas

Reikalinga:
C++17 arba naujesnis
CMake 3.16 arba naujesnis
Catch2 (jei naudojama Homebrew: brew install catch2)

Kompiliacija:
cd projektas8
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

3. Programos paleidimas
./build/student_app

Paleidus programą pateikiamas meniu:
Generuoti naują failą
Naudoti esamą failą
Įvesti duomenis ranka

Vartotojas taip pat pasirenka:

galutinio balo formulę (vidurkis / mediana),
konteinerį (vector / list),
rikiavimo kriterijų (vardas / pavardė / galutinis),
skaidymo strategiją (A, B, optimizuota).

Sugeneruoti rezultatai
results/vargsiukai.txt
results/kietuoliai.txt

4. Unit testai (Catch2)

Projektas turi pilną test_studentas.cpp testų rinkinį.
Testų paleidimas:
ctest --test-dir build --output-on-failure

Arba:
./build/tests

<img width="743" height="503" alt="Screenshot 2025-12-10 at 13 51 33" src="https://github.com/user-attachments/assets/0bf85dd3-3ba0-4a79-ac8f-00db01dc57ec" />

5. Dokumentacija (Doxygen)
Doxygen HTML dokumentacija yra:
docs/html/index.html

Kaip sugeneruoti iš naujo:
doxygen Doxyfile

Atidaryti dokumentaciją macOS:
open docs/html/index.html



<img width="1428" height="895" alt="Screenshot 2025-12-10 at 14 12 19" src="https://github.com/user-attachments/assets/5ff137ee-f014-45e6-b2a6-a96100d0f366" />



6. Našumo matavimai
Programa pateikia laikus:
failo nuskaitymo,
rikiavimo,
skaidymo,
rezultatų rašymo,
bendrą programos laiką.
7. Release'ai
v2.0
Pridėti Unit testai
Įkelta Doxygen dokumentacija
Įgyvendintos optimizuotos split strategijos
Rule of Three diagnostika
Veikiantys našumo matavimai
