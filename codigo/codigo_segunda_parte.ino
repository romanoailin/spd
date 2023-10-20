/*
En este circuito utilizamos 7 conexiones para conectar dos  displays a la placa de Arduino. Dichos displays están interconectados entre sí a través de la técnica de la multiplexación. 
Los negativos de los displays están conectados en los pines 2 y 3, de forma que se pueda mostrar números de manera
independiente en cada uno de ellos. Por ejemplo, si el display que se quiere encender es el definido como DECENAS, por el pin de salida del display definido como UNIDADES enviamos 5v para que la diferencia de potencial sea 0 y no circule corriente, y al de DECENAS lo dejamos en GND para que haya diferencia de potencial y la corriente circule. 
Alternamos el encendido y apagado de cada display con un delay muy corto que nos permita crear la falsa sensación de que están encendidos en simultaneo y mostrar los números que queramos.
Posee un Slideswitch que permite que el contador suba o baje entre numeros primos o de 1 en 1 dependiendo el estado en el que se encuentre. 
Posee un sensor de fuerza el cual, en el caso de llegar al máximo de fuerza posible, genera que se resetee el contador.
Posee un motor que gira hacia adelante o atrás dependiendo el modo en que se encuentre el contador.
*/

/* Define los segmentos de cada display con su pin correspondiente.*/
#define A 10
#define B 11
#define C 5
#define D 6
#define E 7
#define F 9
#define G 8
/* Define el positivo y negativo del motor.*/
#define ADELANTE 13
#define ATRAS 12
/*Se definen los terminales de cada display: UNIDADES y DECENAS a sus pines*/
#define UNIDADES 3
#define DECENAS 2
/*Se definen los pulsadores SUBIR y BAJAR a sus pines */
#define SUBIR A5
#define BAJAR A4
/*Se define cada lado del Slideswitch a sus pines */
#define D_IZQUIERDA A2
#define D_DERECHA A3
/*Define el sensor de fuerza a su pin*/
int FUERZA = A0;

void setup()
{
/*Configura pines correspondientes como salida*/
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(UNIDADES, OUTPUT);
  pinMode(DECENAS, OUTPUT);
  pinMode(ADELANTE, OUTPUT);
  pinMode(ATRAS, OUTPUT);
/*Configura los pulsadores, slideswitch y sensor como input PULLUP para que estén conectados a la resistencia interna de la placa*/
  pinMode(SUBIR, INPUT_PULLUP);
  pinMode(BAJAR, INPUT_PULLUP);
  pinMode(D_IZQUIERDA, INPUT_PULLUP);
  pinMode(D_DERECHA, INPUT_PULLUP);
  pinMode(FUERZA, INPUT_PULLUP);
}
/*Define como NO PRESIONADOS a los valores iniciales de los pulsadores*/
int subirAnterior = 1;
int bajarAnterior = 1;
/*Inicializa las variables unidades y decenas en 0*/
int unidades = 0;
int decenas = 0;
/*Inicializa el modo de números primos y el del estado deslizante en falso*/
bool modoPrimo = false;
bool estadoDeslizante = false;
/*Inicializa las variables lectura y estandar (para el sensor de fuerza) como int */
int lectura;
int estandar;

/*Enciende y apaga de manera alternativa cada display, con un delay de 50 milisegundos entre un display y el otro, mostrando los números que deseamos como si ambos estuviesen encendidos simultaneamente.*/
void loop()
{
  leeSensor(FUERZA);
  ManipularBotones();
  PrenderDisplay(UNIDADES, unidades);
  delay(50);
  PrenderDisplay(DECENAS, decenas);
  delay(50);
}
/*Lee el valor del sensor de fuerza y si llega a 1, resetea el contador*/
void leeSensor(int sensor)
{
  lectura = analogRead(FUERZA); //analogRead() lee el valor del sensor de fuerza,devuelve un valor máximo de 1023.
  estandar = map(lectura,1023,24,0,10); //mapea el min y máx del sensor a un valor de 0 a 10 respectivamente
  estandar = estandar / 10; //Toma el nuevo rango min y max (0 a 10) y lo divide entre diez para poder tener decimales del 0 al 1 teniendo en cuenta la fuerza aplicada
  if (estandar == 1) //Si la medida de fuerza llega al max (1), se resetea el contador
  {
    Resetear();
  }
    
}
/*Crea la función resetear para que reinicie el contador en 00*/
void Resetear()
{
  unidades = 0;
  decenas = 0;
}

/*Chequea el estado de los pulsadores utilizando las funciones Subir(), Bajar(). Toma la función deslizante() con dos parámetros dependiendo su estado. Actualiza los valores de las variables subirAnterior, bajarAnterior*/
void ManipularBotones()
{
  subirAnterior = Subir(SUBIR, subirAnterior, modoPrimo);
  bajarAnterior = Bajar(BAJAR, bajarAnterior, modoPrimo);
  deslizante(D_IZQUIERDA, D_DERECHA);
}

void deslizante(int izquierdo, int derecho)
{ //lee el estado del deslizante del switch
  int estadoIzq = digitalRead(izquierdo); 
  int estadoDer = digitalRead(derecho);
  //si el estado izq está prendido el contador va a ir a través de numeros pares y el motor se va a mover hacia atras
  if (estadoIzq == 0)
  {
    modoPrimo = true;
    digitalWrite(ATRAS, HIGH);
    digitalWrite(ADELANTE, LOW);
  }
  else
  { //de lo contrario, el contador irá de uno en uno y el motor se moverá hacia adelante
    modoPrimo = false;
    digitalWrite(ADELANTE, HIGH);
    digitalWrite(ATRAS, LOW);
  }
}

/*Aumenta el valor de las unidades o decenas cuando el pulsador es presionado. Si el valor de las unidades alcanza el valor de 10, unidades vuelve a valer cero y decenas aumenta un valor. La función retorna el estado del botón. Toma el parámetro modoPrimo para ver si se mueve a través de los primos o a través de todos los números del 0 al 99*/

int Subir(int pulsador, int estadoAntBoton, bool modoPrimo)
{
  int estadoBoton = digitalRead(pulsador); //define el estado del boton a través de la lectura del pulsador
  //si el boton es presionado y se encontraba apagado
  if (estadoBoton == 0 && estadoAntBoton == 1)
  {
    if (modoPrimo) {
      // Si el modoPrimo está activado, busca el siguiente número primo
      int num = decenas * 10 + unidades + 1; // Combinamos decenas y unidades para formar el número completo, se le suma 1 para saber el numero siguiente y corroborar
      while (num <= 99) { // Aseguramos que el número no sea mayor de 99
        if (EsPrimo(num)) { //le paso el número a la función es primo
          unidades = num % 10; // Unidades del número primo
          decenas = num / 10; // Decenas del número primo
          break;
        } 
        num++; //pasamos al siguiente número y vuelve al loop
      }
    } //si el modo primo está apagado
    else {
      unidades++; //unidades va a aumentar su valor en uno
      if (unidades == 10) //si unidades llega a diez, unidades vuelve a 0 y decenas aumenta en uno
      {
        unidades = 0;
        decenas++;
        if (decenas == 10) //si decenas llega a diez, vuelve a 0 (el contador queda en 00)
        {
          decenas = 0;
        }
      }
    }
  }
  return estadoBoton; //devuelve el estado nuevo del botón 
}

/*Disminuye el valor de las unidades o decenas cuando el pulsador es presionado. Si el valor de las unidades alcanza el valor de 0, unidades pasa a valer 9 y decenas disminuye un valor. Toma el parámetro modoPrimo para ver si se mueve a través de los primos o a través de todos los números del 0 al 99. La función retorna el estado del botón.*/
int Bajar(int pulsador, int estadoAntBoton, bool modoPrimo)
{
  int estadoBoton = digitalRead(pulsador);//define el estado del boton a través de la lectura del pulsador
  //si el boton es presionado y se encontraba apagado
  if (estadoBoton == 0 && estadoAntBoton == 1)
  { // Si el modoPrimo está activado, busca el número primo anterior
    if (modoPrimo) {
      //Combinamos decenas y unidades para formar el número completo,
      int num = decenas * 10 + unidades;
      //Definimos el numero que se le pasa como numero anterior(el mostrado en pantalla)
      int numAnterior = num;
      while (numAnterior > 2) { //mientras que el numero anterior sea mayor a 2 (primer numero primo)
        numAnterior--; //se resta 1 al número
        if (EsPrimo(numAnterior)) { //se chequea si ese numero es o no primo
          break;
        }
      }
      //Se establece el número actual en el número primo anterior
      unidades = numAnterior % 10;
      decenas = numAnterior / 10;
    } 
    else { //si modo primo no está activado se resta de uno en uno
      if (unidades == 0 && decenas == 0) {
//no restar si se encuentra en 00
      }
      else 
      {
      unidades--; //se resta una unidad
      if (unidades < 0) //si unidades es menor a 0
      {
        unidades = 9; //unidades vuelve a 9
        decenas--;  //se resta una decena
        if (decenas < 0)
        {
          decenas = 0;
        }
      }
    }
  }
  return estadoBoton;
  }
}

/*Función que toma un número y devuelve true si es primo, false si no lo es*/
bool EsPrimo(int num)
{
  if (num <= 1) 
  {
    return false; //retorna falso si el numero es 1 o menor
  } 
  else
  {
    int cont = 0; //inicializamos un contador para tener el número de divisores del número pasado por parámetro
    for (int i = 1; i <= num; i++) //Inicializa un bucle desde 1 hasta el número
  	{
      if (num % i == 0) //si el número pasado es divisible por el número por el que está pasando el bucle, el contador aumenta en 1
      {
        cont++;
      }   
  	}
    if (cont == 2) //si el contador es igual a 2, es decir, si el numero pasado se puede dividir por dos numeros, devuelve primo
    {
      return true;
    }
    else
    {
      return false; //caso contrario devuelve falso
    }
  }  
  
}

/*Enciende los segmentos del display pasado por parámetro. Utiliza la función MostrarNumero() para encender los segmentos correspondientes al número ingresado por parámetro*/
void PrenderDisplay(int display, int numero)
{
  if (display == UNIDADES)
  {
    digitalWrite(DECENAS, HIGH);
    digitalWrite(UNIDADES, LOW);
  }
  else if (display == DECENAS)
  {
    digitalWrite(DECENAS, LOW);
    digitalWrite(UNIDADES, HIGH);
  }
  else
  {
    digitalWrite(DECENAS, LOW);
    digitalWrite(UNIDADES, LOW);
  }
  MostrarNumero(numero);
}


/*Enciende los segmentos del display según el número pasado por parámetro. Utiliza la función ApagarDisplay() para apagar todos los segmentos antes de encender los segmentos correspondientes.*/
void MostrarNumero(int numero)
{
  ApagarDisplay();
  switch (numero)
  {
    case 0:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    break;
    case 1:
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    break;
    case 2:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(D, HIGH);
    break;
    case 3:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(G, HIGH);
    break;
    case 4:
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    break;
    case 5:
    digitalWrite(A, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    break;
    case 6:
    digitalWrite(A, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    break;
    case 7:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    break;
    case 8:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    break;
    case 9:
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    break;
  }
}


/*Apaga todos los segmentos del display*/
void ApagarDisplay()
{
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);	
}



