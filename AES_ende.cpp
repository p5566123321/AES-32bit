#include <stdio.h>
#include <iostream>
#include <cstdlib>	
#include <ctime>  

#define Nb 4 	

#define xtime(x)   ((x << 1) ^ (((x >> 7) & 0x01) * 0x1b))	

using namespace std;

int Nr = 0; 
int Nb_k = 0; 

unsigned char in[16];        
unsigned char out[16];      
unsigned char state[4][4];   
unsigned char Roundkey[240]; 
unsigned char Key[32];         


/* S-box */
int S_Box[256] =   
{
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  //F
};
int S_Box_Inv[256] =   
{
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};


int Rcon[11] = 
{
//   0     1     2     3      4    5     6     7     8    9     10
    0x87, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};
void KeyExpansion(){
    unsigned char tempByte[4]; 
    unsigned char a0;      
    
    
    for (int i = 0;i < Nb_k;i++){
        Roundkey[i * 4] = Key[i * 4];
        Roundkey[i *4 + 1] = Key[i * 4 + 1];
        Roundkey[i *4 + 2] = Key[i * 4 + 2];
        Roundkey[i *4 + 3] = Key[i * 4 + 3];
    }

   
    for (int i = Nb_k;i < (Nb * (Nr + 1));i++)
    {
        for (int j = 0;j < 4;j++){ 
            tempByte[j] = Roundkey[(i - 1) * 4 + j]; 
        }
        if (i % Nb_k == 0){
            
            a0 = tempByte[0];
            tempByte[0] = tempByte[1];
            tempByte[1] = tempByte[2];
            tempByte[2] = tempByte[3];
            tempByte[3] = a0;

            
            tempByte[0] = S_Box[(int)tempByte[0]];
            tempByte[1] = S_Box[(int)tempByte[1]];
            tempByte[2] = S_Box[(int)tempByte[2]];
            tempByte[3] = S_Box[(int)tempByte[3]];
            
            
            tempByte[0] = tempByte[0] ^ Rcon[i / Nb_k]; 
        }
        else if (Nb_k == 8 && i % Nb_k == 4){
            
            tempByte[0] = S_Box[(int)tempByte[0]];
            tempByte[1] = S_Box[(int)tempByte[1]];
            tempByte[2] = S_Box[(int)tempByte[2]];
            tempByte[3] = S_Box[(int)tempByte[3]];
        }
        
        Roundkey[i * 4 + 0] = Roundkey[(i - Nb_k) * 4 + 0] ^ tempByte[0];
        Roundkey[i * 4 + 1] = Roundkey[(i - Nb_k) * 4 + 1] ^ tempByte[1];
        Roundkey[i * 4 + 2] = Roundkey[(i - Nb_k) * 4 + 2] ^ tempByte[2];
        Roundkey[i * 4 + 3] = Roundkey[(i - Nb_k) * 4 + 3] ^ tempByte[3];   
    }
}
void AddRoundKey(int round)
{
    
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            state[j][i] ^= Roundkey[(i * Nb + j) + (round * Nb * 4)]; 
}

// S-Box Substitution
void SubBytes(){
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            state[i][j] = S_Box[state[i][j]];
}

void ShiftRows(){
    unsigned char tempByte;
    
    // 2nd row left Circular Shift 1 byte
    tempByte    = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = tempByte;

    // 3th row left Circular Shift 2 byte
    tempByte    = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = tempByte;

    tempByte    = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = tempByte;

    // 4th row left Circular Shift 3 byte
    tempByte    = state[3][0];
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = tempByte;
}


void MixColumns()
{
    unsigned char Tmp,Tm,t;
    for(int i = 0;i < 4;i++)
    {    
        t   = state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        Tm  = state[0][i] ^ state[1][i]; Tm = xtime(Tm); state[0][i] ^= Tm ^ Tmp ;
        Tm  = state[1][i] ^ state[2][i]; Tm = xtime(Tm); state[1][i] ^= Tm ^ Tmp ;
        Tm  = state[2][i] ^ state[3][i]; Tm = xtime(Tm); state[2][i] ^= Tm ^ Tmp ;
        Tm  = state[3][i] ^ t;           Tm = xtime(Tm); state[3][i] ^= Tm ^ Tmp ;
    }
}

void Cipher()
{
    int round = 0;
    
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            state[j][i] = in[i * 4 + j]; 
    

    AddRoundKey(0);

    for (round = 1;round < Nr;round++){
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }


    SubBytes();
    ShiftRows();
    AddRoundKey(Nr);
	
    for(int i = 0;i < 4;i++) 
        for(int j = 0;j < 4;j++)
            out[i * 4 + j]=state[j][i];
        
    
}
void SubBytes_Inv(){
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            state[i][j] = S_Box_Inv[state[i][j]];
}


void ShiftRows_Inv(){
    unsigned char tempByte;
    
    tempByte    = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = tempByte;

    tempByte    = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = tempByte;

    tempByte    = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = tempByte;

    tempByte    = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = tempByte;
}


#define Multiply(x,y)   ((y      & 0x01) * x) \
                      ^ ((y >> 1 & 0x01) * xtime(x)) \
                      ^ ((y >> 2 & 0x01) * xtime(xtime(x))) \
                      ^ ((y >> 3 & 0x01) * xtime(xtime(xtime(x)))) \
                      ^ ((y >> 4 & 0x01) * xtime(xtime(xtime(xtime(x)))))


void MixColumns_Inv()
{
    unsigned char d0, d1, d2, d3;
    for (int i = 0;i < 4;i++){

        d0 = state[0][i];
        d1 = state[1][i];
        d2 = state[2][i];
        d3 = state[3][i];

        state[0][i] = Multiply(d0, 0x0e) ^ Multiply(d1, 0x0b) ^ 
                      Multiply(d2, 0x0d) ^ Multiply(d3, 0x09);
        state[1][i] = Multiply(d0, 0x09) ^ Multiply(d1, 0x0e) ^ 
                      Multiply(d2, 0x0b) ^ Multiply(d3, 0x0d);
        state[2][i] = Multiply(d0, 0x0d) ^ Multiply(d1, 0x09) ^ 
                      Multiply(d2, 0x0e) ^ Multiply(d3, 0x0b);
        state[3][i] = Multiply(d0, 0x0b) ^ Multiply(d1, 0x0d) ^ 
                      Multiply(d2, 0x09) ^ Multiply(d3, 0x0e);
    }
}


void Cipher_Inv(){

   int round = Nr - 1;
   
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            state[j][i] = in[i * 4 + j]; 
    
    
    AddRoundKey(Nr);

    
    for (round = Nr - 1;round > 0;round--){
        ShiftRows_Inv();
        SubBytes_Inv();
        AddRoundKey(round);
        MixColumns_Inv();
    }

    ShiftRows_Inv();
    SubBytes_Inv();
    AddRoundKey(0);

    for(int i = 0;i < 4;i++) 
        for(int j = 0;j < 4;j++)
            out[i * 4 + j]=state[j][i];
}

void printUnsignedCharArrayToInt(unsigned char in[], int size){
    for (int i = 0;i < size;i++){
        printf("%d ", in[i]);
    }
}

void ecb(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char plaintext_block[16];	
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6;
    
    if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
    KeyExpansion();
    
    printf("---------------------------------------------\n");
    while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
    	}

        for (int c = 0;c < 16;c++){
            in[c] = plaintext_block[c];
        }
         
        Cipher(); 
		
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
  		
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(in, 16);    
        printf("\n");

        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp);
        }
    }

    fclose(fp);
    rewind(wp);
    fclose(wp);

    printf("------------------------------------------------\n");
    printf("Encryption process complete !! \n");
}
void ecb_Inv(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char Ciphertext_block[16];
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6;
    
    if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
    KeyExpansion();
    printf("---------------------------------------------\n");
    while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
        for (int c = 0;c < 16;c++){
            in[c] = Ciphertext_block[c];
        }

       
        Cipher_Inv(); 
        
        // write decrypted plaintext to file and discard padding byte
        for (int c = 0;c < 16;c++){
            // discard padding byte
            if (out[c] == 0x00){
                feof_flag = 0;
                break;
            }
            
            fprintf(wp, "%c", out[c]);
        }

        // print Cipher(character format) in Integer Format
        // print plaintext(character format) in Integer Format
        printf("Block %d(128 bits) - Ciphertext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(in, 16);    
        printf("\n");
        // print Decrypted plaintext(character format) in Integer Format
        printf("Block %d(128 bits) - Plaintext  (Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); // if not read EOF, restore character
        }
    }



    fclose(fp);
    rewind(wp);
    fclose(wp);
    printf("------------------------------------------------\n");
    printf("Decryption process complete !! \n");
}

void cbc(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char plaintext_block[16];
	unsigned char tmp[16];
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6; 	
	srand( time(NULL) );
	
	
	if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
	for(int i=0;i<16;i++){
	iv[i]=rand()%128;
	}
	KeyExpansion();
	
	printf("---------------------------------------------\n");

	cout<<"initialization vector :";
	for(int i=0;i<16;i++){
		printf("%d ",iv[i]);
	}
	cout<<endl;
	
	for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
        }
    
		for (int c = 0;c < 16;c++){
            in[c] = plaintext_block[c]^iv[c];
        }
    	Cipher(); 
		for (int c = 0;c < 16;c++){
			tmp[c]=out[c];
		}
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(plaintext_block, 16);    
        printf("\n");
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
        
		while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
        }
		
        for (int c = 0;c < 16;c++){
            in[c] = plaintext_block[c]^tmp[c];
        }

      
        Cipher(); 
		for (int c = 0;c < 16;c++){
		tmp[c]=out[c];
		}
        
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
       
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(plaintext_block, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
    }

    fclose(fp);
    rewind(wp);
    fclose(wp);

    printf("------------------------------------------------\n");
    printf("Encryption process complete !! \n");
	
} 

void cbc_Inv(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char Ciphertext_block[16];
	unsigned char tmp[16];
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6;
	 
	 if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
    
    KeyExpansion();
    
    int y;
	cout<<"Please Enter initialization vector  => ";
	for(int i=0;i<16;i++){
		cin>>y;
		iv[i]=y;
	}
	
	
	
	for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
        
        for (int c = 0;c < 16;c++){
            in[c] = Ciphertext_block[c];
            tmp[c]= Ciphertext_block[c];
        }
    Cipher_Inv(); 
        
        
        
        for (int c = 0;c < 16;c++){
        	out[c]=out[c]^iv[c];
		}
        for (int c = 0;c < 16;c++){
       
            if (out[c] == 0x00){
                feof_flag = 0;
                break;
            }
            
            fprintf(wp, "%c", out[c]);
        }

        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum);
        printUnsignedCharArrayToInt(in, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); // if not read EOF, restore character
    	}
    
    
    while(feof_flag == 1){
        
        
        
        for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
        
        for (int c = 0;c < 16;c++){
            in[c] = Ciphertext_block[c];
        }

       
        Cipher_Inv(); 
        for (int c = 0;c < 16;c++){
        	out[c]^=tmp[c];
		}
		for (int c = 0;c < 16;c++){
			tmp[c]=in[c];
		}
        for (int c = 0;c < 16;c++){
            if (out[c] == 0x00){
                feof_flag = 0;
                break;
            }
            
            fprintf(wp, "%c", out[c]);
        }

        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum);
        printUnsignedCharArrayToInt(in, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
    }



    fclose(fp);
    rewind(wp);
    fclose(wp);
    printf("------------------------------------------------\n");
    printf("Decryption process complete !! \n");
    
} 
void ctr(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char plaintext_block[16];
	unsigned char counter[16]={0};
	unsigned char tmp[16];
	int blockNum = 0; 
	int x=0;
    bool feof_flag = 1;
    
    
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6; 	
	srand( time(NULL) );
	
	
	if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }

	for(int i=0;i<16;i++){
		iv[i]=rand()%128;
	}
	
	KeyExpansion();
	
	printf("---------------------------------------------\n");
	cout<<"initialization vector :";
	for(int i=0;i<16;i++){
		printf("%d ",iv[i]);
	}
	cout<<endl;
	
	

	
        
	while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
        }
		
        for (int c = 0;c < 16;c++){
            in[c] = iv[c]^counter[c];
        }

      
        Cipher(); 
		for (int c = 0;c < 16;c++){
		out[c]^=plaintext_block[c];
		}
    
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
       
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(plaintext_block, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
        for(int i=15;i>=0;i--) 
		    if(counter[i]<127){
		    	x++;
		    	counter[i]=x;
		    	break;
		    }
    }

    fclose(fp);
    rewind(wp);
    fclose(wp);

    printf("------------------------------------------------\n");
    printf("Encryption process complete !! \n");
}

void ctr_Inv(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char Ciphertext_block[16];
	unsigned char counter[16]={0};
	unsigned char tmp[16];
	int blockNum = 0; 
	int x=0;
    bool feof_flag = 1;
    
    
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6; 	
	srand( time(NULL) );
	
	
	if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }

	int y;
	cout<<"Please Enter initialization vector  => ";
	for(int i=0;i<16;i++){
		cin>>y;
		iv[i]=y;
	}
	
	KeyExpansion();
	
	printf("---------------------------------------------\n");
        
	while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
		
        for (int c = 0;c < 16;c++){
            in[c] = iv[c]^counter[c];
        }

      
        Cipher(); 
		for (int c = 0;c < 16;c++){
		out[c]^=Ciphertext_block[c];
		}
    
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
       
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(Ciphertext_block, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
        for(int i=15;i>=0;i--) 
		    if(counter[i]<127){
		    	x++;
		    	counter[i]=x;
		    	break;
		    }
    }

    fclose(fp);
    rewind(wp);
    fclose(wp);

    printf("------------------------------------------------\n");
    printf("Encryption process complete !! \n");
}

void ofb(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char plaintext_block[16];
	unsigned char tmp[16];
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6; 	
	srand( time(NULL) );
	
	
	if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
    int arr[8];
	for(int i=0;i<8;i++){
		arr[i]=rand()%128;
		iv[i]=arr[i];
	}
	for(int i=8;i<16;i++){
		iv[i]=arr[i-8];
	}
	KeyExpansion();
	
	printf("---------------------------------------------\n");
	cout<<"initialization vector :";
	for(int i=0;i<16;i++){
		printf("%d ",iv[i]);
	}
	cout<<endl;
	
	for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
        }
    
	for (int c = 0;c < 16;c++){
        in[c] = iv[c];
    }
	Cipher(); 
	for (int c = 0;c < 16;c++){
		tmp[c]=out[c];
		out[c]^=plaintext_block[c];
	}
    for(int c = 0; c < 16;c++)  
        fprintf(wp, "%c", out[c]);
    
    printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
    printUnsignedCharArrayToInt(plaintext_block, 16);    
    printf("\n");
    printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
    printUnsignedCharArrayToInt(out, 16);
    printf("\n");
    blockNum++;

    char c;
    if ((c = fgetc(fp)) == EOF){
        feof_flag = 0;
    }
    else{
        ungetc(c, fp); 
    }

	while(feof_flag == 1){
        
        for (int c = 0;c < 16;c++){
            plaintext_block[c] = fgetc(fp);
			
            if (feof(fp)){
                for (int padding = c;padding < 16;padding++){
                    plaintext_block[padding] = 0x00;
                }
                feof_flag = 0;
            }
        }
		
        for (int c = 0;c < 16;c++){
            in[c] = tmp[c];
        }

      
        Cipher(); 
		for (int c = 0;c < 16;c++){
		tmp[c]=out[c];
		out[c]^=plaintext_block[c];
		}
    
        for(int c = 0; c < 16;c++)  
            fprintf(wp, "%c", out[c]);
        
       
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum);
        printUnsignedCharArrayToInt(plaintext_block, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
    }

    fclose(fp);
    rewind(wp);
    fclose(wp);

    printf("------------------------------------------------\n");
    printf("Encryption process complete !! \n");
}

void ofb_Inv(FILE *fp,FILE *wp,int KeySize,unsigned char *input_key){
	unsigned char iv[16];
	unsigned char Ciphertext_block[16];
	unsigned char tmp[16];
	int blockNum = 0; 
    bool feof_flag = 1;
    Nb_k = KeySize / 32;
    Nr   = Nb_k + 6;
	 
	if (KeySize == 128){
        
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
       
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
    }
    
    KeyExpansion();
    
    int y;
	cout<<"Please Enter initialization vector  => ";
	for(int i=0;i<16;i++){
		cin>>y;
		iv[i]=y;
	}
	
	
	
	for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
        
    for (int c = 0;c < 16;c++){
        in[c] = iv[c];
    }
    Cipher(); 
        
        
        
    for (int c = 0;c < 16;c++){
    	tmp[c]=out[c];
    	out[c]^=Ciphertext_block[c];
	}
    for (int c = 0;c < 16;c++){
   
        if (out[c] == 0x00){
            feof_flag = 0;
            break;
        }
        
        fprintf(wp, "%c", out[c]);
    }

    
    printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum);
    printUnsignedCharArrayToInt(Ciphertext_block, 16);    
    printf("\n");
    
    printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum); 
    printUnsignedCharArrayToInt(out, 16);
    printf("\n");
    blockNum++;

    char c;
    if ((c = fgetc(fp)) == EOF){
        feof_flag = 0;
    }
    else{
        ungetc(c, fp); // if not read EOF, restore character
	}
    
    
    while(feof_flag == 1){
        
        
        
        for (int c = 0;c < 16;c++){
            Ciphertext_block[c] = fgetc(fp);
        }
        
        for (int c = 0;c < 16;c++){
            in[c] = tmp[c];
        }

       
        Cipher(); 
        for (int c = 0;c < 16;c++){
        	tmp[c]=out[c];
        	out[c]^=Ciphertext_block[c];
		}
		
        for (int c = 0;c < 16;c++){
            if (out[c] == 0x00){
                feof_flag = 0;
                break;
            }
            
            fprintf(wp, "%c", out[c]);
        }

        
        printf("Block %d(128 bits) - Ciphertext(Int format) : ", blockNum);
        printUnsignedCharArrayToInt(Ciphertext_block, 16);    
        printf("\n");
        
        printf("Block %d(128 bits) - Plaintext (Int format) : ", blockNum); 
        printUnsignedCharArrayToInt(out, 16);
        printf("\n");
        blockNum++;

        char c;
        if ((c = fgetc(fp)) == EOF){
            feof_flag = 0;
        }
        else{
            ungetc(c, fp); 
        }
    }



    fclose(fp);
    rewind(wp);
    fclose(wp);
    printf("------------------------------------------------\n");
    printf("Decryption process complete !! \n");
}

