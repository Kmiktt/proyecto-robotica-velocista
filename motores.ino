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
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
  }
  else{
    //va en reverza
    digitalWrite(AIN2,LOW);
    digitalWrite(AIN1,HIGH);
    value = (value ^ 0xFFFF) + 1;
  }
  analogWrite(PWMA,value);
}
//lo mismo aqui
void MotorDe(int value){
  if(value>=0){
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,HIGH);

  }
  else{
    digitalWrite(BIN2,LOW);
    digitalWrite(BIN1,HIGH);

    value = (value ^ 0xFFFF) + 1; 
  }
  analogWrite(PWMB,value);
}
void Motores(int left, int right){
  MotorIz(left);
  MotorDe(right);
}
