// 
// 
// 

#include "SD_ListFiles.h"

#include "genieArduino.h"
#include "Touch_Screen_Definitions.h"
#include "Marlin.h"
#include "Configuration.h"
#include "stepper.h"
#include "temperature.h"
#include "cardreader.h"


Listfiles::Listfiles(){
	 dias=-1, horas=-1, minutos=-1;
	filmetros1 = 0 ,filmetros2=0;
	filgramos1 = 0 ,filgramos2=0;
 comandline[50];
 comandline2[18];
	simplify3D = -1;
}


void Listfiles::get_lineduration(void){
	
	card.openFile(card.filename, true);
	//memset(comandline, '\0', sizeof(char)*65 );
	//memset(comandline2, '\0', sizeof(char)*65 );
	dias=-1, horas=-1, minutos=-1;
	char serial_char='\0';
	//comandline="";
	int posi = 0;
	int linecomepoint=0;
	simplify3D=-1;
	while(linecomepoint < 3 && !card.isEndFile()){
		memset(comandline, '\0', sizeof(comandline) );
		while(comandline[0]!=';' && !card.isEndFile()){
			serial_char='\0';
			posi = 0;
			while(serial_char != '\n' && posi < 49 && !card.isEndFile()){
				
				int16_t n=card.get();
				serial_char = (char)n;
				comandline[posi]=serial_char;
				
				
				posi++;
			}
		}
		if(linecomepoint == 0){
			if (extract_data_Symplify() !=-1 ){
				linecomepoint = 3;
				simplify3D = 1;
				//Serial.println("Simplify");
			}
			
			else{
				simplify3D = 0;
				//Serial.println("CURA");
			}
		}
		linecomepoint++;
	}
	
	Serial.println(simplify3D);
	if(simplify3D == 0){
		//Serial.println("CURA-1");
		extract_data();
		memset(comandline, '\0', sizeof(comandline) );
		posi = 0;
		serial_char='\0';
		
		while(serial_char != '\n' && posi < 49 && !card.isEndFile()){
			
			int16_t n=card.get();
			serial_char = (char)n;
			comandline[posi]=serial_char;
			
			
			posi++;
		}
		extract_data1();
		card.closefile();
		memset(comandline, '\0', sizeof(comandline) );
	}
	else if (simplify3D == 1){
		//Serial.println("Simplify-1");
		card.sdfinalline();
		memset(comandline, '\0', sizeof(comandline) );
		
		
		
		linecomepoint=0;
		while(linecomepoint < 2 && !card.isEndFile()){
			memset(comandline, '\0', sizeof(comandline) );
			while(comandline[0]!=';' && !card.isEndFile()){
				serial_char='\0';
				posi = 0;
				while(serial_char != '\n' && posi < 49 && !card.isEndFile()){
					
					int16_t n=card.get();
					serial_char = (char)n;
					comandline[posi]=serial_char;
					
					
					posi++;
				}
				//Serial.println(comandline);
			}
			//Serial.println(comandline);
			linecomepoint++;
		}
		extract_data();
		memset(comandline, '\0', sizeof(comandline) );
		posi = 0;
		serial_char='\0';
		linecomepoint = 0;
		while(linecomepoint < 3 && !card.isEndFile()){
			memset(comandline, '\0', sizeof(comandline) );
			while(comandline[0]!=';' && !card.isEndFile()){
				serial_char='\0';
				posi = 0;
				while(serial_char != '\n' && posi < 49 && !card.isEndFile()){
					
					int16_t n=card.get();
					serial_char = (char)n;
					comandline[posi]=serial_char;
					
					
					posi++;
				}
				//Serial.println(comandline);
			}
			linecomepoint++;
		}
		extract_data1();
		card.closefile();
		memset(comandline, '\0', sizeof(comandline) );
	}
	
	
}
int Listfiles::extract_data_Symplify(void){
	int Symplify_ok =-1;
	sscanf( comandline, "; G-Code generated by Simplify3D(R) Version %d", &Symplify_ok);
	//Serial.println(Symplify_ok);
	return Symplify_ok;
}
void Listfiles::extract_data(void){
	if (simplify3D == 0){
		sscanf( comandline, ";Print time: %d days %d hours %d minutes", &dias, &horas, &minutos);
		if(horas == -1){
		sscanf( comandline, ";Print time: %d day %d hours %d minutes", &dias, &horas, &minutos);
		if(minutos == -1){
		sscanf( comandline, ";Print time: %d day %d hour %d minutes", &dias, &horas, &minutos);
		}
		}
		else{
		if(minutos == -1){
		sscanf( comandline, ";Print time: %d days %d hour %d minutes", &dias, &horas, &minutos);
		}
		}
		if(minutos ==-1 && horas ==-1){
			dias = 0;
			sscanf( comandline, ";Print time: %d hours %d minutes", &horas, &minutos);
		}
		if(horas !=-1 && minutos==-1){
			sscanf( comandline, ";Print time: %d hour %d minutes", &horas, &minutos);
		}
		if(minutos ==-1){
			dias = 0;
			horas = 0;
			sscanf( comandline, ";Print time: %d minutes", &minutos);
		}
		if(minutos ==0){
			minutos = 1;
		}
		//Serial.println("CURA me mola");
	}
	else if (simplify3D == 1){
		dias = 0;
		sscanf( comandline, ";   Build time: %d hours %d minutes", &horas, &minutos);
		if(minutos==-1){
			sscanf( comandline, ";   Build time: %d hour %d minutes", &horas, &minutos);
		}
		//Serial.println("simplify3D");
	}
	//Serial.println(comandline);
	
}
void Listfiles::extract_data1(void){
	//Serial.println(comandline);
	
	filgramos1 = 0;
	filgramos2 = 0;
	if(simplify3D == 0){
	sscanf(comandline, ";Filament used: %d.%dm %d.%dg", &filmetros1, &filmetros2, &filgramos1, &filgramos2);
	}
	else if ( simplify3D == 1){
		sscanf(comandline, ";   Plastic weight: %d.%dg", &filgramos1, &filgramos2);
	}
	//Serial.println(filmetros1);
	//Serial.println(comandline);
}
	
int Listfiles::get_hours(){
	int hours = 0;
	if(dias>0){
		hours = 24*dias + horas;
	}
	else{
		hours = horas;
	}
	return hours;
}
int Listfiles::get_minutes(){
	int mins = 0;
	mins = minutos;
	return mins;
}
int Listfiles::get_filmetros1(){
	int metr = 0;
	metr = filmetros1;
	return metr;
}
int Listfiles::get_filmetros2(){
	int metr = 0;
	metr = filmetros2;
	return metr;
}
int Listfiles::get_filgramos1(){
	int gram = 0;
	gram = filgramos1;
	return gram;
}
int Listfiles::get_filgramos2(){
	int gram = 0;
	gram = filgramos2;
	return gram;
}
int Listfiles::get_hoursremaning(){
	long hours = 0;
	if (get_hours()==0)return 0;
	hours = ((long)get_hours()*60+(long)get_minutes());
	hours = hours-hours*(long)card.getSdPosition()/(long)card.getFileSize();
	hours = hours/60;
	return (int) hours;
}
int Listfiles::get_minutesremanig(){
	long minu = 0;
	if (get_minutes()==-1)return 0;
	minu = ((long)get_hours()*60+(long)get_minutes());
	minu = minu-minu*(long)card.getSdPosition()/(long)card.getFileSize();
	minu = minu%60;
	return (int) minu;
}