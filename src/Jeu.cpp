#include <string>
#include <iostream>

#include "Joueurs.h"
#include "GestionnaireMemoireEnvironnement.h"
#include "GestionnaireMemoireMeeple.h"

#include "Tuile.h"
#include "Jeu.h"

namespace Carcassonne {

    using namespace std;

    Jeu::~Jeu() {
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

    void Jeu::partie() {

        while(!plateau.isEmpty())
        {
            interface.affichePlateau(plateau);

            interface.afficheJoueur(*joueurCourant);

            interface.afficheTuileCourante(*plateau.getTuile());

            recupEmplacementsJouables();

            interface.afficheEmplacementsJouables(emplacementJouables);

            size_t saisie = -1;

            // Tant que l'utilisateur ne rentre pas une action viable, il doit refaire la saisie
            while(true) {
               try {
                    saisie = interface.demanderOuPoser();
               } catch(InterfaceException& e) {}

               if(saisie < emplacementJouables.size()) {
                  break;
               }
            }

            plateau.poserTuile(emplacementJouables[saisie]);

            joueurCourant->incrementScore(1);

            nextJoueur();
        }

        interface.afficheFinPartie();

        interface.affichePlateau(plateau);

        interface.afficheScore(*joueurs);

        interface.demanderPointsAAttribuer(*joueurs);

        interface.afficheScore(*joueurs);

        interface.afficheGagnant(getJoueurGagnant());

    }

}
