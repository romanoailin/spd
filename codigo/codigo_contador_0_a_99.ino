/*
En este circuito utilizamos 7 conexiones para conectar dos 
displays a la placa de Arduino. Dichos displays están interconectados
entre sí a través de la técnica de la multiplexación. 
Los negativos de los displays están conectados 
en los pines 2 y 3, de forma que se pueda mostrar números de manera
independiente en cada uno de ellos. Por ejemplo, si el display que 
se quiere encender es el definido como DECENAS, por el pin de salida 
del display definido como UNIDADES enviamos 5v para que la diferencia 
de potencial sea 0 y no circule corriente, y al de DECENAS lo dejamos
en GND para que haya diferencia de potencial y la corriente circule. 
Alternamos el encendido y apagado de cada display con un delay muy corto
que nos permita crear la falsa sensación de que están encendidos 
en simultaneo y mostrar los números que queramos.

*/

/* Define los segmentos A de cada display con el pin 10...*/
#define A 10
#define B 11
#define C 5
#define D 6
#define E 7
#define F 9
#define G 8

/*Se definen los displays UNIDADES y DECENAS
con los pines 3 y 2 respectivamente*/
#define UNIDADES 3
#define DECENAS 2

/*Se definen los pulsadores SUBIR, BAJAR y RESETEAR
con los pines A5, A4 y A3 respectivamente*/
#define SUBIR A5
#define BAJAR A4
#define RESETEAR A3

void setup()
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(UNIDADES, OUTPUT);
  pinMode(DECENAS, OUTPUT);
  
  /*Utilizamos la configuración PULLUP para los pulsadores, 
  de manera que estén conectados a la resistencia interna de la placa*/
  pinMode(SUBIR, INPUT_PULLUP);
  pinMode(BAJAR, INPUT_PULLUP);
  pinMode(RESETEAR, INPUT_PULLUP);
}


/*Define como NO PRESIONADOS a los valores iniciales
de estas tres variables*/
int subirAnterior = 1;
int bajarAnterior = 1;
int resetearAnterior = 1;

int unidades = 0;
int decenas = 0;


/*Enciende y apaga de manera alternativa cada display, 
con un delay de 50 milisegundos entre un display y el otro,
mostrando los números que deseamos como si ambos estuviesen encendidos
simultaneamente.*/
void loop()
{
  ManipularBotones();
  PrenderDisplay(UNIDADES, unidades);
  delay(50);
  PrenderDisplay(DECENAS, decenas);
  delay(50);
}


/*Chequea el estado de los pulsadores utilizando las funciones
Subir(), Bajar() y Resetear(). Actualiza los valores de las variables
subirAnterior, bajarAnterior y resetearAnterior.*/
void ManipularBotones()
{
  subirAnterior = Subir(SUBIR, subirAnterior);
  bajarAnterior = Bajar(BAJAR, bajarAnterior);
  resetearAnterior = Resetear(RESETEAR, resetearAnterior);
}


/*Aumenta el valor de las unidades o decenas de 1 en 1
cuando el pulsador es presionado. Si el valor de las unidades
alcanza el valor de 10, unidades vuelve a valer cero
y decenas aumenta un valor. 
La función retorna el estado del botón*/
int Subir(int pulsador, int estadoAntBoton)
{
  int estadoBoton = digitalRead(pulsador);
  int cambio = !estadoBoton && estadoAntBoton;
  if (cambio)
  {
    unidades++;
    if (unidades == 10)
    {
      unidades = 0;
      decenas++;
      if (decenas == 10)
      {
        decenas = 0;
      }
    }
  }
  return estadoBoton;
}


/*Disminuye el valor de las unidades o decenas de 1 en 1 
cuando el pulsador es presionado. Si el valor de las unidades
alcanza el valor de 0, unidades pasa a valer 9 y 
decenas disminuye un valor. 
La función retorna el estado del botón*/
int Bajar(int pulsador, int estadoAntBoton)
{
  int estadoBoton = digitalRead(pulsador);
  int cambio = !estadoBoton && estadoAntBoton;
  if (cambio) //Si presioné, entonces ejecutame.
  {
    // Añade una verificación aquí para ver si ambas variables son 0
    if (unidades == 0 && decenas == 0) 
    {
      // No hacer nada si ambas son 0
    }
    else 
    {
      unidades--;
      if (unidades < 0)
      {
        unidades = 9;
        decenas--;
        if (decenas < 0)
        {
          decenas = 0;
        }
      }
    }
  }
  return estadoBoton;
}


/*Reinicia los valores a cero de unidades y decenas 
cuando el pulsador es presionado.
La función retorna el estado del botón*/
int Resetear(int pulsador, int estadoAntBoton)
{
  int estadoBoton = digitalRead(pulsador);
  int cambio = !estadoBoton && estadoAntBoton;
  if (cambio)
  {
    unidades = 0;
    decenas = 0;
  }
  return estadoBoton;
}


/*Enciende los segmentos del display pasado por parámetro.
Utiliza la función MostrarNumero() para encender los
segmentos correspondientes al número ingresado por parámetro*/
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


/*Enciende los segmentos del display según el número pasado por parámetro.
Utiliza la función ApagarDisplay() para apagar todos los segmentos
antes de encender los segmentos correspondientes.*/
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








