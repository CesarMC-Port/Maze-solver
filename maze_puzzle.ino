#include <Arduino.h> // Asegúrate de tener esta línea

#define LED_ON  12
#define LED     13
#define BOTON    9
#define pwmi     3   //PWM LEFT MOTOR
#define izq1     5   //LEFT1 MOTOR
#define izq2     4   //LEFT2 MOTOR
#define pwmd     11  //PWM RIGHT MOTOR
#define der1     6   //RIGHT1 MOTOR
#define der2     7   //RIGHT2 MOTOR
int pines[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensores[8];
int digital[8];
int lectura_fondo[8];
int lectura_linea[8];
int umbral[8];
long int sumap, suma, pos, poslast, position;
unsigned long tiempoInicio = 0;
unsigned long tiempoGiro = 1000; 

/////////////datos de retardo por algoritmo de reloj/////////////
// unsigned long previousMillis = 0;   
// bool esCruz = false;

////////////////pid/////////////////////
float KP=0.8;//constante proporcional 0.18
float KD=6;//constante derivativa 4
float KI=0.002;//constante integral
int vel=200;//VELOCIDAD MÁXIMA DEL ROBOT MÁXIMA 255
int veladelante=190;//VELOCIDAD DEL FRENO DIRECCIÓN ADELANTE
int velatras=95;//VELOCIDAD DEL FRENO DIRECCIÓN ATRÁS
int tiempogiro=0;
int tiempogiro2=0;
int tiempogiro3=0;
int tiempoeleccion=0;
int controlTodos1=0;
int vel2=200;//VELOCIDAD MÁXIMA DEL ROBOT MÁXIMA 255
int veladelante2=190;//VELOCIDAD DEL FRENO DIRECCIÓN ADELANTE
int velatras2=95;//VELOCIDAD DEL FRENO DIRECCIÓN ATRÁS
int numTimeNode=0;
// int casoAnterior=0;
////////////////////////////////////////

/// datos para la integral
int error1=0;
int error2=0;
int error3=0;
int error4=0;
int error5=0;
int error6=0;
/////////////////////////

///////////variable PID///////////////
int proporcional=0;
int integral=0;
int derivativo=0;
int diferencial=0;
int last_prop;
int setpoint=350;
// int nodoActual = 0;
// int camino[100]; // Almacena el camino recorrido (máximo 100 nodos)
// int pasos = 0;   // Número de nodos recorridos
int contadorDerechas = 0;  // Contador de giros consecutivos a la derecha
int contadorIzquierdas = 0; // Contador temporal para izquierdas obligatorias
int tiempoLineas = 0;
int tiempoPasarLinea = 0;
int timeReset = 0;
String nodos = "";
String casoAnterior = "";
String casoPegado = "";
int casoAnteriorTimer = 0;

// Variables adicionales para la resolución del laberinto
// enum Nodo {T, CRUZ, META, NINGUNO};
// Nodo tipoNodo = NINGUNO;

String detectarNodo() {
  bool todos0 = true; 
  bool todos1 = true; 
  bool digitalCenter = false;
  for (int i = 0; i < 8; i++) {
    if (digital[i] != 0) todos0 = false;
    if (digital[i] != 1) todos1 = false;
  };

  digitalCenter = (digital[3] || digital[4]) 
  || (digital[3] || digital[4] || digital[5]) 
  || (digital[3] || digital[4] || digital[2]) 
  || (digital[3] || digital[2])
  || (digital[4] || digital[5]);

  if(digitalCenter && !digital[0] && !digital[7]){
    tiempogiro++;
    if(tiempogiro > 20){
      casoPegado = "";
      tiempogiro = 0;
    }
  }
  
  if(!todos0 && casoPegado == "IZQUIERDA"){casoPegado = "";}
  if(casoPegado != ""){return casoPegado;};

  casoAnteriorTimer++;
  if(digital[7] && digitalCenter && casoAnteriorTimer > 50 && !todos1){ 
    casoAnteriorTimer = 0;
    casoAnterior = "izquierda"; 
  };
  if(digitalCenter && digital[0] && casoAnteriorTimer > 50 && !todos1){
    casoAnteriorTimer = 0; 
    casoAnterior = "derecha"; 
  }
  if(todos1 && casoAnteriorTimer > 250){
    casoAnteriorTimer = 0;
    return "META";
  }
   
  // Serial.print("casoAnterior");
  // Serial.print("\t");
  // Serial.print(casoAnterior);

  if(todos0 && casoAnterior == ""){ return "GIRAR"; };
  if(((casoAnterior == "derecha" && digitalCenter) || casoAnterior == "todo") && (digitalCenter || todos0)){ 
    tiempoLineas = 0; return "DERECHA";
  };

  if(casoAnterior == "izquierda" && todos0){ 
    return "IZQUIERDA";
  };
  
  return "RECTO";
}



// Función para elegir un camino
void tomarDecision(String cases) {
  int velsd1 = nodos != "RECTO" ? veladelante2 : veladelante;
  int velsd2 = nodos != "RECTO" ? velatras2 : velatras;
  int porcentaje = 
  if(cases == "GIRAR"){ girar(); }else{ tiempogiro2 = 0; }
  if(cases =="DERECHA"){
    tiempoeleccion++;
    if(tiempoeleccion > 150){
       motores(180, -180);
      casoAnterior = "";
    }else{
      motores(velsd1*0.65,velsd1*0.65);
    }
  };
  if(cases =="IZQUIERDA"){
    tiempoeleccion++;
    if(tiempoeleccion > 150){
      motores(-180, 180);
      casoAnterior = "";
    }else{
      motores(velsd1,velsd1);
    }
  };
  if(cases == "META"){ detenerse(); };
}

void girar() {
  tiempogiro2++;
  int velss1 = nodos != "RECTO" ? veladelante2 : veladelante;
  int velss2 = nodos != "RECTO" ? velatras2 : velatras;
  if(tiempogiro2 > 320){
    motores(-180, 180); 
  }else{
    motores(0, -0); 
  }
}

void detenerse() {
  motores(0, 0);
  while (true) {
    digitalWrite(LED, HIGH); // Parpadeo para indicar meta
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}

void fondos(){
  for (int i = 0; i < 8; i++) {
    lectura_fondo[i]=analogRead(pines[i]);
  }
}

void lineas(){
  for (int i = 0; i < 8; i++) {
    lectura_linea[i]=analogRead(pines[i]);
  }
}

void promedio(){
  for(int i=0;i<8;i++){
    umbral[i]=(lectura_fondo[i]+lectura_linea[i])/2;
  }
}

int lectura(void){
  
  for (int i = 0; i < 8; i++) {
    sensores[i]=analogRead(pines[i]);
    if(sensores[i]<=umbral[i]){digital[i]=0;}else{digital[i]=1;};
    // Serial.print(digital[i]);
    // Serial.print("\t");
  }

  sumap=(700*digital[7]+600*digital[6]+500*digital[5]+400*digital[4]+300*digital[3]+200*digital[2]+100*digital[1]+0*digital[0]);
  suma=(digital[0]+digital[1]+digital[2]+digital[3]+digital[4]+digital[5]+digital[6]+digital[7]);
  pos=(sumap/suma);
  if(poslast<=100 && pos==-1){
    pos=0;
  }
  if(poslast>=600 && pos==-1){
    pos=700;
  }
  poslast=pos;
  // Serial.print(pos);
  // Serial.print("\t");
  // Serial.println(" "); 
  return pos;
}

void PID(){
  // Serial.print("posicion");
  // Serial.print("\t");
  // Serial.print(pos);
  int velselect = nodos != "RECTO" ? vel2 : vel;
  proporcional=pos-setpoint;
  derivativo=proporcional-last_prop;
  integral=error1+error2+error3+error4+error5+error6;
  last_prop=proporcional;
  error6=error5;
  error5=error4;
  error4=error3;
  error3=error2;
  error2=error1;
  error1=proporcional;
  int diferencial=(proporcional*KP) + (derivativo*KD) + (integral*KI);
  if(diferencial > velselect) diferencial=velselect;
  else if(diferencial < -velselect) diferencial=-velselect;
  
  // if(diferencial < 0){
  //   Serial.print("velocidad 1");Serial.print("\t");Serial.print(velselect);Serial.print("\t");Serial.print(velselect+diferencial);Serial.print("\t");
  // }else{
  //   Serial.print("velocidad 2");Serial.print("\t");Serial.print(velselect-diferencial);Serial.print("\t");Serial.print(velselect);Serial.print("\t");
  // }
  // Serial.println(" ");
  (diferencial < 0)?
  motores(velselect, velselect+diferencial):motores(velselect-diferencial, velselect);
}

void frenos(){
  int vels1 = nodos != "RECTO" ? veladelante2 : veladelante;
  int vels2 = nodos != "RECTO" ? velatras2 : velatras;
  if(pos<=100){
    motores(vels1, -vels2);
  }
  if(pos>=600){
    motores(-vels2, vels1);
  }
}

void motores(int izq, int der){//0 hasta 255    0 hasta -255
  ////////////////motor LEFT "IZQUIERDO" ////////////////////////

  if(izq>=0){
    digitalWrite(der1,LOW);
    digitalWrite(der2,HIGH);
  }
  else{
    digitalWrite(der1,HIGH);
    digitalWrite(der2,LOW);
    izq=izq*(-1);
  }
  analogWrite(pwmd,izq);
  
  ////////////////motor RIGHT "DERECHO" ////////////////////////
  if(der>=0){
    digitalWrite(izq1,HIGH);
    digitalWrite(izq2,LOW);
  }
  else{
    digitalWrite(izq1,LOW);
    digitalWrite(izq2,HIGH);
    der=der*(-1);
  }
  analogWrite(pwmi,der);
  
}

void setup() {
  Serial.begin(115200);
  //TCCR2B = TCCR2B & B11111000 | B00000001;
  //set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz

  //TCCR2B = TCCR2B & B11111000 | B00000010; 
  //set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz

  TCCR2B = TCCR2B & B11111000 | B00000011;   
  //set timer 2 divisor to    32 for PWM frequency of   980.39 Hz

  //TCCR2B = TCCR2B & B11111000 | B00000100;   
  //set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)

  //TCCR2B = TCCR2B & B11111000 | B00000101;   
  //set timer 2 divisor to   128 for PWM frequency of   245.10 Hz

  //TCCR2B = TCCR2B & B11111000 | B00000110;   
  //set timer 2 divisor to   256 for PWM frequency of   122.55 Hz

  //TCCR2B = TCCR2B & B11111000 | B00000111;   
  //set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
  pinMode(izq1,OUTPUT);
  pinMode(izq2,OUTPUT);
  pinMode(der1,OUTPUT);
  pinMode(der2,OUTPUT);
  for (int i = 0; i < 8; i++) {pinMode(pines[i], INPUT);}
  pinMode(LED,OUTPUT);
  pinMode(LED_ON,OUTPUT);
  pinMode(BOTON,INPUT);
  digitalWrite(LED_ON,1);
  digitalWrite(LED,1);
  delay(500);
  while(digitalRead(BOTON));
  for(int i=0;i<50;i++){
    fondos();
    digitalWrite(LED,0);
    delay(20);
    digitalWrite(LED,1);
    delay(20);
  }
  while(digitalRead(BOTON));
  for(int i=0;i<50;i++){
    lineas();
    digitalWrite(LED,0);
    delay(20);
    digitalWrite(LED,1);
    delay(20);
  }
  promedio();
  while(digitalRead(BOTON));
  digitalWrite(LED,0);
}

// Añadido en el loop
void loop() {
  while (true) {
    nodos = detectarNodo();
    if(nodos == "RECTO"){ frenos(); };
    lectura();
    // Serial.print(nodos);
    // Serial.println(" ");
    if (nodos != "RECTO") {
      casoPegado = nodos;
      timeReset=0;
      tomarDecision(nodos);
    }else{
      timeReset++;
      if(timeReset > 100){
        tiempoeleccion = 0;
        timeReset = 0;
        casoAnterior = "";
        casoPegado = "";
      }
      PID(); 
    }
  }
}