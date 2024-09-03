//AES 32-bit.cpp 
#include<iostream>
#include "AES_ende.cpp"	

using namespace std;

int main(){
	FILE *fp, *wp,*ivp;		
	bool loop=true,en_de;	
	int opMode,KeySize;		
	char fileName[50];	
	unsigned char input_key[32];
	
	while(loop){
	cout<<"////AES 32-bit system////"<<endl;
	cout<<"Enter the mode of operation:"<<endl
		<<"1.ECB"<<endl<<"2.CBC"<<endl<<"3.CTR"<<endl<<"4.CFB-8"<<endl<<"5.OFB-8"<<endl;
	cin>>opMode;
	cout<<"Encryption or Decrption (Enter 1 or 0) :";		
	cin>>en_de;
	cout<<"Enter AES key size (Only 128/192/256) :";		
	cin>>KeySize;
		
	if (KeySize == 128){
        printf("Enter AES KEY (16 characters) : ");
        scanf("%s", input_key);
        for (int i = 0;i < 16;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 192){
        printf("Enter AES KEY (24 characters) : ");
        scanf("%s", input_key);
        for (int i = 0;i < 24;i++)
            Key[i] = input_key[i];
    }
    else if (KeySize == 256){
        printf("Enter AES KEY (32 characters) : ");
        scanf("%s", input_key);
        for (int i = 0;i < 32;i++)
            Key[i] = input_key[i];
    }
    else{
        printf("Error input to KeySize, Exit Now!");
        return 0;
    }
    
	if(en_de){
		cout<<"Enter plaintext file name => "; 
		cin>>fileName;
		if ((fp = fopen(fileName, "rb")) == NULL){
        cout<<"Open file Erorr...";
    }
    printf("Enter file name to store cipher => "); 
    cin>>fileName;
    wp = fopen(fileName,"wb");
	printf("Enter file name to store initialization vector => "); 
    cin>>fileName;
    ivp = fopen(fileName,"w");

	}else if(en_de==false){
		cout<<"Enter ciphertext file name => ";
		cin>>fileName;
		if ((fp = fopen(fileName, "rb")) == NULL){		
        cout<<"Open file Erorr...";
    	}
    	
		cout<<"Enter file name to store plaintext  => ";
		cin>>fileName;
		wp = fopen(fileName,"wb");				
		
		cout<<"Enter initialization vector file name => ";
		cin>>fileName;
		if ((ivp = fopen(fileName, "r")) == NULL){	
        	cout<<"Open file Erorr...";
    	}
	}else{
		cout<<"[WARNING]choice encryption/decryption !";
	}
	
 
 	if(opMode==1){
 		if(en_de==1)
 			ecb(fp,wp,KeySize,input_key);
 		else
 			ecb_Inv(fp,wp,KeySize,input_key);
 	}
    
    if (opMode==2){
    	if(en_de==1)
    		cbc(fp,wp,ivp,KeySize,input_key);
    	else
    		cbc_Inv(fp,wp,ivp,KeySize,input_key);
	}
	if (opMode==3){
    	if(en_de==1)
    		ctr(fp,wp,ivp,KeySize,input_key);
    	else
    		ctr_Inv(fp,wp,ivp,KeySize,input_key);
	}
	if (opMode==4){
    	if(en_de==1)
    		cfb(fp,wp,ivp,KeySize,input_key);
    	else
    		cfb_Inv(fp,wp,ivp,KeySize,input_key);
	}
	if (opMode==5){
    	if(en_de==1)
    		ofb(fp,wp,ivp,KeySize,input_key);
    	else
    		ofb_Inv(fp,wp,ivp,KeySize,input_key);
	}
	
    char e;
    cout<<"Do you want to contuine?(y/n)";
    cin>>e;
    
    if(e=='y')
    	system("CLS");
    else
    	break;
	}
}