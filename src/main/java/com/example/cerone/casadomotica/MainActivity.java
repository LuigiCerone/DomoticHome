package com.example.cerone.casadomotica;

import android.os.AsyncTask;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;


public class MainActivity extends ActionBarActivity {

    private String url = "http://192.168.0.254/?domotica";
    private EditText button1,button2,button3,button4,button5,button6,button7,button13,button14;
    private EditText button8,button9,button10,button11,button12,tempInt,tempEst,umidInt,umidEst;
    private ParserXML obj;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        Button dati = (Button) findViewById(R.id.button1);
        button1 = (EditText)findViewById(R.id.editText1);//Stairs.
        button2 = (EditText)findViewById(R.id.editText2);//Bathroom.
        button3 = (EditText)findViewById(R.id.editText3);//First dedroom.
        button4 = (EditText)findViewById(R.id.editText12);//Outside.
        button5 = (EditText)findViewById(R.id.editText4);//Fountain.
        button6 = (EditText)findViewById(R.id.editText5);//Living room.
        button7 = (EditText)findViewById(R.id.editText6);//Second bedroom.
        button8 = (EditText)findViewById(R.id.editText7); //Garage.
        button9 = (EditText)findViewById(R.id.editText8); // Servo.
        button10 = (EditText)findViewById(R.id.editText9);//Entrance.
        button11 = (EditText)findViewById(R.id.editText10); // Alarm.
        button12 = (EditText)findViewById(R.id.editText11); // Kitchen.
        button13 = (EditText) findViewById(R.id.editText17); //Fans.
        button14 = (EditText) findViewById(R.id.editText18); //Upstairs corridor.

        tempInt = (EditText)findViewById(R.id.editText13);
        umidInt = (EditText)findViewById(R.id.editText15);
        tempEst = (EditText)findViewById(R.id.editText14);
        umidEst = (EditText)findViewById(R.id.editText16);

        Button led1on = (Button) findViewById(R.id.led_1on); //Stairs.
        Button led1off = (Button) findViewById(R.id.led_1off);
        Button led2on = (Button) findViewById(R.id.led_2on); //Bathroom.
        Button led2off = (Button) findViewById(R.id.led_2off);
        Button led3on = (Button) findViewById(R.id.led_3on);//First bedroom.
        Button led3off = (Button) findViewById(R.id.led_3off);
        Button led4on = (Button) findViewById(R.id.led_4on);
        Button led4off = (Button) findViewById(R.id.led_4off);//Fountain.
        Button led5on = (Button) findViewById(R.id.led_5on);
        Button led5off = (Button) findViewById(R.id.led_5off);//Living room.
        Button led6on = (Button) findViewById(R.id.led_6on);
        Button led6off = (Button) findViewById(R.id.led_6off);//Second bedroom.
        Button led7on = (Button) findViewById(R.id.led_7on);
        Button led7off = (Button) findViewById(R.id.led_7off);//Garage.
        Button led8on = (Button) findViewById(R.id.led_8on);
        Button led8off = (Button) findViewById(R.id.led_8off);//Garage's door.
        Button led9on = (Button) findViewById(R.id.led_9on);
        Button led9off = (Button) findViewById(R.id.led_9off);//Entrance.
        Button led10on = (Button) findViewById(R.id.led_10on);
        Button led10off = (Button) findViewById(R.id.led_10off);//Alarm.
        Button led11on = (Button) findViewById(R.id.led_11on);
        Button led11off = (Button) findViewById(R.id.led_11off);//Kitchen.
        Button led12on = (Button) findViewById(R.id.led_12on);
        Button led12off = (Button) findViewById(R.id.led_12off);//Fans.
        Button led13on = (Button) findViewById(R.id.led_13on);
        Button led13off = (Button) findViewById(R.id.led_13off);//Upstairs corridor.

        dati.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                String finalUrl = url;
                obj = new ParserXML(finalUrl);
                obj.fetchXML();
                while(obj.parsingComplete);
                button1.setText(obj.getButton1());
                button2.setText(obj.getButton2());
                button3.setText(obj.getButton3());
                button4.setText(obj.getButton4());
                button5.setText(obj.getButton5());
                button6.setText(obj.getButton6());
                button7.setText(obj.getButton7());
                button8.setText(obj.getButton8());
                button9.setText(obj.getButton9());
                button10.setText(obj.getButton10());
                button11.setText(obj.getButton11());
                button12.setText(obj.getButton12());
                button13.setText(obj.getButton13());
                button14.setText(obj.getButton14());

                tempInt.setText(obj.getTempInt());
                tempEst.setText(obj.getTempEst());
                umidInt.setText(obj.getUmidInt());
                umidEst.setText(obj.getUmidEst());

                Toast.makeText(getApplicationContext(), "Acquisizione dati...", Toast.LENGTH_LONG).show();
            }
        });

        led1on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b1o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b1o");
                Toast.makeText(getApplicationContext(), "Scale_on", Toast.LENGTH_LONG).show();
            }
        });

        led1off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b1f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b1f");
                Toast.makeText(getApplicationContext(), "Scale_off",Toast.LENGTH_LONG).show();
            }
        });
        led2on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b2o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b2o");
                Toast.makeText(getApplicationContext(), "Bagno_on", Toast.LENGTH_LONG).show();
            }
        });

        led2off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b2f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b2f");
                Toast.makeText(getApplicationContext(), "Bagno_off",Toast.LENGTH_LONG).show();
            }
        });

        led3on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b3o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b3o");
                Toast.makeText(getApplicationContext(), "Camera1_on", Toast.LENGTH_LONG).show();
            }
        });

        led3off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b3f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b3f");
                Toast.makeText(getApplicationContext(), "Camera1_off",Toast.LENGTH_LONG).show();
            }
        });

        led4on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b4o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b4o");
                Toast.makeText(getApplicationContext(), "Fontana_on", Toast.LENGTH_LONG).show();
            }
        });

        led4off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b4f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b4f");
                Toast.makeText(getApplicationContext(), "Fontana_off",Toast.LENGTH_LONG).show();
            }
        });

        led5on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b5o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b5o");
                Toast.makeText(getApplicationContext(), "Salotto_on", Toast.LENGTH_LONG).show();
            }
        });

        led5off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b5f");
                Toast.makeText(getApplicationContext(), "Salotto_off",Toast.LENGTH_LONG).show();
            }
        });

        led6on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b6o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b6o");
                Toast.makeText(getApplicationContext(), "Camera2_on", Toast.LENGTH_LONG).show();
            }
        });

        led6off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b6f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b6f");
                Toast.makeText(getApplicationContext(), "Camera2_off",Toast.LENGTH_LONG).show();
            }
        });

        led7on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b7o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b7o");
                Toast.makeText(getApplicationContext(), "Garage_on", Toast.LENGTH_LONG).show();
            }
        });

        led7off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b7f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b7f");
                Toast.makeText(getApplicationContext(), "Garage_off",Toast.LENGTH_LONG).show();
            }
        });

        led8on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b8o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b8o");
                Toast.makeText(getApplicationContext(), "Porta_Aperta", Toast.LENGTH_LONG).show();
            }
        });

        led8off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b8f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b8f");
                Toast.makeText(getApplicationContext(), "Porta_Chiusa",Toast.LENGTH_LONG).show();
            }
        });

        led9on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b9o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b9o");
                Toast.makeText(getApplicationContext(), "Ingresso_on", Toast.LENGTH_LONG).show();
            }
        });

        led9off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b9f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b9f");
                Toast.makeText(getApplicationContext(), "Ingresso_off",Toast.LENGTH_LONG).show();
            }
        });

        led10on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b10o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b10o");
                Toast.makeText(getApplicationContext(), "Allarme_on", Toast.LENGTH_LONG).show();
            }
        });

        led10off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b10f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b10f");
                Toast.makeText(getApplicationContext(), "Allarme_off",Toast.LENGTH_LONG).show();
            }
        });

        led11on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b11o");
                Toast.makeText(getApplicationContext(), "Cucina_on", Toast.LENGTH_LONG).show();
            }
        });

        led11off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b11f");
                Toast.makeText(getApplicationContext(), "Cucina_off",Toast.LENGTH_LONG).show();
            }
        });
        led12on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b12o");
                Toast.makeText(getApplicationContext(), "Ventole_on", Toast.LENGTH_LONG).show();
            }
        });

        led12off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b12f");
                Toast.makeText(getApplicationContext(), "Ventole_off",Toast.LENGTH_LONG).show();
            }
        });
        led13on.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11o");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b13o");
                Toast.makeText(getApplicationContext(), "Corridoio_on", Toast.LENGTH_LONG).show();
            }
        });

        led13off.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                //commandArduino("http://192.168.0.254/?b11f");
                Invio inv = new Invio();
                inv.execute("http://192.168.0.254/?b13f");
                Toast.makeText(getApplicationContext(), "Corridoio_off",Toast.LENGTH_LONG).show();
            }
        });
    }

    private class Invio extends AsyncTask<String,Void,String> {

        @Override
        protected String doInBackground(String... params) {
            int count = params.length;

            for (int i = 0; i < count; i++) {

                try {
                    HttpClient httpclient = new DefaultHttpClient();
                    httpclient.execute(new HttpGet(params[i]));
                } catch (Exception e) {
                    e.printStackTrace();

                }
            }
            return null;
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}

