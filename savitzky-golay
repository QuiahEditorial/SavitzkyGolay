// Se calcula por mínimos cuadrados la parábola que mejor se ajusta a los datos Y_1 a Y_5 =========================

int n = 5;
float Fi;
float Ft;

int X [5] = { 0, 1, 2, 3, 4 };
int X2[5] = { 0, 1, 4, 9, 16 };
int X3[5] = { 0, 1, 8, 27, 64 };
int X4[5] = { 0, 1, 16, 81, 256 };
int Y [5];

void setup()
{
  Serial.begin(9600);
// Realiza n+1 mediciones iniciales para aplicar SG desde el primer punto =========================================
  for ( int i=0; i<=4; i++ ) { Y[i] = analogRead(A0); }
}

void loop()
{
// Recorre el valor de los primeros valores para dar lugar a un quinto valor nuevo ================================
  for ( int i=0; i<=3; i++ ) { Y[i] = Y[i+1]; }
  Y[4] = analogRead(A0);

// Resuelve el sistema de ecuaciones ==============================================================================
  float XY[5] = { X[0]*Y[0], X[1]*Y[1], X[2]*Y[2], X[3]*Y[3], X[4]*Y[4] };
  float X2Y[5]= { X[0]*X[0]*Y[0], X[1]*X[1]*Y[1], X[2]*X[2]*Y[1], X[3]*X[3]*Y[3], X[4]*X[4]*Y[4] };
  float S [7] = { X[0]+X[1]+X[2]+X[3]+X[4], 
                  X[0]*X[0]+X[1]*X[1]+X[2]*X[2]+X[3]*X[3]+X[4]*X[4], 
                  X[0]*X[0]*X[0]+X[1]*X[1]*X[1]+X[2]*X[2]*X[2]+X[3]*X[3]*X[3]+X[4]*X[4]*X[4], 
                  X[0]*X[0]*X[0]*X[0]+X[1]*X[1]*X[1]*X[1]+X[2]*X[2]*X[2]*X[2]+X[3]*X[3]*X[3]*X[3]+X[4]*X[4]*X[4]*X[4], 
                  Y[0]+Y[1]+Y[2]+Y[3]+Y[4],
                  X[0]*Y[0]+X[1]*Y[1]+X[2]*Y[2]+X[3]*Y[3]+X[4]*Y[4],
                  X[0]*X[0]*Y[0]+X[1]*X[1]*Y[1]+X[2]*X[2]*Y[2]+X[3]*X[3]*Y[3]+X[4]*X[4]*Y[4], 
                };
  float M[3][4]={ { n , S[0], S[1], S[4] } ,
                { S[0], S[1], S[2], S[5] } ,
                { S[1], S[2], S[3], S[6] }
                };
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

  int F3 = M[2][2] / M[1][2];

  M[2][1]= M[1][1]*F3 - M[2][1];
  M[2][2]= M[1][2]*F3 - M[2][2];
  M[2][3]= M[1][3]*F3 - M[2][3];

  float S3 = M[2][3] / M[2][2];
  float S2 = ( M[1][3]-( M[1][2]*S3 ) ) / M[1][1];
  float S1 = ( M[0][3] - M[0][2]*S3 - M[0][1]*S2 ) / M[0][0];
  float Sol[3] = { S1 , S2 , S3 };

  Serial.print(Sol[0]);
  Serial.print("  ");
  Serial.print(Sol[1]);
  Serial.print("  ");
  Serial.println(Sol[2]);

  Serial.print( " f(t) = ");
  Serial.print( Sol[0]);
  Serial.print( " + ");
  Serial.print( Sol[1]);
  Serial.print( " t + ");
  Serial.print( Sol[2]);
  Serial.println( " t^2 ");

// Calcula f(t)

  Ft = Sol[0] + Sol[1] * 2 + Sol[2] *4;
  Serial.println(Ft);
  
delay(10000);
}
