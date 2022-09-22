/*
 *Button_bot 
 * 2022.9.14
 * H.kamano
 * R.onishi
 * H.shinohara
 */

// ピン番号をマクロで定義
#define TRIG_PIN1 16 // トリガー（送信）
#define ECHO_PIN1 17 // エコー（受信）
#define TRIG_PIN2 22 // トリガー（送信）
#define ECHO_PIN2 21 // エコー（受信）
double duration1 = 0; // 受信までの時間
double distance1 = 0; // 物との距離
double duration2 = 0; // 受信までの時間
double distance2 = 0; // 物との距離

#include <ESP32Servo.h>

Servo servo1; // create four servo objects 
int servo1Pin = 5;

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2400;

int pos = 0;      // position in degrees
int maxdist = 30;
double analogValue = 0;



// 起動時に一度だけ呼び出されるメソッド
void setup() {
 Serial.begin(115200); // シリアル通信の転送レート（bps）を設定
 
 // ピンの入出力設定
 pinMode(TRIG_PIN1, OUTPUT);
 pinMode(TRIG_PIN2, OUTPUT);
 
 pinMode(ECHO_PIN1, INPUT);
 pinMode(ECHO_PIN2, INPUT);
 
 // トリガーを LOW に落としておく
 digitalWrite(TRIG_PIN1, LOW);
 digitalWrite(TRIG_PIN2, LOW);
 delay(10); // 落ちきるまで少し待機

 pinMode(A6,INPUT);//ADC6を入力へ
 delay(1000);//シリアル通信初期化待ち


 servo1.setPeriodHertz(50);      // Standard 50hz servo
 servo1.attach(servo1Pin, minUs, maxUs);
  
}
// 超音波送信メソッド
void sendTrigger() {
 digitalWrite(TRIG_PIN1, HIGH);
 digitalWrite(TRIG_PIN2, HIGH);
 delayMicroseconds(10); // 送信してから一瞬だけ待機
 digitalWrite(TRIG_PIN1, LOW);
 digitalWrite(TRIG_PIN2, LOW);
}

// メインループメソッド
void loop() {
 //超音波センサ１
 sendTrigger(); // 超音波送信
 duration1 = pulseIn(ECHO_PIN1, HIGH); // 受信した時間を代入
 // 値が 0 より大きい場合（受け取れていたら）
 if (duration1 > 0) {
   duration1 /= 2; // 往復の距離を片道に変換
   // 音速を 340m/s として受信時間から距離に変換
   distance1 = duration1 * 340 / 10000;
   Serial.print("echo1 ");
   Serial.println(distance1);
 }
 delay(15);
 
 //超音波センサ2
 sendTrigger(); // 超音波送信
 duration2 = pulseIn(ECHO_PIN2, HIGH); // 受信した時間を代入
 // 値が 0 より大きい場合（受け取れていたら）
 if (duration2 > 0) {
   duration2 /= 2; // 往復の距離を片道に変換
   // 音速を 340m/s として受信時間から距離に変換
   distance2 = duration2 * 340 / 10000;
   Serial.print("echo2 ");
   Serial.println(distance2);
 }
 delay(15);
 
 //フォトトランジスタ
analogValue = analogRead(A6);//ADC6のアナログ変換値を取得
Serial.println(analogValue);

 if(distance1 < maxdist){
   Serial.println("入室");
   if(analogValue == 0){
    for (pos = 0; pos <= 60; pos += 1) { // sweep from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo1.write(pos);
      delay(2);             // waits 20ms for the servo to reach the position
    }
    for (pos = 60; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
      servo1.write(pos);
      delay(2);
    }
   }
 }else if(distance2 < maxdist){
   Serial.println("退室");
   if(analogValue >= 100){
    for (pos = 0; pos <= 60; pos += 1) { // sweep from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo1.write(pos);
      delay(2);             // waits 20ms for the servo to reach the position
    }
    for (pos = 60; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
      servo1.write(pos);
      delay(2);
    }
   }
 } 
  
 delay(1000); // 1 秒待機
} 
