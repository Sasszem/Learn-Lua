# Learn-Lua
GTK+ alapú interaktív Lua Oktatóprogram C-ben

### Bevezetés
A program a Lua nyelv alapjait tanítja meg a felhasználónak feladatokon keresztül.
Minden feladat egy újabb programelem használatát és működését tanítja meg, például a változókat vagy az eljárásokat. 

### Működés
A felhasználónak újonnan tanult elem után írnia kell egy programot, ami az adott elemre épül. A felhasználó programját egy automate tesztelőprogram különböző bemenetekkel futtatja, és ha sikeresen megoldotta a feladatok, tovbblépteti a következő feladatra.

### Felület
A program a GTK+ 3.0 könyvtárra épülő, könnyen kezelhető felületet biztosít. 
![felület](pic/interface.png?raw=true "Felület")

A felület három lapra van osztva:
 - Utasítások: Ezen az oldalon látja a felhasználó az adott feladat utasításait, itt olvashatja el az éppen tanult programelem(pl. eljárások, változók) bemutatását.
![utasítások](pic/task.png?raw=true "Feladat")
 - Programkód: Ezen az oldalon írhatja a programozó a programját. A kulcsszavakat a program automatikusan színesen kiemeli.
![Szerkesztő](pic/code.png?raw=true "Szerkesztő")
 - Feladatok: Itt a felhasználó egy fanézetben választhat az elérhető témakörök és feladatok között.
![fanézet](pic/tree.png?raw=true "Feladatok")


 A lapok közötti goyrs váltásra szolgálnak az **F1-F3** gyorsbillentyűk
 
 #### Menüsáv
 A program menüsávjában kapott helyet a mentés és a futtatás gomb, melyekkel elmenthetjük vagy futtathatjuk az aktuális programunkat. Ezen funkciókat a **Ctrl-S** és a **Ctrl-R** gyorsbillentyűkkel is elérhetjük.
 
### Mentés
A program feladatváltáskor és kilépéskor automatikusan elmenti az aktuális programkódot, feladatkiválasztáskor pedig betölti a mentett programot. A program emellett elmenti az is, hogy mely feladatokat oldottunk meg.
A mentés helye a `/src/data.h` file-ban van meghatározva

### Lua környezet
A Lua környezet a Lua 5.2-es verziójára épül.
A környetet részben módosítva van, több modul le van tiltva, és néhány eljárás felül van írva, például a `print`, amely felugró ablakot jelenít meg a parancssorba való írás helyett.

A felhasználó a programjának bemeneteit elnevezve, változókban kapja meg pl. `ELETKOR`, `BEMENET`, stb. A programjának kimenetét a `return` kulcsszóval adja meg, pl. `return 42`

#### Az értékelő
A felhasználó programját is egy Lua szkript teszteli, melyet feladatonként adhatunk meg a feladatleírás melett.
Az értékelőprogram ugyanazon Lua környezetben fut, mint a felhasználó programja.
Az értékelő többször is futtathatja a felhasználó programját, többféle bemenettel. A felhasználó programjának bemeneteket változókban adhat át, a kimeneteit pedig a `run()` eljárás kimeneteiként kapja meg.

Az értékelő szintén a `return` utasítással adja vissza kimenetét, amely logikai(boolean) érték, helyes felhasználói program esetén `true`

### Adatbetöltés
A program az adatokat dinamikusan tölti be, az adatok helye a `/src/data.h` file-ban van meghatározva. A betöltendő adatok témakörökre, azon belül pedig feladatokra vannak osztva. A betöltés gyökérköyvtárában és az egyes témakörök könytárában is szerepel egy listafájl, ami soronként tartalmazza a témakörök nevét, illetve az adott témakör feladatait, melyeket az azonos nevű mappában keres a program. Minen feladat mappájában két file kell hogy legyen, az `instructions.txt` és a `tester.lua`. Előbbi tartalmazza az adott feladat leírását, utasításait, utóbbi pedig a tesztelőprogram.
A leírás __pango makrup__ elemeket tartalmazhat.

### Fejlesztési állapot
A program ("keretrendszer") elkészült, nagyobb módosítás már nem várható.
Az oktatófeladatok közül még csak néhány van megírva, ezeken folyamatosan dolgozom.

A programot **Ubuntu 16.04** rendszeren fejlesztem
Futtatás előtt szükséges a `liblua5.2-dev` és a `libgtk-3-0` csomagok telepítése:
`sudo apt install libgtk+-3.0 liblua5.2`

Ha ezek telepítve vannak, a programot fordíthatjuk és futtathatjuk:
`make && ./main`




