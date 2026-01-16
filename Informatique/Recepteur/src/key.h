#define A a
// &é"'(-è_çà)
// 1234567890-

const char* azerty_to_qwerty(const char* c) {
    if (strcmp(c, "a") == 0) return "q";
    if (strcmp(c, "z") == 0) return "w";
    if (strcmp(c, "q") == 0) return "a";
    if (strcmp(c, "w") == 0) return "z";

    if (strcmp(c, "A") == 0) return "Q";
    if (strcmp(c, "Z") == 0) return "W";
    if (strcmp(c, "Q") == 0) return "A";
    if (strcmp(c, "W") == 0) return "Z";

    // Caractères spéciaux
    if (strcmp(c, "&") == 0) return "1";
    if (strcmp(c, "é") == 0) return "2";
    if (strcmp(c, "\"") == 0) return "3";
    if (strcmp(c, "'") == 0) return "4";
    if (strcmp(c, "(") == 0) return "5";
    if (strcmp(c, "-") == 0) return "6";
    if (strcmp(c, "è") == 0) return "7";
    if (strcmp(c, "_") == 0) return "8";
    if (strcmp(c, "ç") == 0) return "9";
    if (strcmp(c, "à") == 0) return "0";

    // Sinon on retourne la chaîne d'origine
    return c;
}

