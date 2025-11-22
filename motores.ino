void inicializarMotores(){
  //inicializa los pin del motor izquierdo
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  //inicializa los pin del motor derecho
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
}
//si es que ese no es el caso igual podemos ponerle una mascara a value en vez de multiplicarlo por -1
void MotorIz(int value){
  if(value>=0){
    PORTB=PORTB&B11111101;
    PORTB=PORTB|B00000001;
  }
  else{
    //va en reverza
    PORTB=PORTB&B11111110;
    PORTB=PORTB|B00000010;
    value = (value ^ 0xFFFF) + 1;
  }
  analogWrite(PWMA,value);
}
//lo mismo aqui
void MotorDe(int value){
  if(value>=0){
    PORTD=PORTD&B01111111;
    PORTD=PORTD|B00010000;

  }
  else{
    PORTD=PORTD&B11101111;
    PORTD=PORTD|B10000000;

    value = (value ^ 0xFFFF) + 1; 
  }
  analogWrite(PWMB,value);
}
void Motores(int left, int right){
  MotorIz(left);
  MotorDe(right);
}
