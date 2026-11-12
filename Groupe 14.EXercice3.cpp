#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ==========================================
// 1. Classe Mère Abstraite : Employe
// ==========================================
class Employe {
protected:
    string matricule;
    string nom;
    string prenom;
    double salaireBase;

public:
    Employe(string mat, string n, string p, double sb)
        : matricule(mat), nom(n), prenom(p), salaireBase(sb) {}

    // Destructeur virtuel obligatoire pour la libération mémoire via la liaison dynamique
    virtual ~Employe() {}

    // Méthode virtuelle pure : rend la classe abstraite
    virtual double calculerSalaire() const = 0;

    // Méthode virtuelle d'affichage (redéfinie dans les classes filles)
    virtual void afficher() const {
        cout << "Matricule: " << matricule 
             << " | Nom: " << prenom << " " << nom 
             << " | Salaire de base: " << fixed << setprecision(2) << salaireBase << " €";
    }

    string getMatricule() const { return matricule; }
};

// ==========================================
// 2. Classe Dérivée : Enseignant
// ==========================================
class Enseignant : public Employe {
private:
    double indemnitesSpecifiques;

public:
    Enseignant(string mat, string n, string p, double sb, double indemnites)
        : Employe(mat, n, p, sb), indemnitesSpecifiques(indemnites) {}

    double calculerSalaire() const override {
        return salaireBase + indemnitesSpecifiques;
    }

    void afficher() const override {
        cout << "[Enseignant]    ";
        Employe::afficher();
        cout << " | Indemnités: " << indemnitesSpecifiques << " €"
             << " | Salaire Total: " << calculerSalaire() << " €" << endl;
    }
};

// ==========================================
// 3. Classe Dérivée : Agent Administratif
// ==========================================
class Administratif : public Employe {
private:
    double primeAdministrative;

public:
    Administratif(string mat, string n, string p, double sb, double prime)
        : Employe(mat, n, p, sb), primeAdministrative(prime) {}

    double calculerSalaire() const override {
        return salaireBase + primeAdministrative;
    }

    void afficher() const override {
        cout << "[Administratif] ";
        Employe::afficher();
        cout << " | Prime Admin: " << primeAdministrative << " €"
             << " | Salaire Total: " << calculerSalaire() << " €" << endl;
    }
};

// ==========================================
// 4. Classe Dérivée : Technicien
// ==========================================
class Technicien : public Employe {
private:
    double primeTechnicite;

public:
    Technicien(string mat, string n, string p, double sb, double prime)
        : Employe(mat, n, p, sb), primeTechnicite(prime) {}

    double calculerSalaire() const override {
        return salaireBase + primeTechnicite;
    }

    void afficher() const override {
        cout << "[Technicien]    ";
        Employe::afficher();
        cout << " | Prime Tech:  " << primeTechnicite << " €"
             << " | Salaire Total: " << calculerSalaire() << " €" << endl;
    }
};

// ==========================================
// 5. Programme Principal (Test de la collection & polymorphisme)
// ==========================================
int main() {
    // Collection hétérogène stockant des pointeurs sur la classe de base (Employe*)
    vector<Employe*> personnel;

    // Ajout de différentes catégories d'employés dans la même collection
    personnel.push_back(new Enseignant("E001", "Durand", "Pierre", 2200.0, 600.0));
    personnel.push_back(new Administratif("A001", "Bernard", "Sophie", 1800.0, 300.0));
    personnel.push_back(new Technicien("T001", "Leroy", "Marc", 1700.0, 350.0));
    personnel.push_back(new Enseignant("E002", "Moreau", "Claire", 2400.0, 750.0));

    cout << "==========================================================" << endl;
    cout << "           GESTION DU PERSONNEL UNIVERSITAIRE             " << endl;
    cout << "==========================================================" << endl;

    // Démonstration du polymorphisme et de la liaison dynamique :
    // Chaque appel à afficher() déclenche la méthode propre à la classe réelle de l'objet.
    for (const auto& emp : personnel) {
        emp->afficher();
    }

    // Libération propre de la mémoire allouée dynamiquement
    for (auto& emp : personnel) {
        delete emp;
    }
    personnel.clear();

    return 0;
}
