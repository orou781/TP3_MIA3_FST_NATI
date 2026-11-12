#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Énumérations pour les statuts
enum StatutChambre { LIBRE, OCCUPEE, EN_MAINTENANCE };
enum StatutDemande { EN_ATTENTE, VALIDEE, REJETEE, ANNULEE };
enum EtatAttribution { ACTIVE, CLOTUREE };

// Forward declarations
class Chambre;
class Etudiant;
class DemandeHebergement;
class Attribution;

// Classe Etudiant
class Etudiant {
private:
    string numEtudiant;
    string nom;
    string prenom;
    string email;
    string telephone;

public:
    Etudiant(string num, string n, string p, string mail, string tel)
        : numEtudiant(num), nom(n), prenom(p), email(mail), telephone(tel) {}

    string getNumEtudiant() const { return numEtudiant; }
    void afficher() const {
        cout << "Étudiant [" << numEtudiant << "] : " << prenom << " " << nom << " (" << email << ")" << endl;
    }
};

// Classe Chambre
class Chambre {
private:
    string numChambre;
    int capacite;
    StatutChambre statut;
    double tarifMensuel;

public:
    Chambre(string num, int cap, double tarif)
        : numChambre(num), capacite(cap), statut(LIBRE), tarifMensuel(tarif) {}

    string getNumChambre() const { return numChambre; }
    StatutChambre getStatut() const { return statut; }
    
    void setStatut(StatutChambre s) { statut = s; }

    bool estDisponible() const {
        return statut == LIBRE;
    }

    void afficher() const {
        cout << "Chambre " << numChambre << " - Tarif: " << tarifMensuel << "€ - Statut: ";
        if (statut == LIBRE) cout << "Libre";
        else if (statut == OCCUPEE) cout << "Occupée";
        else cout << "En maintenance";
        cout << endl;
    }
};

// Classe DemandeHebergement
class DemandeHebergement {
private:
    string idDemande;
    string dateDemande;
    string dateDebutSouhaitee;
    StatutDemande statutDemande;
    Etudiant* etudiant;

public:
    DemandeHebergement(string id, string dateD, string dateSouh, Etudiant* etu)
        : idDemande(id), dateDemande(dateD), dateDebutSouhaitee(dateSouh), statutDemande(EN_ATTENTE), etudiant(etu) {}

    string getIdDemande() const { return idDemande; }
    StatutDemande getStatutDemande() const { return statutDemande; }
    Etudiant* getEtudiant() const { return etudiant; }

    void valider() { statutDemande = VALIDEE; }
    void rejeter() { statutDemande = REJETEE; }
    void annuler() { statutDemande = ANNULEE; }

    void afficher() const {
        cout << "Demande " << idDemande << " (Étudiant: " << etudiant->getNumEtudiant() << ") - Statut: ";
        switch(statutDemande) {
            case EN_ATTENTE: cout << "En attente"; break;
            case VALIDEE: cout << "Validée"; break;
            case REJETEE: cout << "Rejetée"; break;
            case ANNULEE: cout << "Annulée"; break;
        }
        cout << endl;
    }
};

// Classe Attribution
class Attribution {
private:
    string idAttribution;
    string dateDebut;
    string dateFinEffective;
    EtatAttribution etat;
    Etudiant* etudiant;
    Chambre* chambre;

public:
    Attribution(string id, string debut, Etudiant* etu, Chambre* cham)
        : idAttribution(id), dateDebut(debut), dateFinEffective(""), etat(ACTIVE), etudiant(etu), chambre(cham) {
        chambre->setStatut(OCCUPEE);
    }

    string getIdAttribution() const { return idAttribution; }
    Chambre* getChambre() const { return chambre; }
    EtatAttribution getEtat() const { return etat; }

    void cloturer(string dateFin) {
        dateFinEffective = dateFin;
        etat = CLOTUREE;
        chambre->setStatut(LIBRE);
    }

    void afficher() const {
        cout << "Attribution " << idAttribution << " | Étudiant: " << etudiant->getNumEtudiant() 
             << " | Chambre: " << chambre->getNumChambre() << " | État: " 
             << (etat == ACTIVE ? "Active" : "Clôturée") << endl;
    }
};

// Classe Residence
class Residence {
private:
    string idResidence;
    string nomResidence;
    string adresse;
    vector<Chambre> chambres;
    vector<Etudiant> etudiants;
    vector<DemandeHebergement> demandes;

    vector<Attribution> listeAttributions;

public:
    Residence(string id, string nom, string adr)
        : idResidence(id), nomResidence(nom), adresse(adr) {}

    void ajouterChambre(const Chambre& c) { chambres.push_back(c); }
    void ajouterEtudiant(const Etudiant& e) { etudiants.push_back(e); }
    void enregistrerDemande(const DemandeHebergement& d) { demandes.push_back(d); }

    bool attribuerChambre(string idDemande, string numChambre, string dateDebut) {
        DemandeHebergement* demande = nullptr;
        for (auto& d : demandes) {
            if (d.getIdDemande() == idDemande && d.getStatutDemande() == EN_ATTENTE) {
                demande = &d;
                break;
            }
        }

        Chambre* chambre = nullptr;
        for (auto& c : chambres) {
            if (c.getNumChambre() == numChambre && c.estDisponible()) {
                chambre = &c;
                break;
            }
        }

        if (demande != nullptr && chambre != nullptr) {
            demande->valider();
            string idAttr = "ATT_" + idDemande;
            listeAttributions.push_back(Attribution(idAttr, dateDebut, demande->getEtudiant(), chambre));
            cout << "Succès : Chambre " << numChambre << " attribuée à l'étudiant." << endl;
            return true;
        }

        cout << "Erreur : Attribution impossible (demande introuvable ou chambre non disponible)." << endl;
        return false;
    }

    void enregistrerDepart(string numChambre, string dateFin) {
        for (auto& attr : listeAttributions) {
            if (attr.getChambre()->getNumChambre() == numChambre && attr.getEtat() == ACTIVE) {
                attr.cloturer(dateFin);
                cout << "Départ enregistré pour la chambre " << numChambre << "." << endl;
                return;
            }
        }
        cout << "Erreur : Aucune attribution active trouvée pour cette chambre." << endl;
    }

    void consulterChambresDisponibles() const {
        cout << "\n--- Chambres Libres ---" << endl;
        for (const auto& c : chambres) {
            if (c.estDisponible()) {
                c.afficher();
            }
        }
    }

    void consulterChambresOccupees() const {
        cout << "\n--- Chambres Occupées ---" << endl;
        for (const auto& c : chambres) {
            if (c.getStatut() == OCCUPEE) {
                c.afficher();
            }
        }
    }

    void consulterHistorique() const {
        cout << "\n--- Historique des Occupations ---" << endl;
        for (const auto& attr : listeAttributions) {
            attr.afficher();
        }
    }

    // Extension : Calcul du taux d'occupation
    double calculerTauxOccupation() const {
        if (chambres.empty()) return 0.0;
        int nbOccupees = 0;
        for (const auto& c : chambres) {
            if (c.getStatut() == OCCUPEE) {
                nbOccupees++;
            }
        }
        return (static_cast<double>(nbOccupees) / chambres.size()) * 100.0;
    }

    void afficherStatistiques() const {
        cout << "\n=== STATISTIQUES DE LA RÉSIDENCE ===" << endl;
        cout << "Nombre total de chambres : " << chambres.size() << endl;
        cout << "Taux d'occupation : " << calculerTauxOccupation() << "%" << endl;
        cout << "Demandes en attente : " << count_if(demandes.begin(), demandes.end(), [](const DemandeHebergement& d){
            return d.getStatutDemande() == EN_ATTENTE;
        }) << endl;
    }
};

// Fonction principale de test
int main() {
    // Initialisation de la résidence
    Residence residence("RES_01", "Résidence Universitaire Campus", "12 Rue de l'Université");

    // Ajout de chambres
    residence.ajouterChambre(Chambre("A101", 1, 150.0));
    residence.ajouterChambre(Chambre("A102", 2, 120.0));
    residence.ajouterChambre(Chambre("A103", 1, 150.0));

    // Ajout d'étudiants
    Etudiant e1("ETU01", "Dupont", "Jean", "jean.dupont@email.com", "0601020304");
    Etudiant e2("ETU02", "Martin", "Alice", "alice.martin@email.com", "0605060708");
    residence.ajouterEtudiant(e1);
    residence.ajouterEtudiant(e2);

    // Enregistrement de demandes
    residence.enregistrerDemande(DemandeHebergement("D01", "2026-09-01", "2026-09-15", &e1));
    residence.enregistrerDemande(DemandeHebergement("D02", "2026-09-02", "2026-09-15", &e2));

    // Consultation initiale
    residence.consulterChambresDisponibles();

    // Attribution d'une chambre
    cout << "\n--- Test d'attribution ---" << endl;
    residence.attribuerChambre("D01", "A101", "2026-09-15");

    // Affichage des statistiques et du taux d'occupation (Extension)
    residence.afficherStatistiques();

    // Consultation des chambres occupées
    residence.consulterChambresOccupees();

    // Enregistrement d'un départ
    cout << "\n--- Test de départ ---" << endl;
    residence.enregistrerDepart("A101", "2027-06-30");

    // Historique
    residence.consulterHistorique();

    // Statistiques finales
    residence.afficherStatistiques();

    return 0;
}
