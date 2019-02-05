#include <NewPing.h>
#include <Servo.h>

/*
  Pin 15: para el ULTRA SONIDO que dispara INODORO
  Pin 14: para el ULTRA SONIDO que recibe  INODORO
  Pin 17: para el BOTON que dispara INODORO
  Pin 16: para el BOTON que recibe INODORO
  Pin 4:  para el SERVO MOTOR 
  Pin A0: para el SENSOR DE AGUA INODORO
  --------------------------------------------------
  Pin 19: para el ULTRA SONIDO que dispara LAVAMANOS
  Pin 18: para el ULTRA SONIDO que recibe LAVAMANOS
  Pin 31: para la ELECTRO VALVULA
  Pin A1: para el SENSOR DE AGUA LAVAMANOS
  --------------------------------------------------
  Pin 45: para el ULTRA SONIDO que dispara PUERTA Fuera
  Pin 46: para el ULTRA SONIDO que recibe PUERTA Fuera
  Pin 50: para el ULTRA SONIDO que dispara PUERTA Adentro
  Pin 51: para el ULTRA SONIDO que recibe PUERTA Adentro  
  Pin 53: para el SERVO Puerta
*/

//Configuracion Ultra Sonido
#define dispara  15
#define recibe   14
#define distanciaMax 30
NewPing sonar(dispara, recibe, 10);
NewPing boton(17, 16, 10);

//Variable para el servo
Servo ServoInodoro;

//Inicialisacion de variables para el tiempo
long momentoDetectado = 0;
long tiempoServo = 0;
long tiempoMojado = 0;
long tiempoMojadoLavamanos = 0; 

//Flags
boolean flagUltraSonido = false;  //true si se detecto con el ultrasonido algo x mas de 5 segundos
boolean flagTiempo = true;        //true si no detecto nada y false si detecto
boolean flagServo = false;
boolean flagServo90 = true;
boolean flagMojadoInodoro = true;
boolean flagAlertAguaInodoro = false ;
boolean flagMojadoLavamanos = true;

//Configuracion Ultra Sonido
  #define disparaLavamanos  19
  #define recibeLavamanos   18
  NewPing sonarLavamanos(disparaLavamanos, recibeLavamanos, 10);
  
  //Configuracion variables Lavamanos
  int electrovalvula = 31; //EL PIN 31 PARA LA ELECTROVALVULA     //LAVAMANOS
  
  //Inicializacion de variables para el tiempo
  long tiempoDetectadoLavamanos = 0;          //LAVAMANOS

//--------------------------------------NIKI PUERTA----------------------------------

//----------------------------------------------------Configuracion Ultra Sonido Puerta de INGRESO--------------------------------------------------
#define disparaPuerta  45
#define recibePuerta   44
#define distanciaMaxPuerta 15
NewPing sonarPuerta(disparaPuerta, recibePuerta, distanciaMaxPuerta);

//----------------------------------------------------Configuracion Ultra Sonido Puerta de RETIRO---------------------------------------------------
                    //Codigo Nuevo!!!!
#define disparaPuertaRetiro  50
#define recibePuertaRetiro   51
#define distanciaMaxPuertaRetiro 15
NewPing sonarPuertaRetiro(disparaPuertaRetiro, recibePuertaRetiro, distanciaMaxPuertaRetiro);


//--------------------------------------------------------------Variable para el servo--------------------------------------------------------------
Servo ServoPuerta;

//-----------------------------------------------------Inicialisacion de variables para el tiempo---------------------------------------------------
long tiempoDetectadoPuerta = 0;
long tiempoServoPuerta = 0;
                                        //Codigo Nuevo!!!!
long tiempoDetectadoPuertaRetiro = 0;
long tiempoServoPuertaRetiro = 0;

//---------------------------------------------------------------------Flags------------------------------------------------------------------------
boolean flagUltraSonidoPuerta = false;
boolean flagTiempoPuerta = true;
boolean flagServoPuerta = false;
boolean flagServo90Puerta = true;
                                        //Codigo Nuevo!!!!
boolean flagUltraSonidoPuertaRetiro = false;
boolean flagTiempoPuertaRetiro = true;
boolean flagServoPuertaRetiro = false;
boolean flagServo90PuertaRetiro = true;
boolean flagPuertaRetiro = false;
boolean flagPuertaRetiroDentro = true;


void setup()
{
  Serial.begin(9600);
  ServoPuerta.attach(52);
  ServoPuerta.write(0);
  ServoInodoro.attach(4);
  ServoInodoro.write(0);
  pinMode(A0, INPUT); //Se define el pin A0 para el sensor de agua del inodoro
  pinMode(A1, INPUT); //Se define el pin A1 para el sensor de agua del lavamanos
  pinMode(electrovalvula, OUTPUT); //SE DEFINE EL PIN DE LA ELECTRVALVULA COMO SALIDA       //LAVAMANOS
}

void loop()
{ 
  Serial.print("El sensor de agua mide: ");
    Serial.println(analogRead(A0));
  delay(1000);
  
  if(flagAlertAguaInodoro)
  {
    Serial.println("ALERTA/ALERTA/ALERTA/ALERTA/ALERTA/ALERTA/ALERTA");
    apagado();
    ultrasonidoLavamanos();
    puerta();
  }
  else
  {
    ultraSonidoInodoro();
    servoInodoro();
    aguaInodoro();
    ultrasonidoLavamanos();
    puerta();
  }
  
}  

void ultraSonidoInodoro()
{
  if (detectar(1))                       //ingresa si detecta presencia
  {
    Serial.println("A");
    if (flagTiempo)                       //ingresa si es la primera vez que detecta presencia.
    {
      Serial.println("-------------------PRIMERA-------------------");
      flagTiempo = false;
      momentoDetectado = millis();         //guarda el momento cuando se detecta presencia x primera vez 
    }
    if (millis() > momentoDetectado + 5000)//pregunta si pasaron 5s desde que detecto presencia x primera vez
    {
      flagUltraSonido = true;
      Serial.println("------------------5 SEGUNDOS-----------------");
    }
  }
  else
  {
    flagTiempo = true;
    Serial.println("NO Detectado");
    if (flagUltraSonido)
    {
      Serial.println("MOVE EL CULO");
      flagUltraSonido = false;
      flagServo = true;
    }
  }
}

boolean detectar(int num)
{
  //devuelve true si detecta una precencia a 20 cm o menos
  int distancia = calcularDistancia(num);
    Serial.print("la distancia detectada es: ");
    Serial.println(distancia);
  if ( distancia <20 and distancia >= 5)
  {
    //Serial.println("DETECTADO");
    return true;
  }
  else
  {
    return false;
  }
}

int calcularDistancia(int num)
{
  //calcula el tiempo que tardo ir y venir la sonda y con eso se calcula la distancia
  int Tiempo;
  switch(num)
  {
    case 1: //ultraSonido inodoro
            Tiempo = sonar.ping_median();
            Serial.println("INODORO");
            break;
    case 2: //ultraSonido boton
            Serial.println("BOTON");
            Tiempo = boton.ping_median();
            break;
    case 3: //ultraSonido lavamanos
            Serial.println("LAVAMANOS");
            Tiempo = sonarLavamanos.ping_median();
            break;
  }
   int Distancia = Tiempo / US_ROUNDTRIP_CM;
   return Distancia;
}
void servoInodoro()
{
    if (flagServo)
  {
    if (flagServo90)
    {
      moverServo90();
    }
    if (millis() > tiempoServo + 2000)
    {
      moverServo0();
    }
  }
}

void moverServo90()
{
  Serial.println("MOVIDO 90");
  ServoInodoro.write(90);
  flagServo90 = false;
  tiempoServo = millis();
}

void moverServo0()
{
  Serial.println("MOVIDO 0");
  ServoInodoro.write(0);
  flagServo = false;
  flagServo90 = true;
}

void aguaInodoro()
{
  if(analogRead(A0)>250)
  {
    Serial.println("-----------------------------------------------------------------------------------------------------------------------------------");
    if(flagMojadoInodoro)
    {
      tiempoMojado = millis();
      Serial.println("------------------MOJADO-----------------");
      flagMojadoInodoro=false;
    }

    if(millis() > tiempoMojado+5000)
    {
      Serial.println("------------------ALERT------------------");
      flagAlertAguaInodoro = true;
      ServoInodoro.write(0);
    }
  }
  else
  {
    flagMojadoInodoro=true;
  }
}

void apagado()
{
  if(detectar(2))
  {
    Serial.println("Boton Activado");
    flagAlertAguaInodoro = false;
    delay(5000);
  }
}

//--------------------------------------------------------------------------------LAVAMANOS-----------------------------------------------------------------------------------------------



void ultrasonidoLavamanos()
{
  if(analogRead(A1)>250)
  {
    aguaLavamanos();
  }else
    {
      if(detectar(3))
        {
          apagarLavamanos();
        }
      else 
        {
          tiempoDetectadoLavamanos = activarLavamanos();
        }
    }

}

void aguaLavamanos()
{
    if(flagMojadoLavamanos)
    {
      tiempoMojadoLavamanos = millis();
      Serial.println("------------------MOJADO-----------------");
      flagMojadoLavamanos = false; 
    }

    if(millis() > tiempoMojadoLavamanos+5000)
    {
      Serial.println("------------------ALERT------------------");
      apagarLavamanos();
    } 
}

int activarLavamanos(){
  digitalWrite(electrovalvula, HIGH);
  Serial.println("prendemos lavamanos");  
  return millis();
}
  
void apagarLavamanos(){
  digitalWrite(electrovalvula, LOW);
  Serial.println("apagamos lavamanos");
 }

 //--------------------------------------------PUERTA------------------------------------
 void puerta()
{
  if (detectarPuertaIngreso())                                                                    //ingresa si detecta presencia
  {
   if (flagTiempoPuerta)                                                                          //ingresa si es la primera vez que detecta presencia
   {
     flagTiempoPuerta = false;                                                                    //Cambia a false para que no entre la segunda vez que se detecta
     tiempoDetectadoPuerta = millis();                                                            //Empieza a correr el tiempo mientras se este detectando
   }
   if (millis() > tiempoDetectadoPuerta + 3500)                                                   //Cuando tiempo llegue a 3,5 segundos
   {
     flagUltraSonidoPuerta = true;  
     flagServoPuerta = true;                                                         
   }  
  }
  else if(detectarPuertaRetiro())   //CODIGO NUEVO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {
   if (flagTiempoPuertaRetiro)                                                                    //ingresa si es la primera vez que detecta presencia
   {
    flagTiempoPuertaRetiro = false;                                                               //Cambia a false para que no entre la segunda vez que se detecta
    tiempoDetectadoPuertaRetiro = millis();                                                       //Empieza a correr el tiempo mientras se este detectando
   }
   if (millis() > tiempoDetectadoPuertaRetiro + 3500)                                             //Cuando tiempo llegue a 3,5 segundos
   {
    flagUltraSonidoPuertaRetiro = true;                                                           //El flagUltraSonidoPuertaRetiro es True    
   }  
  }
  else
  {
    flagTiempoPuerta = true;                                                                      
  }
  
  while(flagUltraSonidoPuertaRetiro){                                                             //Cuando el flagUltraSonidoPuertaRetiro sea True va a entrar
    Serial.println("Entro al While de Puerta");
    flagTiempoPuertaRetiro = true;                                                                      
    if (flagUltraSonidoPuertaRetiro)                                                                    
    {
      flagUltraSonidoPuertaRetiro = false;                                                              //Cambia el flagUltraSonidoPuertaRetiro a False para que no entre otra vez  
      flagServoPuertaRetiro = true;                                                                     
    }
  }
  
  if (flagServoPuerta)                                                                            //Cuando el flag sea true va a entrar
  {
    if (flagServo90Puerta)
    {
      moverServo90Puerta();                                                                       //Mueve el Servo unos 90º
    }
    if (millis() > tiempoServoPuerta + 5500)                                                      //Espera unos 5,5 segundos y despues cierra la puerta
    {
      moverServo0Puerta();                                                                        //Mueve el Servo a la posicion inicial 0°
    }
  }
  else if(flagServoPuertaRetiro)      //CODIGO NUEVO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {
     if (flagServo90PuertaRetiro)
    {
      moverServo90PuertaRetiro();                                                                 //Mueve el Servo unos 90º
    }
    if (millis() > tiempoServoPuertaRetiro + 5500)                                                //Espera unos 3,5 segundos y despues cierra la puerta
    {
      moverServo0PuertaRetiro();                                                                  //Mueve el Servo a la posicion inicial 0°
    }
  }
}


boolean detectarPuertaIngreso()                                                                   //Devuelve true si se detecta una presencia a 100cm o menos
{
                                      //CODIGO NUEVO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if(flagPuertaRetiroDentro){                                                                     //Si hay alguien dentro del baño la puerta no se puede abrir desde afuera
     int distanciaPuerta = calcularDistanciaPuerta();
    if ( distanciaPuerta <= 25  and  distanciaPuerta >=5 )
    {
      Serial.println(distanciaPuerta);
      Serial.println("Detecta AFUERA");
      return true;
    }
    else
    {
      Serial.println("NO Detecta AFUERA");
      return false;
    }
  }
  return false;
} 

int calcularDistanciaPuerta()                                                                    //Calcula el tiempo que tarde en ir y volver la sonda y asi calcula la distancia
{
  int TiempoPuerta = sonarPuerta.ping_median();
  int DistanciaPuerta = TiempoPuerta / US_ROUNDTRIP_CM;
  return DistanciaPuerta;
}

void moverServo90Puerta()                                                                        //Mueve al Servo en 90°
{
  Serial.println("Se abrio la puerta desde AFUERA");
  ServoPuerta.write(90);
  flagServo90Puerta = false;
  tiempoServoPuerta = millis();
}

void moverServo0Puerta()                                                                         //Mueve al Servo a 0°
{
  Serial.println("Se cerro la puerta desde AFUERA");
  ServoPuerta.write(0);
  flagServoPuerta = false;
  flagServo90Puerta = true; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ALERT
}



//----------------------------------------------------------------------      C O D I G O  N U E V O      ----------------------------------------------------------------------      


int calcularDistanciaPuertaRetiro()                                                              //Calcula el tiempo que tarde en ir y volver la sonda y asi calcula la distancia
{
  int TiempoPuertaRetiro = sonarPuertaRetiro.ping_median();
  int DistanciaPuertaRetiro = TiempoPuertaRetiro / US_ROUNDTRIP_CM;
  return DistanciaPuertaRetiro;
}
     
void moverServo0PuertaRetiro()                                                                   //Mueve al Servo en 0°
{
  Serial.println("Se cerro la puerta desde ADENTRO");
  ServoPuerta.write(0);
  flagServoPuertaRetiro = false;
  flagServo90PuertaRetiro = true;
  flagServoPuerta = false;
}

void moverServo90PuertaRetiro()                                                                  //Mueve al Servo en 90°
{
  Serial.println("Se abrio la puerta desde ADENTRO");
  ServoPuerta.write(90);
  flagServo90PuertaRetiro = false;
  tiempoServoPuertaRetiro = millis();
}

boolean detectarPuertaRetiro()                                                                   //Si la ditancia detectada es igual o menor a 100 cm devuelve true
{
  int distanciaPuertaRetiro = calcularDistanciaPuertaRetiro();                                   //Calcula el tiempo que tarde en ir y volver la sonda y asi calcula la distancia, y la guarda en la variable
  if ( distanciaPuertaRetiro <= 25  and  distanciaPuertaRetiro >=5)                               //Verifica si la distancia es igual o menor a 100 cm
  {
    Serial.println(distanciaPuertaRetiro);
    Serial.println("Detecta ADENTRO");
    return true;
  }
  else
  {
    Serial.println("NO Detecta ADENTRO");
    return false;
  }
}
