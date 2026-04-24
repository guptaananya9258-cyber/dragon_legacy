#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// ----------------------------
// Ability (Composition)
// ----------------------------
class Ability {
private:
    string name;
    int damage;
    int cooldown;
    int currentCooldown;

public:
    Ability(string n, int d, int cd)
        : name(n), damage(d), cooldown(cd), currentCooldown(0) {}

    bool canUse() { return currentCooldown == 0; }

    void use() {
        cout << "[Ability] " << name << " used! Damage: " << damage << endl;
        currentCooldown = cooldown;
    }

    void reduceCooldown() {
        if (currentCooldown > 0) currentCooldown--;
    }

    int getDamage() { return damage; }
};

// ----------------------------
// Abstract Dragon Class
// ----------------------------
class Dragon {
protected:
    string name;
    int health, maxHealth;
    int attackPower;
    Ability ability;

public:
    Dragon(string n, int hp, int atk, Ability ab)
        : name(n), health(hp), maxHealth(hp), attackPower(atk), ability(ab) {}

    virtual void attack() = 0;

    void normalAttack(Dragon* enemy) {
        cout << "[Polymorphism] Normal attack called\n";
        cout << name << " attacks for " << attackPower << endl;
        enemy->takeDamage(attackPower);
    }

    void specialAttack(Dragon* enemy) {
        if (ability.canUse()) {
            ability.use();
            enemy->takeDamage(ability.getDamage());
        } else {
            cout << "Ability on cooldown!\n";
        }
    }

    void reduceCooldown() { ability.reduceCooldown(); }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    bool isAlive() { return health > 0; }

    void showHP() {
        cout << name << " HP: [";
        int bars = health * 20 / maxHealth;
        for (int i = 0; i < bars; i++) cout << "#";
        for (int i = bars; i < 20; i++) cout << "-";
        cout << "] " << health << "/" << maxHealth << endl;
    }

    string getName() { return name; }

    virtual ~Dragon() {}
};

// ----------------------------
// Dragon Types
// ----------------------------
class FireDragon : public Dragon {
public:
    FireDragon(string n)
        : Dragon(n, 300, 80, Ability("Inferno Blast", 120, 2)) {}

    void attack() override {
        cout << "[FireDragon] Flames unleashed!\n";
    }
};

class IceDragon : public Dragon {
public:
    IceDragon(string n)
        : Dragon(n, 350, 70, Ability("Frozen Spike", 100, 2)) {}

    void attack() override {
        cout << "[IceDragon] Ice attack!\n";
    }
};

class ForestDragon : public Dragon {
public:
    ForestDragon(string n)
        : Dragon(n, 400, 60, Ability("Poison Vines", 90, 1)) {}

    void attack() override {
        cout << "[ForestDragon] Nature strikes!\n";
    }
};

// ----------------------------
// Save System
// ----------------------------
class SaveSystem {
public:
    static void save(int level, int gold) {
        ofstream file("save.txt");
        file << level << " " << gold;
        file.close();
        cout << "[SaveSystem] Game Saved!\n";
    }
};

// ----------------------------
// Rider
// ----------------------------
class Rider {
private:
    string name;
    Dragon* dragon;
    int level;
    int gold;
    bool exitedToMenu;   //  important

public:
    Rider(string n, Dragon* d)
        : name(n), dragon(d), level(1), gold(0), exitedToMenu(false) {
        cout << "[Encapsulation] Rider " << name << " created\n";
    }

    void battle(Dragon* enemy) {
        exitedToMenu = false;

        cout << "\n=== QUEST BATTLE START ===\n";

        while (dragon->isAlive() && enemy->isAlive()) {
            dragon->showHP();
            enemy->showHP();

            cout << "\n1. Normal Attack\n2. Special Attack\n3. Return to Main Menu\nChoice: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                dragon->normalAttack(enemy);
            }
            else if (choice == 2) {
                dragon->specialAttack(enemy);
            }
            else if (choice == 3) {
                cout << "\n[System] Returning to Main Menu...\n";
                exitedToMenu = true;
                return;
            }
            else {
                cout << "Invalid choice!\n";
                continue;
            }

            if (enemy->isAlive()) {
                enemy->normalAttack(dragon);
            }

            dragon->reduceCooldown();
            enemy->reduceCooldown();
        }

        if (dragon->isAlive()) {
            cout << "You WON!\n";
            level++;
            gold += 100;
            SaveSystem::save(level, gold);
        } else {
            cout << "You LOST!\n";
        }
    }

    bool didExit() { return exitedToMenu; }
};

// ----------------------------
// MAIN GAME LOOP
// ----------------------------
int main() {

    while (true) {
        cout << "\n====== DRAGON LEGACY ======\n";
        cout << "1. Start Game\n2. Exit\nChoice: ";

        int menu;
        cin >> menu;

        if (menu == 2) break;

        // Rider name input
        string riderName;
        cout << "Enter Rider Name: ";
        cin >> riderName;

        // Dragon selection
        cout << "\nChoose your Dragon:\n1. Fire\n2. Ice\n3. Forest\nChoice: ";
        int choice;
        cin >> choice;

        Dragon* player;

        if (choice == 1)
            player = new FireDragon("Ember");
        else if (choice == 2)
            player = new IceDragon("Frost");
        else
            player = new ForestDragon("Verdant");

        Rider rider(riderName, player);

        string quests[4] = {
            "Siege of Ashveil",
            "Frozen Citadel",
            "Poison Marsh",
            "Dragon's Lair"
        };

        for (int i = 0; i < 4; i++) {
            cout << "\n=== QUEST: " << quests[i] << " ===\n";

            Dragon* enemy = new FireDragon("Enemy");

            rider.battle(enemy);

            if (rider.didExit()) {
                delete enemy;
                break;
            }

            if (!player->isAlive()) {
                delete enemy;
                break;
            }

            delete enemy;
        }

        delete player;
    }

    cout << "\nGame Exited.\n";
    return 0;
}