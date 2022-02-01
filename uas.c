#include <Blynk.h>                                  //memanggil library blynk
#define BLYNK_PRINT Serial                          //Untuk serial tampilan di aplikasi
#include <ESP8266WiFi.h>                            //Penggunaan ESP8266Wifi untuk terhubung internet
#include <BlynkSimpleEsp8266.h>                     //Library Esp8266
char auth[] = "aOkKYts_XsX6VOQzvYXYrkrkc0HyTLpJ"; `//kode dari aplikasi blynk ke email yang didaftarkan agar terhubung
char ssid[] = "Imam";     //nama wifi yang dipakai
char pass[] = "12345678"; //password wifi yang dipakai

WidgetLCD lcd1(V1);       //untuk menampilkan LCD di blynk lewat pin Virtual1

#define sensorPIR D6   // Vout sensor PIR

#define trig D8       //pin triiger sensor ultrasonic
#define echo  D7//pin echo sensor ultrasonik
#define pompa D3  //pin D3 nodmcu untuk pompa
#define kipas D2//pin D2 nodmcu untuk kipas
#define alarm D1//pin D1 nodmcu untuk alarm
#define lampu D0//pin D0 nodmcu untuk lampu
int durasi, jarak; //inisialisasi durasi dan jarak dengan int untuk sensor ultrasonik
int data = LOW;  // inisialisasi data low agar sensor PIR awalnya low
void setup(){
  pinMode (trig, OUTPUT);//inisialisasi pin triger sebagai input
  pinMode (echo, INPUT);//inisialisasi pin echo sebagai output
  pinMode(sensorPIR, INPUT);//inisialisasi pin PIR sebagai input
  pinMode (pompa, OUTPUT);//inisialisasi pompa sebagai output
  pinMode (kipas, OUTPUT);//inisialisasi kipas sebagai output
  pinMode (alarm, OUTPUT);//inisialisasi alarm sebagai output
  pinMode (lampu, OUTPUT);//inisialisasi lampu sebagai output
  digitalWrite (pompa, HIGH);// kondisi awal pompa mati (relay aktif low)
  digitalWrite (kipas, HIGH);// kondisi awal kipas mati (relay aktif low)
  digitalWrite (lampu, HIGH);// kondisi awal lampu mati (relay aktif low)
  digitalWrite (alarm, HIGH);// kondisi awal alarm mati (relay aktif low)
  Serial.begin(9600);// memulai serial
  Blynk.begin(auth, ssid, pass);
}
void loop(){
  lcd1.clear();//membersihkan LCD tahap awal
  lcd1.print(0,0,"jarak :");// print kata jarak di LCD
  digitalWrite(trig, LOW);//inisialisasi awal pin trigger LOW
  delayMicroseconds(8);// jeda

  digitalWrite(trig, HIGH);//inisialisasi pin trigger high mengirim sinyal untuk ketinggian air
  delayMicroseconds(8);//jeda
  digitalWrite(trig, LOW);//inisialisasi pin trigger low berhenti mengirim sinyal untuk ketinggian air
  durasi = pulseIn(echo, HIGH);//keterangan lama pengukuran, apabila echo menerima sinya HIGH
  jarak = durasi /29 / 2;// rumus untuk menghitung jarak menjadi "cm"
  Serial.print(jarak);//print jarak pada LCD
  Serial.print(" cm");//print "cm" pada LCD
  lcd1.print(8, 0, jarak);//posisi print jarak
  lcd1.print(12, 0, " Cm");//posisi print cm
  if (jarak <= 5){  //jika jarak kurang dari 5 cm
    delay(200);
    digitalWrite (pompa, HIGH);// maka pompa mati
  }
  else if (jarak >= 17){ //lalu jika jarak melebihi 5cm 
    delay(200);
    digitalWrite (pompa, LOW);//maka pompa nyala
  }
  Blynk.virtualWrite(V5, jarak);//menampilkan ketinggian pada aplikasi
  delay (200);
int sensorValue = digitalRead (sensorPIR);//inisisalisai sensor pir
Blynk.virtualWrite (V0, sensorValue);// menampilkan logika sensor pada aplikasi

data = digitalRead(sensorPIR); // baca input dr Vout
if (data == HIGH)  {  // cek jika ada pergerakan
        Blynk.notify("ada penyusup"); //buat pemberitahuan ke HP
         delay (1000);  
 }
  Blynk.run();    //aplikasi blynk berjalan
}
