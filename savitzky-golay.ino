// Por Roberto A. Zavala
// Libro :     https://www.amazon.com.mx/dp/B074TTGLL2
// Tienda:     https://www.facebook.com/LaTeXnomicon/shop_tab
// X :         @EditorialQuiah
// QuiahEditorial R
// 🙏🏼 DogeTip: DNv7acPAeVBhTXbKv26itJecPG1SPy2o4F

// Software genérico para espectrómetro V 1.1

String comando;
String muestraa;

float Pd = 0;            // Puerto analógico foto diodo
float I;                 // Intensidad foto diodo
float Ip = 10;           // Itensidades a promediar
float Ti = 100;          // timepo de integración

int   Pf=2;              // Puerto fotopuerta
int   Sf;                // Señal fotopuerta

int   A = 6;             // Puerto digital motor Rejilla
int   B = 9;
int   C = 10;
int   D = 11;
bool  Dir;               // Dirección
int   d   = 2;           // delay
int   de  = d;           // delay de control
float Stp = 0.19125;     // pasos
float TP = Stp;

float Li;                // lambda inicial
float Lf;                // lambda final
float La;                // lambda actual
int   Lmin = 11.72;      // lambda mínima
int   Lmax = 1000;       // lambda máxima


int   i;                 // contadores
int   j;
float k=1;
int   K=1;
int   n;
int   q;
int   e;

float Z;             // parse

int Suave[31]=
{
 255 , // estos valores se guardan previamente en EEPROM
 255 ,
 254 ,
 252 ,
 249 ,
 246 ,
 243 ,
 238 ,
 233 ,
 227 ,
 221 ,
 214 ,
 206 ,
 198 ,
 190 ,
 180 ,
 171 ,
 160 ,
 150 ,
 139 ,
 128 ,
 116 ,
 104 ,
 91  ,
 79  ,
 66  ,
 53  ,
 40  ,
 27  ,
 13  ,
 0
};

void setup()
{
Serial.begin(1000000);
pinMode(A, OUTPUT);
pinMode(B, OUTPUT);
pinMode(C, OUTPUT);
pinMode(D, OUTPUT);
// return ( A==0 ) ? cero() : ayuda();

Serial.println("\nBienvenido :)\n\nEspectrómetro con Arduino\n\nArduino Para El Trabajo Científico Espectrómetro v1.1\n\nIngresa 'Ayuda'");
}

void loop()
{
if ( Serial.available() > 0 )
  {
  comando = Serial.readStringUntil('\n');
  comando.trim();
  if      ( comando.equals("Cero")  ) cero();
  else if ( comando.equals("Ayuda")  ) ayuda();
  else if ( comando.equals("Rango")  ) rango();
  else if ( comando.equals("Medir")  ) medir();
  else if ( comando.equals("TI")     ) ti();
  else if ( comando.equals("TP")     ) tp();
  else if ( comando.equals("Muestra")) smpl();
  else if ( comando.equals("Info")   )  info();
  }
delay(300);
}

void ayuda()
{
Serial.println("\nAyuda\nCero    : Reinicio\nRango   : Define rango\nMedir   : Mide espectro\nTI      : Tiempo integración\nTP      : Paso\nMuestra : Nombrar espectro\nInfo    : Estado actual");
Serial.println("Gráfica : https://www.overleaf.com/read/jtbrrwyrvfmk\nSoporte : Arduino Para El Trabajo Científico\nGNU3.0");
return;
}

void cero()  // ======================================================================================== CERO
{
Serial.println("\n Iniciando Spectronic 20 Genesys... ");
Sf = analogRead(Pf);   if      ( Sf >= 150 ) { dex(); }  else if ( Sf <  150 ) { set(); }
for ( n = 13448; n>=1; n--) { Dir=0;Pas(); delay(d);}    La = Lmin; // ventana 13548 -- 13348
Serial.println(" Listo");
}

void dex () { while ( Sf>= 300 ) { Dir=1;Pas(); Sf= analogRead(Pf); delay(d); } delay(1000); set(); }

void set ()
{
d  = 50; while ( Sf < 500 ) { Dir=0;Pas(); Sf= analogRead(Pf); delay(d);}
Li = 0;
Lf = 0;
Ti = 10;
d  = de;
}

void rango()// ======================================================================================== RANGO
{                                                                            // 400 : 400 / 650 : 641.5
Serial.print("\n Lambda inicial = ");    leer();    Li=Z;
if (Lmin>Li||Li>=Lmax) {Serial.print(Lmin); Serial.print(" <= Lambda <");  Serial.println(Lmax); return;}
Serial.print(Li);  Serial.println(" nm");
if ( Li < La ) { while (Li<La) { Dir=0;Pas(); } }    else if ( Li > La ) { while (Li>La) { Dir=1;Pas(); } }
final:
leer();    Lf=Z;    Serial.print(" Lambda final   = ");    leer();    Lf=Z;
if (Lf<=Li||Lf>Lmax)   {Serial.print(Li);   Serial.print(" < Lambda <= "); Serial.println(Lmax); goto final;}
Serial.print(Lf); Serial.print(" nm ");
}

void leer() { while (Serial.available()==0){} Z=Serial.parseFloat();}

void medir() // ======================================================================================= MEDIR
{
if ( Li<=Lmin || Lf<=Li ) { Serial.println("\n Ingresa: lambda inicial + lambda final" ); return; }
else
  {
  Serial.println("\n% nm \t I");
  while ( La<=Lf ) 
    {I=0; Medir();
     Serial.print(La,2);  Serial.print("\t");  Serial.println(I, 4);
     for ( int e = 1; e <= K; e++ ) { Dir=1;Pas(); delay(5); } }
while ( La > Li ) { Dir=0;Pas(); }
  }
}

void Medir() { for ( j=1; j<=Ti; j++) { I += analogRead(Pd)/819.0; delay(1); } I = I / Ti; }

void ti() // ========================================================================== TIEMPO DE INTEGRACIÓN
{
Serial.print("\n TI Actual      = "); Serial.print(Ti);  Serial.print(" ms    | Nuevo TI = ");  leer();
if (Z<1) {Ti=1; Serial.print(Ti);  Serial.print(" ms");  Serial.print(" + 1 <= Ti <= 5000");}
else     {Ti=Z; Serial.print(Ti);  Serial.print(" ms");}
}

void tp()  // ========================================================================================== PASO
{
Serial.print("\n Paso Actual    = ");  Serial.print(TP); Serial.print(" nm | Nuevo Paso = "); leer();
if (Z<Stp) {TP=Stp; Serial.print(TP); Serial.print(" + 0.19025 <= Paso <= "); Serial.println(Lf-Li);}
else       {TP=Z;   Serial.print(TP); Serial.print(" nm");}
k = TP / 0.19025;
K = k;
}

void smpl() // ====================================================================================== MUESTRA
{
Serial.print("\n Muestra        = "); while( Serial.available() == 0 ){} muestraa = Serial.readString(); 
Serial.print(muestraa);
}

void info()
{
Serial.print("\nRango : ");  Serial.print(Li);  Serial.print(" : ");  Serial.println(Lf);
Serial.print("Paso : ");  Serial.println(TP);
Serial.print("Tiempo de integración: ");  Serial.println(Ti);
Serial.print("Posición actual : ");  Serial.print(La);
}

void Pas ()  // ======================================================================================= PASOS
{
if (Dir==1) { La += Stp; i++; n++;} else { La -= Stp; i--; n--;}
if ( i > 119 ) { i = 0; } if ( i < 0 ) { i = 119; }
if (i>89)
   {
   j = i-90; //Serial.print(i); Serial.print("\t"); Serial.println(j);
   analogWrite(A, Suave[30-j]);
   analogWrite(B, 0);
   analogWrite(C, 0);
   analogWrite(D, Suave[j]);
   }
else if ( i>59 )
  {
  j = i-60; //Serial.print(i); Serial.print("\t"); Serial.println(j);
  analogWrite(A, 0);
  analogWrite(B, 0);
  analogWrite(C, Suave[j]);
  analogWrite(D, Suave[30-j]);
  }
else if (i>29)
  {
  j = i-30; //Serial.print(i); Serial.print("\t"); Serial.println(j);
  analogWrite(A, 0);
  analogWrite(B, Suave[j]);
  analogWrite(C, Suave[30-j]);
  analogWrite(D, 0);
  }
else
  {         //Serial.println(i);
  analogWrite(A, Suave[j]);
  analogWrite(B, Suave[30-j]);
  analogWrite(C, 0);
  analogWrite(D, 0);
  }
  delay(d);
}
