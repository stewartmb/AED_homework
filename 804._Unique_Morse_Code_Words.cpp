#include "Table_Hash.cpp"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_set>

using namespace std;

// Lista de las representaciones Morse para cada letra
vector<string> morse = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

// Función que resuelve el problema
int uniqueMorseRepresentations(vector<string>& words) {
    unordered_set<string> uniqueMorseCodes;  // Usamos un set para guardar los códigos Morse únicos

    // Para cada palabra
    for (const string& word : words) {
        string morseCode = "";  // Variable para construir el código Morse de la palabra
        for (char c : word) {
            morseCode += morse[c - 'a'];  // Convertir cada letra al código Morse correspondiente
        }
        uniqueMorseCodes.insert(morseCode);  // Insertar la representación Morse en el set
    }

    return uniqueMorseCodes.size();  // El tamaño del set será el número de representaciones únicas
}

int main() {
    // Ejemplo de entrada
    vector<string> palabras = {"gin", "zen", "gig", "msg"};

    // Llamar a la función y mostrar el resultado
    cout << "Número de representaciones Morse únicas: " << uniqueMorseRepresentations(palabras) << endl;

    return 0;
}
