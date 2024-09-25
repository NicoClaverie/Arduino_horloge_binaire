// Broches pour les registres à décalage
const int dataPin = 8;  // DS
const int clockPin = 9; // SHCP
const int latchPin = 10; // STCP

// Broches des boutons poussoirs
const int buttonHourPin = 2;   // Bouton pour régler les heures
const int buttonMinutePin = 3; // Bouton pour régler les minutes

// Variables pour stocker l'heure, les minutes et les secondes
int hours = 0;
int minutes = 0;
int seconds = 0;

// Variables pour l'état des boutons
int lastButtonHourState = HIGH;   // Initialement, pas de pression sur le bouton
int lastButtonMinuteState = HIGH; // Initialement, pas de pression sur le bouton
unsigned long lastDebounceTime = 0; // Pour éviter le "rebond" des boutons
unsigned long debounceDelay = 50;   // Délai de débounce de 50 ms

void setup() {
    // Définir les broches comme des sorties
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    
    // Définir les broches des boutons comme entrées avec pullup interne
    pinMode(buttonHourPin, INPUT_PULLUP);
    pinMode(buttonMinutePin, INPUT_PULLUP);
}

void loop() {
    // Lire l'état des boutons poussoirs
    int buttonHourState = digitalRead(buttonHourPin);
    int buttonMinuteState = digitalRead(buttonMinutePin);

    // Si le bouton des heures est pressé
    if (buttonHourState == LOW && lastButtonHourState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
        hours = (hours + 1) % 24; // Incrémenter les heures, limite à 24h (format 24h)
        lastDebounceTime = millis(); // Réinitialiser le délai anti-rebond
    }
    
    // Si le bouton des minutes est pressé
    if (buttonMinuteState == LOW && lastButtonMinuteState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
        minutes = (minutes + 1) % 60; // Incrémenter les minutes, limite à 60 minutes
        seconds = 0; // Réinitialiser les secondes à 0 quand on règle les minutes
        lastDebounceTime = millis(); // Réinitialiser le délai anti-rebond
    }

    // Stocker l'état des boutons pour la prochaine itération
    lastButtonHourState = buttonHourState;
    lastButtonMinuteState = buttonMinuteState;

    // Créer des variables pour stocker l'état des LED
    byte ledStateHours = (hours & 0x1F);    // 5 bits pour les heures
    byte ledStateMinutes = (minutes & 0x3F); // 6 bits pour les minutes
    byte ledStateSeconds = (seconds & 0x3F); // 6 bits pour les secondes

    // Envoyer les données aux registres à décalage
    digitalWrite(latchPin, LOW); // Préparer les registres à recevoir les données

    shiftOut(dataPin, clockPin, MSBFIRST, ledStateSeconds); // Envoyer les secondes
    shiftOut(dataPin, clockPin, MSBFIRST, ledStateMinutes); // Envoyer les minutes
    shiftOut(dataPin, clockPin, MSBFIRST, ledStateHours);   // Envoyer les heures

    digitalWrite(latchPin, HIGH); // Mettre à jour l'affichage des LED

    // Incrémenter les secondes
    delay(1000); // Attendre une seconde
    seconds = (seconds + 1) % 60;

    // Gestion des minutes et des heures
    if (seconds == 0) {
        minutes = (minutes + 1) % 60;
        if (minutes == 0) {
            hours = (hours + 1) % 24;
        }
    }
}
