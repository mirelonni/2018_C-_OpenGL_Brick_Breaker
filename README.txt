Cojocaru Marin-Mircea 335CC
========================================================
Am incercat sa apropii cat mai mult de exemplul dat.
Pentru a facae acest lucru:
- coliziunile sunt implementate cu 2 frame-uri daca la frame-ul actual este in afara obiectului si la urmatorul frame(presupus) este in interior se considera coliziune.
- miscarea platformei si lansarea bilei se face cu mouseul
- trei powerup-uri explicate mai jos

Ce nu se aseamana cu exemplul dat este:
- peretii laterali se intind pana la nivelul platformei
- spatiul dintre caramizi-platforma si caramizi marginea de sus este egal

Alte precizari:
- am incercat sa pastrez dimensiunile cat de cat proportionale:
	- exista o singura variabila pentru dimensiunea marginilor
	- inaltimea platformei este jumatate din latimea marginilor
	- powerup-ul 1 este la inaltimea platformei si are aceasi inaltime cu aceasta

Powerup 1 (albastru):
Acesta adauga o margine inferioara care nu ii permite bilei sa iasa din spatiul de joc. Aceasta margine are un timp pentru care ramane pe ecran, timp care este aratat de grosimea marginii(direct proportionala cu timpul ramas)

Powerup 2 (roz):
Acesta ii mai da o viata jucatorului, oprindu-se la numarul maxim de vieti.

Powerup 3 (galben):
Acesta face bila invincibila(trece prin caramizi), avand un timp care functioneaza asemanator primului, diferenta e ca bila se face galbena cand este invincibila.