// Por Roberto A. Zavala
// Breve : https://es.overleaf.com/read/pfhcvtsjytbn
// Libro : https://www.amazon.com.mx/dp/B074TTGLL2
// 🙏🏼    : DNv7acPAeVBhTXbKv26itJecPG1SPy2o4F

const int N = 11;                                                      // 2n+1 de Savitzky Golay
const int m = N-1;
const int p = m-1;  

int   X [N];
int   X2[N];
int   X3[N];
int   X4[N];
float Y [N];
float XY[N];
float X2Y[N];
float S[7];

float SY;
float SXY;
float SX2Y;
float S3=0;

float I;                                                              // la medición
float ISG;                                                            // la suavización

void setup()
{
Serial.begin(9600);                                                   // se inicia comunicación UART

for ( int j=0; j<=m; j++ ) { X [j] = j;}                              // se construye un subconjunto de datos de prueba
for ( int j=0; j<=m; j++ ) { X2[j] = j*j;}
for ( int j=0; j<=m; j++ ) { X3[j] = j*j*j;}
for ( int j=0; j<=m; j++ ) { X4[j] = j*j*j*j;}
S[0] =   m * ( m + 1 ) / 2 ;                                          // la suma de n
S[1] = ( m * ( m + 1 ) * ( 2 * m + 1 ) ) / 6;                         // la suma de n^2
S[2] = S[0]*S[0] ;
S[3] = ( 6 * pow(m,5) + 15 * pow(m,4) + 10 * pow(m,3) - m ) / 30;     // la suma de n^3
for ( int i=0; i<m; i++) { Y[i] = analogRead(A0); delay(100);}        // se toman m mediciones
}

void loop()
{
I = analogRead(A0);
SG();
Serial.print(I); Serial.print(" ");  Serial.println(ISG);
delay(10);
}

void SG ()
{
  for ( int j=0; j<=p; j++) { Y[j] = Y[j+1]; }                          // se recorren los valores del vector Y
  Y[m] = I ;

  for ( int j=4; j<=6; j++ )  { S[j] = 0; }
  float   SY   = 0;
  float   SXY  = 0;
  float   SX2Y = 0;
  for ( int j=0; j<=m; j++ ) { XY [j] = X[j] * Y[j];
                               X2Y[j] = X[j] * X[j] * Y[j];    }
  for ( int j=0; j<=m; j++ ) { SY     = SY   + Y[j];           } S[4] = SY; 
  for ( int j=0; j<=m; j++ ) { SXY    = SXY  + X[j] * Y[j];    } S[5] = SXY;
  for ( int j=1; j<=m; j++ ) { SX2Y   = SX2Y + X[j]*X[j]*Y[j]; } S[6] = SX2Y;

  float M[3][4]={{  N , S[0], S[1], S[4] } ,
                { S[0], S[1], S[2], S[5] } ,
                { S[1], S[2], S[3], S[6] }};

  float F1 = M[1][0] / M[0][0];
  float F2 = M[2][0] / M[0][0];

  M[1][0] = M[0][0]*F1 - M[1][0];
  M[1][1] = M[0][1]*F1 - M[1][1];
  M[1][2] = M[0][2]*F1 - M[1][2];
  M[1][3] = M[0][3]*F1 - M[1][3];

  M[2][0] = M[0][0]*F2 - M[2][0];
  M[2][1] = M[0][1]*F2 - M[2][1];
  M[2][2] = M[0][2]*F2 - M[2][2];
  M[2][3] = M[0][3]*F2 - M[2][3];

  float F3 = M[2][1] / M[1][1];

  M[2][1]= M[1][1]*F3 - M[2][1];
  M[2][2]= M[1][2]*F3 - M[2][2];
  M[2][3]= M[1][3]*F3 - M[2][3];

  float S3 = M[2][3] / M[2][2];
  float S2 = ( M[1][3]-( M[1][2]*S3 ) ) / M[1][1];
  float S1 = ( M[0][3] - M[0][2]*S3 - M[0][1]*S2 ) / M[0][0];
  float Sol[3] = { S1 , S2 , S3 };

  ISG = Sol[0] + Sol[1] * (m/2) + Sol[2] * (m/2) * (m/2);
}
