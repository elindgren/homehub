package com.example.ericl.lighter;

import android.content.Context;
import android.graphics.Point;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.nio.charset.Charset;
import java.util.Calendar;

public class MainActivity extends AppCompatActivity {
    private ArduinoCommunicator arduinoCommunicator;

    private ImageButton onAllButton;
    private ImageButton offAllButton;
    private ImageButton tvButton;
    private ImageButton hylla1Button;
    private ImageButton hylla2Button;
    private ImageButton vitrinButton;
    private ImageView cat;

    private Toast toast; //General toast to show information for when onAll or offAll are pressed

    private boolean tvButtonOn = false; //initialized to off TODO - Check status from arduino!
    private boolean hylla1ButtonOn = false;
    private boolean hylla2ButtonOn = false;
    private boolean vitrinButtonOn = false;
    private RequestQueue requestQueue;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        arduinoCommunicator = new ArduinoCommunicator(this.getApplicationContext());


        requestQueue = setupArduinoCommunicator();
        setupCat();
        //Declare on-screen buttons and setup their click-events.
        setupOnAllButton();
        setupOffAllButton();
        setupTVButton();
        setupHylla1Bytton();
        setupHylla2Button();
        setupVitrinButtion();
    }
    private RequestQueue setupArduinoCommunicator(){

        //Instantiate a RequestQueue
        requestQueue = Volley.newRequestQueue(this);
        return requestQueue;
    }
    private void sendRequestArduino(String url, final CharSequence toastText){
        final TextView mTextView = (TextView)findViewById(R.id.debugTextView);

        //Request a string response form the provided URL.

        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        //Display the first 500 characters of the response string.
                        Log.d("ARDUINO_RESPONSE","Message: " + response);
                        String tmp = response.split("<html>")[1];
                        tmp = tmp.trim(); //Remove newline
                        String response_LED = tmp.split("<br />")[0];  //Fetch information about LED
                        Log.d("ARDUINO_RESPONSE", response_LED);
                        mTextView.setText(response_LED);
                        Context context = getApplicationContext();
                        int duration = Toast.LENGTH_LONG;
                        toast = Toast.makeText(context, response_LED, duration);
                        toast.show();
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                mTextView.setText("That didint work! ");
                Log.d("ARDUINO_RESPONSE","Error message: " + error.getMessage() + ", Cause: " + error.getCause());
                Context context = getApplicationContext();
                int duration = Toast.LENGTH_LONG;
                toast = Toast.makeText(context, "That didn't work", duration);
                toast.show();
            }
        });
        // Add the request to the RequestQueue
        requestQueue.add(stringRequest);
    }
    private void setupCat(){
        //Cat handler

        //First, we need to get the dimension of the scrreen
        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        int width = size.x; //Width of the screen in pixels

        cat = (ImageView)findViewById(R.id.cat);
        cat.setX((float)((Math.random()-0.5)*1.6)*(width/2));
        cat.setClickable(true);
        cat.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                Context context = getApplicationContext();
                CharSequence text = "Mjau!";
                int duration = Toast.LENGTH_SHORT;

                Toast toastCat = Toast.makeText(context, text, duration);
                toastCat.show();
                int currentHour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
                if(currentHour > 20 || currentHour < 8){
                    cat.setImageResource(R.drawable.cat_sleeping);
                }
            }
        });
    }
    private void setupOnAllButton(){
        //Turn everything on
        onAllButton = (ImageButton) findViewById(R.id.onAllButton);
        onAllButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works


                // Create the arduino request:
                String url = "http://192.168.0.196/H";
                CharSequence toastTextOnAll = "Everything turned on!";
                sendRequestArduino(url, toastTextOnAll);
            }
        });

        //Turn everything off
        offAllButton = (ImageButton)findViewById(R.id.offAllButton);

        offAllButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                Context context = getApplicationContext();
                CharSequence text = "Everything turned off!";
                int duration = Toast.LENGTH_SHORT;

                toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
    }
    private void setupOffAllButton(){

    }
    private void setupTVButton(){
        //Turn tv on or off
        tvButton = (ImageButton)findViewById(R.id.tvButton);
        tvButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                //Context context = getApplicationContext();
                //CharSequence textOn = "tvButton On!";
                //CharSequence textOff = "tvButton Off!";

                //int duration = Toast.LENGTH_SHORT;

                //Toast toastOn = Toast.makeText(context, textOn, duration);
                //Toast toastOff = Toast.makeText(context, textOff, duration);

                if(!tvButtonOn){ //If TV is off
                    //Change the button to on-sprite
                    tvButton.setImageResource(R.drawable.tv_on);

                    //toastOn.show();
                    tvButtonOn = true;
                }
                else{ //If TV is on
                    //Change the button to off-sprite
                    tvButton.setImageResource(R.drawable.tv_off);

                    //toastOff.show();
                    tvButtonOn = false;

                }
            }
        });

    }
    private void setupHylla1Bytton(){
        //Turn hylla1 on or off
        hylla1Button = (ImageButton)findViewById(R.id.hylla1Button);
        hylla1Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                //Context context = getApplicationContext();
                //CharSequence textOn = "hylla1Button On!";
                //CharSequence textOff = "hylla1Button Off!";

                //int duration = Toast.LENGTH_SHORT;

                //Toast toastOn = Toast.makeText(context, textOn, duration);
                //Toast toastOff = Toast.makeText(context, textOff, duration);

                if(!hylla1ButtonOn){ //If TV is off
                    //Change the button to on-sprite
                    hylla1Button.setImageResource(R.drawable.hylla1_on);

                    //toastOn.show();
                    hylla1ButtonOn = true;
                }
                else{ //If TV is on
                    //Change the button to off-sprite
                    hylla1Button.setImageResource(R.drawable.hylla1_off);

                    //toastOff.show();
                    hylla1ButtonOn = false;
                }
            }
        });
    }
    private void setupHylla2Button(){
        //Turn hylla2 on or off
        hylla2Button = (ImageButton)findViewById(R.id.hylla2Button);
        hylla2Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                //Context context = getApplicationContext();
                //CharSequence textOn = "hylla2Button On!";
                //CharSequence textOff = "hylla2Button Off!";

                //int duration = Toast.LENGTH_SHORT;

                //Toast toastOn = Toast.makeText(context, textOn, duration);
                //Toast toastOff = Toast.makeText(context, textOff, duration);

                if(!hylla2ButtonOn){ //If TV is off
                    //Change the button to on-sprite
                    hylla2Button.setImageResource(R.drawable.hylla2_on);

                    //toastOn.show();
                    hylla2ButtonOn = true;
                }
                else{ //If TV is on
                    //Change the button to off-sprite
                    hylla2Button.setImageResource(R.drawable.hylla2_off);

                    //toastOff.show();
                    hylla2ButtonOn = false;
                }
            }
        });

    }
    private void setupVitrinButtion(){
        // Turn vitrin on or off
        vitrinButton = (ImageButton)findViewById(R.id.vitrinButton);
        vitrinButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Generating a simple toast, to make sure it works
                //Context context = getApplicationContext();
                //CharSequence textOn = "vitrinButton On!";
                //CharSequence textOff = "vitrinButton Off!";

                //int duration = Toast.LENGTH_SHORT;

                //Toast toastOn = Toast.makeText(context, textOn, duration);
                //Toast toastOff = Toast.makeText(context, textOff, duration);

                if(!vitrinButtonOn){ //If TV is off
                    //Change the button to on-sprite
                    vitrinButton.setImageResource(R.drawable.vitrin_on);

                    //toastOn.show();
                    vitrinButtonOn = true;
                }
                else{ //If TV is on
                    //Change the button to off-sprite
                    vitrinButton.setImageResource(R.drawable.vitrin);

                    //toastOff.show();
                    vitrinButtonOn = false;
                }
            }
        });
    }
}


