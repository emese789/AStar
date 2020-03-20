# Feladat
Eljutni egy kezdő állapotból a végállapotba. A kezdő állapot lehet állományból megkapott vagy a standard bemenetről be olvasott kezdő érték, míg a végállapotnak tekinthető pl: n=3 esetében 0 1 2 3 4 5 6 7 8.
# Kritériumok
A programot a parancssor argumentumain keresztül tegye konfigurálhatóvá. Kapcsolók:
1. –input <FILE>: a kezdeti állapotot tartalmazó állomány neve. Ha a kapcsoló hiányzik, a standard bemenetr˝ol
olvassa be a kezdeti állapotot.
2. –solseq: a standard kimenetre írja a teljes megoldási szekvenciát
3. –pcost: a standard kimenetre írja a megoldás költségét
4. –nvisited: a standard kimenetre írja a meglátogatott csomópontok számát
5. –h <H>: a heurisztika típusa. Ha H=1, használja a „rossz helyen lev˝o csempék száma” heurisztikát. Ha
Ha H=2, használja a Manhattan heurisztikát.
6. –rand <N> <M> egy véletlenszer ˝u, N méret ˝u állapotot írjon ki a standard kimenetre. M a véletlenszer ˝u
tologatások számát jelenti.
