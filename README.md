# Karetní RPG
Autor: Daniel Zábojník

## Zadání

Naprogramujte engine pro hraní karetního RPG.

Váš engine:

. ze souboru nahraje definici karet (jejich typ, atributy, efekty), balíčku (seznam karet v balíčku)
. umožní vytvořit a hrát samotnou karetní hru
. implementuje variantu pro jednoho hráče proti počítači (náhodné tahy nestačí)
. implementuje variantu pro dva hráče (výměna přes potvrzovací okno, aby soupeř neviděl karty, nebo naopak soupeři si celou dobu do karet vidí)
. umožňuje ukládat a načítat rozehrané hry

Kde lze využít polymorfismus? (doporučené)

- Typy karet: útočné, obranné, speciální, ...
- Efekty akcí: síla útoku, síla obrany, vem si X karet, zahoď X karet
- Ovládání hráče: lokální hráč, umělá inteligence (různé druhy), síťový hráč
- Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ...

## Specifikace

V rámci své semestrální práce budu implementovat jednoduché karetní RPG. Na začátku hry je každému z hráčů nastavena počáteční hodnota zdraví na 15, štítu na 5 a many na 10. Dále je náhodným losem určen začínající hráč. Druhý hráč dostane jako kompenzaci 3 many navíc. V průběhu hry se hráči střídají po jednotlivých tazích. V rámci jednoho tahu může hráč použít pouze jednu kartu, pokud však nemá dostatek many, bude přeskočen a na tah se dostává protihráč. Po každém ukončeném tahu se danému hráči přičtou 2 many a také si lízne další kartu z balíčku. V ruce však vždy drží maximálně pět karet. Cílem hry je za pomoci vhodné strategie (hráči si po celou dobu hry uvidí navzájem do karet) a jednotlivých karet z balíčku zničit nepřítele.

Ve zmíněném balíčku nalezneme vždy minimálně deset karet. Každá karta může mít až tři ze čtyř dostupných efektů. Jednotlivé efekty mohou být útočné, obranné, uzdravující či magické. Útočný efekt zpravidla ubírá štít a/nebo zdraví nepříteli v závislosti na jeho aktuálním stavu. Obranný efekt přidává štít a uzdravující naopak zdraví hráči, který kartu s daným efektem použije. Velmi zajímavý je ale i magický efekt, který přidá hráči manu dle dvé definice a může tak zásadně ovlivnit průběh hry.

```
┌────────────────────────────────────────────────────────────────────────────────┐
    TOMAS: 34 HP | 20 Shield | 4 Mana
    Playing cards:                                            ________________
          1 Bandage (Behavior: 5 HEAL | Cost: 2 MANA)        | ATTACK        |
          2 Fireball (Behavior: 10 ATTACK  | Cost: 3 MANA)   | Fireball      |
          3 Shotgun (Behavior: 3 ATTACK | Cost: 1 MANA)      | Behavior:     |
          4 Shield (Behavior: 8 DEFENSE | Cost: 4 MANA)      |   - 10 HP     |
          5 Mana unleash (Behavior: 3 MANA | Cost: 1 MANA)   |   - 5 SHIELD  |
                                                             |               |
                                                             |               |
                                                             |               |
                                                             |               |
                                                             |_______________|



    DAVID: 40 HP | 12 Shield | 7 Mana
    Playing cards:                                            ________________
          1 Shield shot (Behavior: 4 DEFENSE | Cost: 1 MANA) | MAGIC         |
          2 Mana (Behavior: 4 MANA | Cost: 1 MANA)           | Mana unleash  |
          3 Enrage (Behavior: 20 ATTACK | Cost: 10 MANA)     | Behavior:     |
          4 Bandage (Behavior: 7 HEAL | Cost: 3 MANA)        |   - 3 MANA    |
                                                             |               |
                                                             |               |
                                                             |               |
                                                             |               |
                                                             |               |
                                                             |_______________|
└────────────────────────────────────────────────────────────────────────────────┘
David is on the move! Select the game card you want to use:
$ 2
```
Na hrací ploše nalezneme hodnoty zdraví, štítu a many obou hráčů. Dále jsou pod stavovým řádkem hráče vypsány jednotlivé karty, které hráč drží ve své ruce a na pravé straně je pak zobrazena karta, kterou hráč použil v předchozím tahu.

## Polymorfismus

Polymorfismus je aplikovaný v rámci hiearchie tříd `CEffect`, která má čtyři potomky `CEffectAttack`, `CEffectDefend`, `CEffectHeal` a `CEffectMana`. Třída `CEffect` má polymorfní metodu `To_String` pro výpis efektu, ale hlavně také mnohem důležitější metodu `ApplyEffect`, která v závislosti na typu efektu aplikuje daný efekt buďto na protihráče nebo hráče, který kartu použil.

K polymorfnímu volání dochází v metodě CCard::UseCard, kde se prochází jednotlivé efekty karty a každý z efektů se aplikuje buďto na protihráče nebo hráče, který kartu použil, a to právě v závislosti na vlastnostech daného efektu.

NOTE: Vedlejší polymorfismus budu mít nad třídou `CPlayer`, který má dva potomky `CPlayerHuman` a `CPlayerRobot`. Třída `CPlayer` má polymorfní metodu `Move`, která si vždy vyžádá od konkrétní implementace hráče další tah. Implementace pro skutečného hráče očekává, že se prostřednictvím konzole dotáži na požadovaný tah a ten následně vykonám. Implementace pro robota se naopak pokusí vypočítat ideální tah na základě karet, které má k dispozici a také karet protihráče.

## Možná rozšíření
Jedním z rozšíření, které bych rád realizoval je uživatelsky přívětivější rozhraní viz ukázka níže.

```
----------------------------------------------------------------------------------------------
Daniel: 21 Health | 5 Shield | 10 Mana
----------------------------------------------------------------------------------------------

 ______________     ______________     ______________     ______________     ______________
| Enrage      |    | Hunter      |    | Starfire    |    | Bandage     |    | Sentry      |
| Cost: 11    |    | Cost: 3     |    | Cost: 6     |    | Cost: 2     |    | Cost: 3     |
|             |    |             |    |             |    |             |    |             |
| Attack: 9   |    | Attack: 5   |    | Attack: 6   |    | Defense: 3  |    | Defense: 5  |
| Defense: 2  |    |             |    | Defense: 2  |    | Heal: 1     |    |             |
|             |    |             |    |             |    |             |    |             |
|             |    |             |    |             |    |             |    |             |
|_____________|    |_____________|    |_____________|    |_____________|    |_____________|



                                       ______________
                                      | Slayer      |
                                      | Cost: 1     |
                                      |             |
                                      | Attack: 2   |
                                      |             |
                                      |             |
                                      |             |
                                      |_____________|




 ______________     ______________     ______________     ______________     ______________
| Protector   |    | Commander   |    | Rhino       |    | Sun Cleric  |    | Multi-shot  |
| Cost: 3     |    | Cost: 5     |    | Cost: 5     |    | Cost: 3     |    | Cost: 4     |
|             |    |             |    |             |    |             |    |             |
| Defense: 5  |    | Heal: 2     |    | Attack: 1   |    | Defense: 3  |    | Attack: 4   |
| Heal: 2     |    | Defense: 1  |    | Defense: 4  |    | Heal: 2     |    | Defense: 2  |
|             |    | Mana: 10    |    |             |    |             |    |             |
|             |    |             |    |             |    |             |    |             |
|_____________|    |_____________|    |_____________|    |_____________|    |_____________|


----------------------------------------------------------------------------------------------
Saltibart Fast: 15 Health | 5 Shield | 14 Mana
----------------------------------------------------------------------------------------------
Daniel is now on the move.
Select the card (1-X) you want to play with:
$ 2
```
Podobně jako v původním návrhu na hrací ploše nalezneme hodnoty zdraví, štítu a many obou hráčů. Namísto pouhého seznamu karet jsou však nově vykresleny jednotlivé karty, které každý z hráčů drží ve své ruce a uprostřed je pak zobrazena karta, která byla použita v předchozím tahu.
