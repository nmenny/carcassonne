/*
    Copyright: (c) 2022, Bigotte Adrien, Lemkhali Bilal, Maistre Elise, Menny Nathan
    GNU General Public License v3.0+ (see COPYING or https://www.gnu.org/licenses/gpl-3.0.txt)
*/

#include <string>
#include <iostream>

#include "Joueurs.h"
#include "GestionnairesMemoire.h"

#include "Tuile.h"
#include "Jeu.h"

namespace Carcassonne {

    using namespace std;

    Jeu::~Jeu() {
        // Libere toutes les instances des gestionnaires de memoire
        Joueurs::libererInstance();
        Abbayes::libererInstance();
        Jardins::libererInstance();
        Pres::libererInstance();
        Rivieres::libererInstance();
        Routes::libererInstance();
        Villes::libererInstance();
        BasicMeeples::libererInstance();
        GdMeeples::libererInstance();
        Abbes::libererInstance();
    }

    void JeuConsole::partie() {
        std::string saisie;
        int indiceJeu = -1;
        bool actionPlacementValide = false;
        bool tuileChangee = true;

        // Tant qu'il reste des Tuiles a jouer
        while(plateau.peutJouerDesTuiles())
        {
            interface.affichePlateau(plateau);

            interface.afficheJoueur(*joueurCourant);

            // Saisie utilisateur de l'action qu'il souhaite realiser
            do
            {
                actionPlacementValide = false;

                if(tuileChangee) {
                    interface.afficheTuileCourante(*plateau.getTuile(), plateau.getPioche().getTaillePioche());

                    recupEmplacementsJouables();
                    interface.afficheEmplacementsJouables(emplacementJouables);
                    tuileChangee = false;
                }

                // Tant que l'utilisateur ne rentre pas une action viable, il doit refaire la saisie

                saisie = interface.demanderAction();

                if(saisie == COMMANDE_ROTATION_DROITE) {
                    plateau.p_getTuile()->rotation(directionRotation::droite);
                    tuileChangee = true;
                } else if(saisie == COMMANDE_ROTATION_GAUCHE) {
                    plateau.p_getTuile()->rotation(directionRotation::gauche);
                    tuileChangee = true;
                } else if(saisie == COMMANDE_REPIOCHER) {
                    plateau.remettreTuile();
                    tuileChangee = true;
                } else {
                    try {
                        indiceJeu = std::stoi(saisie);

                        if(indiceJeu >= 0 && indiceJeu < static_cast<int>(emplacementJouables.size())) {
                           actionPlacementValide = true;
                        } else {
                            interface.afficheMessageErreur(InterfaceError::indiceIncorrect, "N'est pas dans le bon interval !");
                        }

                    } catch(...) {
                        interface.afficheMessageErreur(InterfaceError::indiceIncorrect);
                    }
                }


            }
            while(!actionPlacementValide);


            placerTuile(indiceJeu);
            tuileChangee = true;

            nextJoueur();
        }

        // Fin de partie

        interface.afficheFinPartie();

        interface.affichePlateau(plateau);

        interface.afficheScore(*joueurs);

        interface.demanderPointsAAttribuer(*joueurs);

        interface.afficheScore(*joueurs);

        interface.afficheGagnant(getJoueurGagnant());
    }


    const Tuile* JeuGraphique::placerTuile(Coordonnee c) {
        const Tuile* tuilePosee = plateau.poserTuile(c);
        return tuilePosee;
    }
}
