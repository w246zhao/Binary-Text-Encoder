#include <iostream>
#include <cmath>

char *encode( char *plaintext, unsigned long key );
char *decode( char *ciphertext, unsigned long key );

int main() {
    // unencoded string
    char str0[]{ "A Elbereth Gilthoniel\nsilivren penna miriel\n"
                  "o menel aglar elenath!\nNa-chaered palan-diriel\n"
                  "o galadhremmin ennorath,\nFanuilos, le linnathon\n"
                  "nef aear, si nef aearon!" };

    unsigned long key = 51323;
    char *ciphertext;
    char *plaintext;

    std::cout << "Original String:" << std::endl;
    std::cout << "\"" << str0 << "\"" << std::endl;

    ciphertext = encode(str0,key);

    std::cout<< "\n\nEncoded String:" <<std::endl;
    std::cout<< "\"" << ciphertext << "\"" << std::endl;

    plaintext = decode( ciphertext, key );

    std::cout << "\n\nDecoded String:" << std::endl;
    std::cout << "\"" << plaintext << "\"" << std::endl;

    delete ciphertext;
    ciphertext = nullptr;
    delete plaintext;
    plaintext = nullptr;

    return 0;
}

char *encode( char *plaintext, unsigned long key ){
    unsigned char pad[256];
    char *asciiArmour;
    unsigned char i = 0, j = 0, k, r, R;
    unsigned char temp;
    unsigned int decimal;
    int length =0, encodedLength =0;

    //determining the length of the character array
    while (*(plaintext+length)!= '\0'){
        length++;
    }

    encodedLength = int(ceil(length/4.0))*4;

    unsigned char encodedText[encodedLength];

    asciiArmour = new char[5 * int(ceil(length / 4.0)) + 1];

    for (int i = 0; i < length; i++){
        encodedText[i] = *(plaintext+i);
    }

    for (int i = length; i  < encodedLength; i++){
        encodedText[i] = '\0';
    }

    //initializing state array
    for (int i = 0; i < 256; i++){
        pad[i] = i;
    }

    //randomizing state array
    for (int a = 1; a < 257; a++){
        k = i%64;
        j += pad[i] + ((key&(1UL<<k))>>k);
        temp = pad[i];
        pad[i] = pad[j];
        pad[j] = temp;
        i += 1%256;
    }

    //encoding string
    for (int b = 0; b < encodedLength; b++){
        i += 1%256;
        j+= pad[i]%256;
        temp = pad[i];
        pad[i] = pad[j];
        pad[j] = temp;
        r = pad[i] + pad[j]%256;
        R = pad[r];
        encodedText[b] ^= R;
    }

    //ascii armour
    for (int c = 0; c < int(ceil(length/4.0)); c++){
        decimal = 0;
        for (int d = 0; d < 4; d++){
            decimal+= static_cast<unsigned char>(encodedText[c*4+d])<<((3-d)*8);
        }
        for (int i = 1; i < 6; i++){
            asciiArmour[c*5+5-i] = static_cast<unsigned char>(decimal%85 + 33);
            decimal /= 85;
        }
    }
    asciiArmour[5*int(ceil(length/4.0))] = '\0';
    return asciiArmour;
}

char *decode( char *ciphertext, unsigned long key ){
    unsigned char pad[256];
    char *plaintext = new char;
    unsigned char i = 0, j = 0, k, r, R;
    unsigned char temp;
    unsigned int decimal;
    int length =0, encodedLength =0;

    //determining the length of the character array
    while (*(ciphertext+length)!= '\0'){
        length++;
    }
    encodedLength = (length/5)*4;

    unsigned char encodedText[encodedLength];
    plaintext = new char[encodedLength];

    //reverse ascii armour
    for (int c = 0; c < (length/5); c++){
        decimal = 0;
        for (int i = 1; i < 6; i++){
            int adder = static_cast<unsigned int>(ciphertext[c*5 + i - 1]-33);
            for (int j = 0; j <5-i; j++){
                adder *= 85;
            }
            decimal+= adder;
        }
        for (int d = 3; d >-1; d--){
            encodedText[c*4+d] = static_cast<unsigned char>(decimal %256);
            decimal /= 256;
        }
    }

    for (int i = 0; i < encodedLength; i++){
        plaintext[i] = encodedText[i];
    }

    //initializing state array
    for (int i = 0; i < 256; i++){
        pad[i] = i;
    }

    //randomizing state array
    for (int a = 1; a < 257; a++){
        k = i%64;
        j += pad[i] + ((key&(1UL<<k))>>k);
        temp = pad[i];
        pad[i] = pad[j];
        pad[j] = temp;
        i += 1%256;
    }

    //decoding string
    for (int b = 0; b < encodedLength; b++){
        i += 1%256;
        j+= pad[i]%256;
        temp = pad[i];
        pad[i] = pad[j];
        pad[j] = temp;
        r = pad[i] + pad[j]%256;
        R = pad[r];
        plaintext[b] ^= R;
    }
    return plaintext;
}