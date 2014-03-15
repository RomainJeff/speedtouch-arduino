#include <LiquidCrystal.h>

// BTN JOUEURS
// J1 = VERT
// J2 = JAUNE
const int playerOne = 15;
const int playerTwo = 14;

// LEDS JOUEURS
const int ledPlayerOne = 16;
const int ledPlayerTwo = 17;

// BUZZER
const int buzzer = 6;

// LCD BROCHES
const int lcdBroches[] = {13,12,5,4,3,2};

// SCORES JOUEURS
int scorePlayerOne = 0;
int scorePlayerTwo = 0;

boolean hasBuzzed = false;
boolean stopped = false;

boolean ended = false;

// LCD INIT
LiquidCrystal lcd(lcdBroches[0], lcdBroches[1], lcdBroches[2], lcdBroches[3], lcdBroches[4], lcdBroches[5]);

void setup()
{
    Serial.begin(115200);
   
    // INPUT
    pinMode(playerOne, INPUT);
    pinMode(playerTwo, INPUT);
   
    // OUTPUT
    pinMode(ledPlayerOne, OUTPUT);
    pinMode(ledPlayerTwo, OUTPUT);
    pinMode(buzzer, OUTPUT);
    
    // INITIALISATION DES BOUTONS
    digitalWrite(playerOne, HIGH);
    digitalWrite(playerTwo, HIGH);

    // LCD
    lcd.begin(16, 2);
    lcd.clear();
    delay(10);

    initScore();
    AfficherScore();

}

void loop() {
    
    // Tant que le jeu n'est pas termine
    while (!ended) {
        
        // On definie un temps aleatoire
        int time = random(1000, 5000);
   
        // On attends le temps genere
        delay(time);
 
        // On fait sonner le buzzer
        Sonner();
    
        // Si le buzzer a sonnee
        if (hasBuzzed) {
            // On verifie qui a clique en 1er
            Verifier();
            // On verifie si la partie est terminee
            isEnded();
            // On affiche le score
            AfficherScore();
        }

    }
    
    // Si la partie est fini
    // et que le joueur 1 a un score plus eleve que le joueur 2
    // le joueur 1 gagne
    // Sinon le joueur 2 gagne
    if (scorePlayerOne > scorePlayerTwo) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("VERT GAGNE");
        // Fix du clignotement
        lcd.noBlink();
    } else {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("JAUNE GAGNE");
        // Fix du clignotement
        lcd.noBlink();
    }

    delay(10);
}

/**
 *
 * Allume le buzzer
 *
 */
void Sonner()
{
    hasBuzzed = true;
    analogWrite(buzzer, 255);
    delay(300);
    Eteindre();
}

/**
 *
 * Eteind le buzzer
 *
 */
void Eteindre()
{
    digitalWrite(buzzer, LOW); 
}

/**
 *
 * Verifie que la partie est toujours en cours
 *
 */
void Verifier()
{
    stopped = false;
    
    // Tant que la manche n'est pas stoppe
    while(!stopped) {
        int pOne = digitalRead(playerOne);
        int pTwo = digitalRead(playerTwo); 
        
        // Si la manche n'est pas stoppe
        if (!stopped) {

            // Si le joueur 1 n'a pas appuye 
            // et que le joueur 2 a appuye
            // On increment le score du joueur 2
            // et on allume sa LED
            // et on stoppe la manche
            if (pOne == 0 && pTwo == 1) {
                scorePlayerOne++;
                stopped = true;
                AllumerLed(1);
            }

            // Si le joueur 2 n'a pas appuye 
            // et que le joueur 1 a appuye
            // On increment le score du joueur 1
            // et on allume sa LED
            // et on stoppe la manche
            if (pTwo == 0 && pOne == 1) {
                scorePlayerTwo++;
                stopped = true;
                AllumerLed(2);
            } 
        }
    }
    
    // On reinitialise le buzzer
    hasBuzzed = false;
}

/**
 *
 * Allume la led du joueur qui a clique en premier
 * @param int player Le numero du joueur
 *
 */
void AllumerLed(int player)
{
    // Si c'est le joueur 1
    // On allume sa LED
    if (player == 1) {
        digitalWrite(ledPlayerOne, HIGH); 
    }

    // Si c'est le joueur 2
    // On allume sa LED
    if (player == 2) {
        digitalWrite(ledPlayerTwo, HIGH); 
    }

    delay(1000);

    // On re-initialise les LEDs
    digitalWrite(ledPlayerOne, LOW);
    digitalWrite(ledPlayerTwo, LOW);
}

/**
 *
 * Affiche le score sur le Terminal Serie
 *
 */
void AfficherScore()
{
    lcd.clear();
    delay(10);

    // On renote les noms
    initScore();

    // Score joueur 1
    lcd.setCursor(1, 2);
    lcd.print(scorePlayerOne);
    // Score joueur 2
    lcd.setCursor(13, 2);
    lcd.print(scorePlayerTwo);
}

/**
 *
 * Initialise les scores en affichant les noms
 *
 */
void initScore()
{
    lcd.home();

    // Affichage nom du joueur 1
    lcd.setCursor(0, 0);
    lcd.print("VERT");
    delay(10);
    // Affichage nom du joueur 2
    lcd.setCursor(11, 0);
    lcd.print("JAUNE");
}

/**
 *
 * Verifie si la partie est terminee
 * elle est termine si le score d'un des joueurs est de 9
 *
 */
void isEnded()
{
    // Si le joueur 1 a 9
    if (scorePlayerOne == 9) {
        ended = true;
    } 
    
    // Si le joueur 2 a 9
    if (scorePlayerTwo == 9) {
        ended = true;
    }
}
